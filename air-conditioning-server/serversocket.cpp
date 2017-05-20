#include "serversocket.h"

ServerSocket::ServerSocket(QWidget *parent):
    QTcpServer(parent)
{

}

ServerSocket::~ServerSocket()
{

}

void ServerSocket::incomingConnection(qintptr socketDescriptor)
{
    SocketThread *thread=new SocketThread(0,socketDescriptor,(MainWindow *)this->parent());
    thread->start();
}
