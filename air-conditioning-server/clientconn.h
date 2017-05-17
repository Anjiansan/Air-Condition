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
    explicit ClientConn(qintptr socketDescriptor,QObject *parent = 0);
    ~ClientConn();


public slots:
    void receiveData(void);

    void handleRqt(QJsonDocument parse_document);

    void sendData(QJsonDocument document);

};

#endif // CLIENTCONN_H
