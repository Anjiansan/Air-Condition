#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_loginBtn_clicked()
{
    QString account=ui->account->text();
    QString pwd=ui->pwd->text();

    if(account==ACCOUNT && pwd==PWD)
    {
        this->accept();
    }
    else
    {
         QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码错误!"),QMessageBox::Yes);
    }
}

void LoginDialog::on_cancleBtn_clicked()
{
    this->close();
}
