#include "mainwindow.h"
#include "ui_mainwindow.h"

//QMap<int,QString> MainWindow::rooms;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->isHeatMode=false; //制冷模式
    ui->coldModeBtn->setEnabled(false);

    server=new ServerSocket(this);
    server->listen(QHostAddress::AnyIPv4,11967);

    rooms.insert(0,"0");

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

    clientsData.insert(room_id,new ClientData{false,false,0,0,0,0.0,0.0});
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

//    clientsData.take(room_id);
}

void MainWindow::updateDataSlot(int room_id, bool is_heat_mode, int setTem, int realTem, int speed)
{
    clientsData.value(room_id)->is_heat_mode=is_heat_mode;
    clientsData.value(room_id)->set_temp=setTem;
    clientsData.value(room_id)->real_temp=realTem;
    clientsData.value(room_id)->speed=speed;
}

void MainWindow::on_addRoomBtn_clicked()
{
    int room_id=ui->roomText->text().trimmed().toInt();
    QString user_id=ui->pwdText->text().trimmed();

    rooms.insert(room_id,user_id);

    ui->roomText->clear();  //添加完清除
    ui->pwdText->clear();
}

void MainWindow::on_heatModeBtn_clicked()
{
    this->isHeatMode=true;  //更换工作模式

    ui->heatModeBtn->setEnabled(false);
    ui->coldModeBtn->setEnabled(true);
}

void MainWindow::on_coldModeBtn_clicked()
{
    this->isHeatMode=false;  //更换工作模式

    ui->heatModeBtn->setEnabled(true);
    ui->coldModeBtn->setEnabled(false);
}
