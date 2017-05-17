#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, ClientSocket *client) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->client=client;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    client->login(0,"0");
}

void MainWindow::on_reqStopBtn_clicked()
{
    client->sendReq(false,false,22,22,2);
}

void MainWindow::on_reqResumeBtn_clicked()
{
    client->sendReq(true,false,22,22,2);
}

void MainWindow::on_reqUpdateBnt_clicked()
{
    client->sendReq(true,false,22,22,2);
}

void MainWindow::on_rptStateBtm_clicked()
{
    client->sendReq(true,false,22,22,2);
}
