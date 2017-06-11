#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QtNetwork>
#include <QCloseEvent>

class ServerSocket;
#include "serversocket.h"
class DBManager;
#include "dbmanager.h"

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

    int getFlashSpeed();

    int getStatus();

    DBManager *getDBManager();

    bool isLoginValid(int room_id);

private slots:
    void showClientState(QTreeWidgetItem* item,int column);

    void clientLoginedSlot(int room_id);

    void clientOfflinedSlot(int room_id);

    void updateDataSlot(int room_id,bool is_heat_mode,int setTem,int realTem,int speed,double money,double power);

    void on_addRoomBtn_clicked();

    void on_heatModeBtn_clicked();

    void on_coldModeBtn_clicked();

    void on_tempBox_valueChanged(int arg1);

    void on_flashSpdBox_valueChanged(int arg1);

    void on_turnOnBtn_clicked();

    void on_turnOffBtn_clicked();

    void closeEvent(QCloseEvent *event);

    void on_dayReport_clicked();

    void on_weekReport_clicked();

    void on_monthReport_clicked();

private:
    Ui::MainWindow *ui;

    bool isRun; //是否已运行
    bool isHeatMode;    //工作模式
    int temp;   //温度
    int flashSpeed; //刷新频率

    ServerSocket *server;
    QMap<int,QString> rooms; //房间列表
    QMap<int,struct ClientData*> clientsData;    //在线客户端数据
    QTreeWidgetItem *loginedWidget;   //在线客户端

    DBManager *dbManage; //持久层:数据库类
};

#endif // MAINWINDOW_H
