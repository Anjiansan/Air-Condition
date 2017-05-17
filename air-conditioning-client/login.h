#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>
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
    void on_cancleBtn_clicked();

    void on_loginBtn_clicked();

    void loginResult(bool isOK);

private:
    Ui::Login *ui;

public:
    ClientSocket *client;
};

#endif // LOGIN_H
