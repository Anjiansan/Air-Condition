#include "clientsocket.h"

ClientSocket::ClientSocket(QWidget *parent)
{
    client=new QTcpSocket(this);
    client->connectToHost(QHostAddress("127.0.0.1"),11967);

    connect(client,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

ClientSocket::~ClientSocket()
{
    delete client;
}

void ClientSocket::receiveData()
{
    QByteArray data=client->readAll();
    QJsonParseError err;
    QJsonDocument parse_document = QJsonDocument::fromJson(data,&err);
    if(err.error==QJsonParseError::NoError)
    {
        int ret=parse_document.object().value("ret").toInt();
        if(ret==LOG_IN_SUCC)
        {
            emit loginSignal(true);
        }
        else if(ret==LOG_IN_FAIL)
        {
            emit loginSignal(false);
        }
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
    QByteArray data = document.toJson(QJsonDocument::Indented);
    qDebug()<<data;

    client->write(data);
}

void ClientSocket::sendReq(bool in_on, bool is_heat_mode, int set_temp, int real_temp, int speed)
{
    QJsonObject json;
    json.insert("op", REPORT_STATE);
    json.insert("in_on", in_on);
    json.insert("is_heat_mode", is_heat_mode);
    json.insert("set_temp", set_temp);
    json.insert("real_temp", real_temp);
    json.insert("speed", speed);

    QJsonDocument document;
    document.setObject(json);
    QByteArray data = document.toJson(QJsonDocument::Indented);
    qDebug()<<data;

    client->write(data);
}
