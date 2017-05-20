#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

class ClientConn;
#include "clientconn.h"

class SocketThread : public QThread
{
public:
    SocketThread(QWidget * parent,qintptr p,MainWindow *mainWindow);

protected:
    virtual void run();

private:
    qintptr ptr;
    MainWindow *mainWindow;
    ClientConn *client;
};

#endif // SOCKETTHREAD_H
