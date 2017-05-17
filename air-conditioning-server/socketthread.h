#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

class ClientConn;
#include "clientconn.h"

class SocketThread : public QThread
{
public:
    SocketThread(QWidget * parent,qintptr p);

protected:
    virtual void run();

private:
    qintptr ptr;
    ClientConn *client;
};

#endif // SOCKETTHREAD_H
