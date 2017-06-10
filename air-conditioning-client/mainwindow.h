#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork>
#include <QCloseEvent>
#include "clientsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent,ClientSocket *client);
    ~MainWindow();

private slots:
    void updateMainSlot(bool mode,int temp);

    void on_exitBtn_clicked();

    void updateUISlot(bool is_valid,double power,double money,int frequence);

    void on_riseTem_clicked();

    void on_reduceTem_clicked();

    void on_riseSpeed_clicked();

    void on_reduceSpeed_clicked();

    void on_coldModeBtn_clicked();

    void on_heatModeBtn_clicked();

    void on_turnOnBtn_clicked();

    void updateRealTem();

    void naturalUpdateTem();

    void sendReqPeriod();

    void closeEvent(QCloseEvent *event);

    void errorOccureSLOT();

    void sendRiseTemReq();

    void sendReduceTemReq();

private:
    Ui::MainWindow *ui;

    ClientSocket *client;
    bool isRun;  //是否在运行
    bool isHeatMode;    //是否为制暖模式
    int realTem;//实时温度
    int setTem; //设置温度
    int speed;  //风速
    int frequence;  //刷新速率
    int outDoorTem; //室外温度
    int speedToTime[3]; //风速转换为定时器时间
    QTimer *sendTimer;  //周期发送状态
    QTimer *workTimer;  //空调工作时定时器
    QTimer *naturalTimer;   //温度自然变化定时器
    QTimer *riseTemTimer;
    QTimer *reduceTemTimer;
};

#endif // MAINWINDOW_H
