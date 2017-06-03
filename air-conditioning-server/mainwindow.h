#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QtNetwork>

class ServerSocket;
#include "serversocket.h"

class ClientData
{
public:
    bool is_on; //是否开机
    bool is_heat_mode;  //工作模式
    int set_temp;   //the temperature user wants to get
    int real_temp;  //the actual temperature in user's room
    int speed;  //1..3
    double power;
    double money;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMap<int,QString> getRooms();

    bool getMode();

    int getTemp();

private slots:
    void showClientState(QTreeWidgetItem* item,int column);

    void clientLoginedSlot(int room_id);

    void clientOfflinedSlot(int room_id);

    void updateDataSlot(int room_id,bool is_heat_mode,int setTem,int realTem,int speed);

    void on_addRoomBtn_clicked();

    void on_heatModeBtn_clicked();

    void on_coldModeBtn_clicked();

    void on_tempBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;

    bool isHeatMode;    //工作模式
    int temp;   //温度

    ServerSocket *server;
    QMap<int,QString> rooms; //房间列表
    QMap<int,struct ClientData*> clientsData;    //在线客户端数据
    QTreeWidgetItem *loginedWidget;   //在线客户端
};

#endif // MAINWINDOW_H
