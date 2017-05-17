#include "mainwindow.h"
#include "ui_mainwindow.h"

QMap<int,QString> MainWindow::rooms;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server=new ServerSocket();
    server->listen(QHostAddress::AnyIPv4,11967);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete server;
}

QMap<int, QString> MainWindow::getRooms()
{
    return rooms;
}

void MainWindow::on_addRoomBtn_clicked()
{
    int room_id=ui->roomText->text().trimmed().toInt();
    QString user_id=ui->pwdText->text().trimmed();

    rooms.insert(room_id,user_id);
}
