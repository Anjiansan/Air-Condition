#include "socketthread.h"

SocketThread::SocketThread(QWidget *parent, qintptr p, MainWindow *mainWindow)
{
    this->ptr=p;
    this->mainWindow=mainWindow;
}

void SocketThread::run()
{
    ClientConn *client=new ClientConn(this->ptr,this->mainWindow);
//    client->waitForBytesWritten();
//    qDebug()<<"线程1";
    this->exec();
}
