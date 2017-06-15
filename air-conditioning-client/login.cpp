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

//    ui->accountLab->setVisible(false);
//    ui->accountText->setVisible(false);
//    ui->pwdLab->setVisible(false);
//    ui->pwdText->setVisible(false);
//    ui->cancleBtn->setVisible(false);
//    ui->loginBtn->setVisible(false);

    ui->IP->setText("192.168.43.");
    ui->port->setText("6666");
}

Login::~Login()
{
    delete ui;
}

void Login::connectErrorSlot()
{
    QMessageBox::warning(this,tr("连接失败"),tr("连接主机失败,请重试!"),QMessageBox::Yes);
//    exit(-1);
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

void Login::on_connectBtn_clicked()
{
    QString IP=ui->IP->text();
    QString port=ui->port->text();

    if(client->connectServer(IP,port.toInt()))
        QMessageBox::warning(this,tr("连接成功"),tr("连接主机成功!"),QMessageBox::Yes);
//    if(client->connectServer(IP,port.toInt()))
//    {
//        ui->IPLab->setVisible(false);
//        ui->IP->setVisible(false);
//        ui->portLab->setVisible(false);
//        ui->port->setVisible(false);
//        ui->connectBtn->setVisible(false);

//        ui->accountLab->setVisible(true);
//        ui->accountText->setVisible(true);
//        ui->pwdLab->setVisible(true);
//        ui->pwdText->setVisible(true);
//        ui->cancleBtn->setVisible(true);
//        ui->loginBtn->setVisible(true);
//    }
}
