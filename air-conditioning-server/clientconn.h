#ifndef CLIENTCONN_H
#define CLIENTCONN_H

#include <QtNetwork>
#include <QMutex>
class MainWindow;
#include "mainwindow.h"
#include "protocol.h"

//class MainWindow;

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

    static QVector<int> query;   //请求队列
    static QMutex mutex;    //线程锁
};

#endif // CLIENTCONN_H
