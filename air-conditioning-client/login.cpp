#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    client=new ClientSocket();

    connect(client,SIGNAL(loginSignal(bool)),this,SLOT(loginResult(bool)));
}

Login::~Login()
{
    delete ui;
}

void Login::on_cancleBtn_clicked()
{
    this->close();
}

void Login::on_loginBtn_clicked()
{
    QString account=ui->accountText->text().trimmed();
    QString pwd=ui->pwdText->text().trimmed();

    client->login(account.toInt(),pwd);

}

void Login::loginResult(bool isOK)
{
    if(isOK)
    {
        this->accept();
    }
    else
    {
        QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码错误!"),QMessageBox::Yes);
    }
}
