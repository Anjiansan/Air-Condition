#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include "clientsocket.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void connectErrorSlot();

    void on_cancleBtn_clicked();

    void on_loginBtn_clicked();

    void loginResult(bool isOK);

    void timeoutError();

    void closeEvent(QCloseEvent *event);

    void on_connectBtn_clicked();

private:
    Ui::Login *ui;

public:
    ClientSocket *client;
    QTimer *timer;  //主机超时回复
};

#endif // LOGIN_H
