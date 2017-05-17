#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

class ServerSocket;
#include "serversocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QMap<int,QString> getRooms();

private slots:
    void on_addRoomBtn_clicked();

private:
    Ui::MainWindow *ui;

    ServerSocket *server;

//public:
    static QMap<int,QString> rooms;
};

#endif // MAINWINDOW_H
