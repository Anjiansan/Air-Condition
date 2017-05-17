#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QtNetwork>

class ServerSocket;
#include "socketthread.h"

class ServerSocket:public QTcpServer
{
    Q_OBJECT

public:
    explicit ServerSocket(QWidget *parent = 0);
    ~ServerSocket();

protected:
    virtual void incomingConnection(qintptr socketDescriptor);

};

#endif // SERVERSOCKET_H
