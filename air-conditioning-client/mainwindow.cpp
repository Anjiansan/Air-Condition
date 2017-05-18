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
    ui->realTemNum->display(22);
    ui->setTemNum->display(22);
    ui->mode->setText("制冷");
    ui->power->setText(QString::number(power,10,2));
    ui->money->setText(QString::number(money,10,2));
}

void MainWindow::on_riseTem_clicked()
{
    this->setTem++;
    ui->setTemNum->display(setTem);
    ui->setTemNum->repaint();
    this->client->sendReq(true,isHeatMode,setTem,realTem,speed);
}
