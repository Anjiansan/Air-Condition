#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QtNetwork>
class QMessageBox;
#include "protocol.h"

class ClientSocket:public QObject
{
    Q_OBJECT

public:
    explicit ClientSocket(QWidget *parent = 0);
    ~ClientSocket();

    void login(int room_id,QString user_id);

    void sendReq(bool in_on,bool is_heat_mode,int set_temp,int real_temp,int speed);

    bool connectServer(QString IP,int port);

signals:
    void connectError();

    void loginSignal(bool);

    void updateMain(bool,int);

    void updateUI(bool,double,double,int);

    void errorOccure();

private slots:
    void receiveData(void);

    void serverError();

private:
    QTcpSocket *client;
};

#endif // CLIENTSOCKET_H
