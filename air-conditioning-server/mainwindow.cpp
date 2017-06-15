#include "mainwindow.h"
#include "ui_mainwindow.h"

//QMap<int,QString> MainWindow::rooms;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isRun=false;
    this->isHeatMode=false; //制冷模式
    ui->coldModeBtn->setEnabled(false);
    temp=22;    //制冷默认温度
    ui->tempBox->setValue(22);
    ui->tempBox->setRange(18,25);

    ui->turnOnBtn->setEnabled(true);
    ui->turnOffBtn->setEnabled(false);
    ui->heatModeBtn->setEnabled(false);
    ui->coldModeBtn->setEnabled(false);
//    ui->addRoomBtn->setEnabled(false);

    flashSpeed=2000;    //默认刷新频率
    ui->flashSpdBox->setRange(1000,1000000);
    ui->flashSpdBox->setValue(2000);

    server=new ServerSocket(this);
    server->listen(QHostAddress::AnyIPv4,2222);

    rooms.insert(0,"0");
    rooms.insert(1,"1");
    ui->roomNum->addItem("0");
    ui->roomNum->addItem("1");
    ui->roomNum->setCurrentIndex(0);

    dbManage=new DBManager();

    ui->treeWidget->setColumnCount(1);  //初始化客户端列表

    loginedWidget = new QTreeWidgetItem(ui->treeWidget,QStringList(QString("已登录客户端")));
//    QTreeWidgetItem *leaf1=new QTreeWidgetItem(logined, QStringList(QString("Leaf 1")));
//    root->removeChild(leaf1);
//    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root, QStringList(QString("Leaf 2")));
//    leaf2->setCheckState(0, Qt::Checked);

