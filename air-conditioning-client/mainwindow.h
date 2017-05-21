#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork>
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
    void on_exitBtn_clicked();

    void updateUISlot(bool is_valid,double power,double money);

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

private:
    Ui::MainWindow *ui;

    ClientSocket *client;
    bool isHeatMode;    //是否为制暖模式
    int realTem;//实时温度
    int setTem; //设置温度
    int speed;  //风速
    QTimer *sendTimer;  //周期发送状态
    QTimer *workTimer;  //空调工作时定时器
    QTimer *naturalTimer;   //温度自然变化定时器
};

#endif // MAINWINDOW_H
