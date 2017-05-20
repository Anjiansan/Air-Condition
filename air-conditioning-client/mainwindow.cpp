#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, ClientSocket *client) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    isHeatMode=false;   //开始初始值
    realTem=22;
    setTem=22;
    speed=2;

    ui->realTemNum->display(realTem);
    ui->setTemNum->display(setTem);
    ui->speedNum->display(speed);
    ui->mode->setText("制冷");
    ui->coldModeBtn->setEnabled(false);

    this->client=client;
    connect(this->client,SIGNAL(updateUI(bool,double,double)),this,SLOT(updateUISlot(bool,double,double)));

    this->client->sendReq(true,isHeatMode,realTem,setTem,speed);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete client;
}

void MainWindow::on_exitBtn_clicked()
{
    client->sendReq(false,false,22,22,2);

    this->close();
}

void MainWindow::updateUISlot(bool is_valid, double power, double money)
{
    ui->power->setText(QString::number(power,10,2));
    ui->money->setText(QString::number(money,10,2));
}

void MainWindow::on_riseTem_clicked()
{
    this->setTem++;
    ui->setTemNum->display(setTem);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_reduceTem_clicked()
{
    this->setTem--;
    ui->setTemNum->display(setTem);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
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
    ui->heatModeBtn->setEnabled(true);
    ui->coldModeBtn->setEnabled(false);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}

void MainWindow::on_heatModeBtn_clicked()
{
    this->isHeatMode=true;
    ui->mode->setText("制暖");
    ui->coldModeBtn->setEnabled(true);
    ui->heatModeBtn->setEnabled(false);
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}