//    QList<QTreeWidgetItem *> rootList;
//    rootList << root;
//    ui->treeWidget->insertTopLevelItems(0, rootList);

    ui->treeWidget->show();

    connect(ui->treeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(showClientState(QTreeWidgetItem*,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
    delete loginedWidget;
}

QMap<int, QString> MainWindow::getRooms()
{
    return rooms;
}

bool MainWindow::getMode()
{
    return isHeatMode;
}

int MainWindow::getTemp()
{
    return temp;
}

int MainWindow::getFlashSpeed()
{
    return flashSpeed;
}

int MainWindow::getStatus()
{
    return isRun;
}

DBManager *MainWindow::getDBManager()
{
    return dbManage;
}

bool MainWindow::isLoginValid(int room_id)
{
    if(clientsData.keys().contains(room_id))
        return false;
    else
        return true;
}

void MainWindow::showClientState(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *parent = item->parent();
    if(NULL==parent) //注意：最顶端项是没有父节点的，双击这些项时注意(陷阱)
        return;
    int col = parent->indexOfChild(item); //item在父项中的节点行号(从0开始)

    ui->setTemNum->display(QString::number(clientsData.value(item->text(0).toInt())->set_temp));
    ui->realTemNum->display(QString::number(clientsData.value(item->text(0).toInt())->real_temp));
    ui->speedNum->display(QString::number(clientsData.value(item->text(0).toInt())->speed));
    if(clientsData.value(item->text(0).toInt())->is_heat_mode)
    {
        ui->mode->setText("制暖");
    }
    else
    {
        ui->mode->setText("制冷");
    }
    ui->power->setText(QString::number(0));
    ui->money->setText(QString::number(0));
}

void MainWindow::clientLoginedSlot(int room_id)
{
    new QTreeWidgetItem(loginedWidget, QStringList(QString::number(room_id)));
    ui->treeWidget->expandAll();
    clientsData.insert(room_id,new ClientData{false,false,0,0,0,0.0,0.0});

    if(!ui->treeWidget->currentIndex().isValid())
    {
        ui->treeWidget->setCurrentItem(loginedWidget->child(0));
    }
}

void MainWindow::clientOfflinedSlot(int room_id)
{
    for(int i=0;i<loginedWidget->childCount();i++)  //更新UI
    {
        if(loginedWidget->child(i)->text(0).toInt()==room_id)
        {
            loginedWidget->removeChild(loginedWidget->child(i));
            break;
        }
    }

    clientsData.take(room_id);
}

void MainWindow::updateDataSlot(int room_id, bool is_heat_mode, int setTem, int realTem, int speed, double money, double power)
{
    clientsData.value(room_id)->is_heat_mode=is_heat_mode;
    clientsData.value(room_id)->set_temp=setTem;
    clientsData.value(room_id)->real_temp=realTem;
    clientsData.value(room_id)->speed=speed;
    clientsData.value(room_id)->money=money;
    clientsData.value(room_id)->power=power;

    if(room_id==ui->treeWidget->currentItem()->text(0).toInt())
    {
        ui->setTemNum->display(setTem);
        ui->realTemNum->display(realTem);
        ui->speedNum->display(speed);
        if(is_heat_mode)
            ui->mode->setText("制暖");
        else
            ui->mode->setText("制冷");
        ui->money->setText(QString::number(money,10,2));
        ui->power->setText(QString::number(power,10,2));
    }
}

void MainWindow::on_addRoomBtn_clicked()
{
    int room_id=ui->roomText->text().trimmed().toInt();
    QString user_id=ui->pwdText->text().trimmed();

    rooms.insert(room_id,user_id);

    ui->roomNum->clear();
    for(int i=0;i<rooms.size();i++)
    {
        ui->roomNum->addItem(QString::number(rooms.keys().at(i)));
    }
    ui->roomNum->setCurrentIndex(0);

    ui->roomText->clear();  //添加完清除
    ui->pwdText->clear();
}

void MainWindow::on_heatModeBtn_clicked()
{
    this->isHeatMode=true;  //更换工作模式
    temp=28;    //供暖默认温度
    ui->tempBox->setRange(25,30);
    ui->tempBox->setValue(28);

    ui->heatModeBtn->setEnabled(false);
    ui->coldModeBtn->setEnabled(true);
}

void MainWindow::on_coldModeBtn_clicked()
{
    this->isHeatMode=false;  //更换工作模式
    temp=22;    //制冷默认温度
    ui->tempBox->setRange(18,25);
    ui->tempBox->setValue(22);

    ui->heatModeBtn->setEnabled(true);
    ui->coldModeBtn->setEnabled(false);
}

void MainWindow::on_tempBox_valueChanged(int arg1)
{
    temp=arg1;
}

void MainWindow::on_flashSpdBox_valueChanged(int arg1)
{
    flashSpeed=arg1;
}

void MainWindow::on_turnOnBtn_clicked()
{
    isRun=true;

    ui->turnOnBtn->setEnabled(false);
    ui->turnOffBtn->setEnabled(true);
    ui->heatModeBtn->setEnabled(true);
    ui->coldModeBtn->setEnabled(true);
//    ui->addRoomBtn->setEnabled(true);
}

void MainWindow::on_turnOffBtn_clicked()
{
    isRun=false;

    ui->turnOnBtn->setEnabled(true);
    ui->turnOffBtn->setEnabled(false);
    ui->heatModeBtn->setEnabled(false);
    ui->coldModeBtn->setEnabled(false);
//    ui->addRoomBtn->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->server->disconnect();
    event->accept();
}

void MainWindow::on_dayReport_clicked()
{
    int day=ui->timeWidget->selectedDate().day();
    qDebug()<<day;
    dbManage->genDayReport(ui->roomNum->currentText().toInt(),day);
}

void MainWindow::on_weekReport_clicked()
{
    int week=ui->timeWidget->selectedDate().weekNumber();
    qDebug()<<week;
    dbManage->genWeekReport(ui->roomNum->currentText().toInt(),week);
}

void MainWindow::on_monthReport_clicked()
{
    int month=ui->timeWidget->selectedDate().month();
    qDebug()<<month;
    dbManage->genMonthReport(ui->roomNum->currentText().toInt(),month);
}
