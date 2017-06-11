#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, ClientSocket *client) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isRun=false;
    isHeatMode=false;   //开始初始值
    realTem=22;
    setTem=22;
    speed=2;

    speedToTime[0]=2500;    //风速-时间转换
    speedToTime[1]=2000;
    speedToTime[2]=1500;

    ui->exitBtn->setEnabled(false); //初始化界面
    ui->riseTem->setEnabled(false);
    ui->reduceTem->setEnabled(false);
    ui->riseSpeed->setEnabled(false);
    ui->reduceSpeed->setEnabled(false);
    ui->coldModeBtn->setEnabled(false);
    ui->heatModeBtn->setEnabled(false);

    this->client=client;
    connect(this->client,SIGNAL(updateMain(bool,int)),this,SLOT(updateMainSlot(bool,int)));
    connect(this->client,SIGNAL(updateUI(bool,double,double,int)),this,SLOT(updateUISlot(bool,double,double,int)));
    connect(this->client,SIGNAL(errorOccure()),this,SLOT(errorOccureSLOT()));

    this->workTimer=new QTimer(this);
    connect(this->workTimer,SIGNAL(timeout()),this,SLOT(updateRealTem()));

    this->naturalTimer=new QTimer(this);
    connect(this->naturalTimer,SIGNAL(timeout()),this,SLOT(naturalUpdateTem()));

    this->sendTimer=new QTimer(this);
    connect(this->sendTimer,SIGNAL(timeout()),this,SLOT(sendReqPeriod()));

    this->riseTemTimer=new QTimer(this);
    this->reduceTemTimer=new QTimer(this);
    connect(this->riseTemTimer,SIGNAL(timeout()),this,SLOT(sendRiseTemReq()));
    connect(this->reduceTemTimer,SIGNAL(timeout()),this,SLOT(sendReduceTemReq()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}

void MainWindow::updateMainSlot(bool mode, int temp)
{
    if(mode)
    {
        ui->mainMode->setText("制暖");
    }
    else
    {
        ui->mainMode->setText("制冷");
    }
    ui->realTemNum->display(temp);
}

void MainWindow::on_turnOnBtn_clicked()
{
    isRun=true;

    ui->turnOnBtn->setEnabled(false);   //初始化界面
    ui->exitBtn->setEnabled(true);
    ui->riseTem->setEnabled(true);
    ui->reduceTem->setEnabled(true);
    ui->riseSpeed->setEnabled(true);
    ui->reduceSpeed->setEnabled(true);
    ui->heatModeBtn->setEnabled(true);

//    ui->realTemNum->display(realTem);
    ui->setTemNum->display(setTem);
    ui->speedNum->display(speed);
    ui->mode->setText("制冷");
    outDoorTem=30;  //制冷模式室外温度默认30度
    ui->outDoorTemp->display(outDoorTem);

//    this->sendTimer->start(2000);
    this->client->sendReq(true,isHeatMode,realTem,setTem,speed);
}

void MainWindow::updateRealTem()
{
    if(isRun)
    {
        if(this->isHeatMode)    //制暖
        {
            this->realTem++;
        }
        else    //制冷
        {
            this->realTem--;
        }

        if(this->realTem==this->setTem) //达到请求温度值
        {
            client->sendReq(true,isHeatMode,setTem,realTem,speed);  //发送请求
        }

        ui->realTemNum->display(this->realTem);
    }
}

void MainWindow::naturalUpdateTem()
{
    if(isRun)
    {
        if(this->isHeatMode)    //制暖
        {
            this->realTem--;

            if(this->realTem<outDoorTem)    //最低温度为室外温度
            {
                this->realTem=outDoorTem;
            }
            client->sendReq(true,isHeatMode,setTem,realTem,speed);  //温差超过一度发送请求
        }
        else    //制冷
        {
            this->realTem++;

            if(this->realTem>outDoorTem)    //最高温度为室外温度
            {
                this->realTem=outDoorTem;
            }
            client->sendReq(true,isHeatMode,setTem,realTem,speed);  //温差超过一度发送请求
        }

        ui->realTemNum->display(this->realTem);
    }
}

void MainWindow::sendReqPeriod()
{
    if(isRun)
        client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    client->sendReq(false,isHeatMode,setTem,realTem,speed);
    client->disconnect();
    event->accept();
}

void MainWindow::errorOccureSLOT()
{
    QMessageBox::warning(this,tr("发生网络错误"),tr("请关闭重启"),QMessageBox::Yes);
    sendTimer->stop();
    workTimer->stop();
    naturalTimer->start(3000);
}

void MainWindow::sendRiseTemReq()
{
    if(isRun)
        client->sendReq(true,isHeatMode,setTem,realTem,speed);

    riseTemTimer->stop();
    qDebug()<<"stop";
}

void MainWindow::sendReduceTemReq()
{
    if(isRun)
        client->sendReq(true,isHeatMode,setTem,realTem,speed);

    reduceTemTimer->stop();
}

void MainWindow::on_exitBtn_clicked()
{
    isRun=false;

    client->sendReq(false,isHeatMode,setTem,realTem,speed);
    ui->turnOnBtn->setEnabled(true);
    ui->exitBtn->setEnabled(false);
    ui->riseTem->setEnabled(false);
    ui->reduceTem->setEnabled(false);
    ui->riseSpeed->setEnabled(false);
    ui->reduceSpeed->setEnabled(false);
    ui->coldModeBtn->setEnabled(false);
    ui->heatModeBtn->setEnabled(false);

    sendTimer->stop();  //停止定时器

//    this->close();
}

void MainWindow::updateUISlot(bool is_valid, double power, double money,int frequence)
{
    if(isRun)
    {
        ui->power->setText(QString::number(power,10,2));
        ui->money->setText(QString::number(money,10,2));
        if(!this->sendTimer->isActive())
            this->sendTimer->start(frequence);    //定时发送状态
        else if(this->frequence!=frequence)
        {
            this->sendTimer->stop();
            this->sendTimer->start(frequence);    //定时发送状态
        }
        this->frequence=frequence;

    //    QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
    //    QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
    //    qDebug()<<str<<is_valid;

        if(is_valid)
        {
            if(this->naturalTimer->isActive())  //关闭温度自然变化定时器
            {
                this->naturalTimer->stop();
            }

            if(!this->workTimer->isActive())
            {
                this->workTimer->start(speedToTime[speed-1]);
            }
        }
        else
        {
            if(this->workTimer->isActive())  //关闭空调工作定时器
            {
                this->workTimer->stop();
            }

            if(!this->naturalTimer->isActive())  //开启温度自然变化定时器
            {
                this->naturalTimer->start(3000);
            }
        }
    }
}

void MainWindow::on_riseTem_clicked()
{
    this->setTem++;

    if(isHeatMode && setTem>=30)
    {
        ui->riseTem->setEnabled(false);
    }
    if(!isHeatMode && setTem>=25)
    {
        ui->riseTem->setEnabled(false);
    }

    ui->setTemNum->display(setTem);

    if(riseTemTimer->isActive())
    {
        riseTemTimer->stop();
    }
    riseTemTimer->start(1000);
//    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_reduceTem_clicked()
{
    this->setTem--;

    if(isHeatMode && setTem<=25)
    {
        ui->reduceTem->setEnabled(false);
    }
    if(!isHeatMode && setTem<=18)
    {
        ui->reduceTem->setEnabled(false);
    }

    ui->setTemNum->display(setTem);

    if(reduceTemTimer->isActive())
    {
        reduceTemTimer->stop();
    }
    reduceTemTimer->start(1000);
//    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_riseSpeed_clicked()
{
    this->speed=this->speed<3?this->speed+1:this->speed;
    if(this->speed==3)
    {
        ui->riseSpeed->setEnabled(false);
    }
    ui->reduceSpeed->setEnabled(true);
    ui->speedNum->display(speed);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_reduceSpeed_clicked()
{
    this->speed=this->speed>1?this->speed-1:this->speed;
    if(this->speed==1)
    {
        ui->reduceSpeed->setEnabled(false);
    }
    ui->riseSpeed->setEnabled(true);
    ui->speedNum->display(speed);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_coldModeBtn_clicked()
{
    this->isHeatMode=false;
    ui->mode->setText("制冷");
    outDoorTem=30;  //制冷模式室外温度默认30度
    ui->outDoorTemp->display(outDoorTem);
    ui->heatModeBtn->setEnabled(true);
    ui->coldModeBtn->setEnabled(false);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_heatModeBtn_clicked()
{
    this->isHeatMode=true;
    ui->mode->setText("制暖");
    outDoorTem=10;  //制暖模式室外温度默认10度
    ui->outDoorTemp->display(outDoorTem);
    ui->coldModeBtn->setEnabled(true);
    ui->heatModeBtn->setEnabled(false);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}
