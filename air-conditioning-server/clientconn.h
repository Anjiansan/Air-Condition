#ifndef CLIENTCONN_H
#define CLIENTCONN_H

#include <QtNetwork>
#include <QMutex>
class MainWindow;
#include "mainwindow.h"
//class DBData;
//#include "dbmanager.h"
#include "protocol.h"

class DBData
{
public:
    int room_id;    //房间号
    QString user_id;    //密码
    int switch_num; //开关机次数(每日)
    QString start_time; //开始时间
    QString end_time;   //结束时间
    int start_temp;     //起始温度
    int end_temp;       //结束时间
    int speed;          //风速
    double fee;         //费用(每次)
    double total_fee;   //费用
    double power;       //功率(每次)
    double total_power; //总功率
    int isOneReq;   //是否是一次完整的请求
};

class ClientConn : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ClientConn(qintptr socketDescriptor,MainWindow *mainWindow,QObject *parent = 0);
    ~ClientConn();

signals:
    void clientLogined(int);

    void clientOfflined(int);

    void updateData(int,bool,int,int,int);

public slots:
    void receiveData(void);

    void handleRqt(QJsonDocument parse_document);

    void sendData(QJsonDocument document);

    bool isReqValid(bool isHeatMode,int setTem,int realTem);

private:
    MainWindow *mainWindow;
    int room_id;
    int switch_num; //开关机次数
    DBData dbData;  //写入数据库的数据
    bool isReqFinish;   //本次请求是否结束
    bool lastStatus;    //上次从控机开关机状态,开机:true
    double timeToPower[3];  //时间转功率表

    static QVector<int> query;   //请求队列
    static QMutex mutex;    //线程锁
};

#endif // CLIENTCONN_H
