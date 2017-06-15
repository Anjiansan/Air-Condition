#include "clientsocket.h"

ClientSocket::ClientSocket(QWidget *parent)
{
    client=new QTcpSocket(this);

    connect(client,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(this->client,SIGNAL(disconnected()),this,SLOT(serverError()));
}

ClientSocket::~ClientSocket()
{
    delete client;
}

void ClientSocket::receiveData()
{
    QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
    QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
    char d[512];
    client->read(d,512);
    std::string data(d);
    data=data.substr(0, data.find("}") + 1);
    qDebug()<<str<<QString::fromStdString(data);
    QJsonParseError err;
    QJsonDocument parse_document = QJsonDocument::fromJson(data.data(),&err);
    if(err.error==QJsonParseError::NoError)
    {
        int ret=parse_document.object().value("ret").toInt();
        if(ret==LOG_IN_SUCC)
        {
            bool mode=parse_document.object().value("is_heat_mode").toBool();
            int temp=parse_document.object().value("default").toInt();

            emit loginSignal(true);
            emit updateMain(mode,temp); //通知更新界面
        }
        else if(ret==LOG_IN_FAIL)
        {
            emit loginSignal(false);
        }
        else if(ret==REPLY_CON)
        {
            bool is_valid=parse_document.object().value("is_valid").toBool();
            double power=parse_document.object().value("power").toDouble();
            double money=parse_document.object().value("cost").toDouble();
            int frequence=parse_document.object().value("frequence").toInt();

            emit updateUI(is_valid,power,money,frequence);
        }
    }

    if(client->bytesAvailable()>0)
    {
        emit client->readyRead();
    }
}

void ClientSocket::serverError()
{
    emit errorOccure();
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

bool ClientSocket::connectServer(QString IP, int port)
{
    client->connectToHost(QHostAddress(IP),port);
    if(!client->waitForConnected(2000))
    {
        emit connectError();
        return false;
    }
    return true;
}
