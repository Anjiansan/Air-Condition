#include "clientsocket.h"

ClientSocket::ClientSocket(QWidget *parent)
{
    client=new QTcpSocket(this);
    client->connectToHost(QHostAddress("127.0.0.1"),2222);

    connect(client,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

ClientSocket::~ClientSocket()
{
    delete client;
}

void ClientSocket::receiveData()
{
    QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
    QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
    char data[512];
    client->read(data,512);
    qDebug()<<str<<data;
    QJsonParseError err;
    QJsonDocument parse_document = QJsonDocument::fromJson(data,&err);
    if(err.error==QJsonParseError::NoError)
    {
        int ret=parse_document.object().value("ret").toInt();
//        qDebug()<<ret;4
        if(ret==LOG_IN_SUCC)
        {
            emit loginSignal(true);
        }
        else if(ret==LOG_IN_FAIL)
        {
            emit loginSignal(false);
        }
        else if(ret==REPLY_CON)
        {
            bool is_valid=parse_document.object().value("is_valid").toBool();
            double power=parse_document.object().value("power").toDouble();
            double money=parse_document.object().value("money").toDouble();

            emit updateUI(is_valid,power,money);
        }
    }

    if(client->bytesAvailable()>0)
    {
        emit client->readyRead();
    }
}

void ClientSocket::login(int room_id, QString user_id)
{
    QJsonObject json;
    json.insert("op", LOG_IN_USER);
    json.insert("room_id", room_id);
    json.insert("user_id", user_id);

    QJsonDocument document;
    document.setObject(json);
    QByteArray data = document.toJson(QJsonDocument::Compact);
    qDebug()<<data;

    client->write(data,512);
}

void ClientSocket::sendReq(bool in_on, bool is_heat_mode, int set_temp, int real_temp, int speed)
{
    QJsonObject json;
    json.insert("op", REPORT_STATE);
    json.insert("is_on", in_on);
    json.insert("is_heat_mode", is_heat_mode);
    json.insert("set_temp", set_temp);
    json.insert("real_temp", real_temp);
    json.insert("speed", speed);

    QJsonDocument document;
    document.setObject(json);
    QByteArray data = document.toJson(QJsonDocument::Compact);
//    qDebug()<<data;

    client->write(data,512);
    QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
    QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
    qDebug()<<str<<data;
}
