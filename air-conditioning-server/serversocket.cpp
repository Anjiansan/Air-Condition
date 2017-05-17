#include "serversocket.h"

ServerSocket::ServerSocket(QWidget *parent)
{

}

ServerSocket::~ServerSocket()
{

}

void ServerSocket::incomingConnection(qintptr socketDescriptor)
{
    SocketThread *thread=new SocketThread(0,socketDescriptor);
    thread->start();
}
