#include "socketthread.h"

SocketThread::SocketThread(QWidget *parent, qintptr p)
{
    this->ptr=p;
}

void SocketThread::run()
{
    ClientConn *client=new ClientConn(this->ptr);
//    client->waitForBytesWritten();
//    qDebug()<<"线程1";
    this->exec();
}
