#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    client=new ClientSocket();
    timer=new QTimer(this);

    connect(client,SIGNAL(loginSignal(bool)),this,SLOT(loginResult(bool)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutError()));
    connect(client,SIGNAL(connectError()),this,SLOT(connectErrorSlot()));

    client->connectServer();
}

Login::~Login()
{
    delete ui;
}

void Login::connectErrorSlot()
{
    QMessageBox::warning(this,tr("连接失败"),tr("连接主机失败,请重启!"),QMessageBox::Yes);
    exit(-1);
}

void Login::on_cancleBtn_clicked()
{
    this->close();
}

void Login::on_loginBtn_clicked()
{
    QString account=ui->accountText->text().trimmed();
    QString pwd=ui->pwdText->text().trimmed();

    QByteArray ba = account.toLatin1();//QString 转换为 char*
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;
    if(*s)
    {
        QMessageBox::warning(this,tr("登录失败"),tr("用户名错误!"),QMessageBox::Yes);
    }
    else
    {
        client->login(account.toInt(),pwd);

        timer->start(2000);
    }

}

void Login::loginResult(bool isOK)
{
    timer->stop();

    if(isOK)
    {
        this->accept();
    }
    else
    {
        QMessageBox::warning(this,tr("登录失败"),tr("用户名或密码错误或已登录!"),QMessageBox::Yes);
    }
}

void Login::timeoutError()
{
    QMessageBox::warning(this,tr("登录失败"),tr("主机超时未回复,请重试!"),QMessageBox::Yes);
    timer->stop();
}

void Login::closeEvent(QCloseEvent *event)
{
    client->disconnect();
    event->accept();
}
