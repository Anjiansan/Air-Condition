#ifndef CLIENTCONN_H
#define CLIENTCONN_H

#include <QtNetwork>
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

private:
    MainWindow *mainWindow;
    int room_id;

};

#endif // CLIENTCONN_H
