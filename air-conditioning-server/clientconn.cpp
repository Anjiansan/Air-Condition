#include "clientconn.h"

ClientConn::ClientConn(qintptr socketDescriptor, QObject *parent)
{
    this->setSocketDescriptor(socketDescriptor);

    connect(this,SIGNAL(readyRead()),this,SLOT(receiveData()));
}

ClientConn::~ClientConn()
{

}

void ClientConn::sendData(QJsonDocument document)
{
    QByteArray data = document.toJson(QJsonDocument::Indented);
//    qDebug()<<data;

    this->write(data);
}

void ClientConn::receiveData()
{
//    qDebug()<<QThread::currentThreadId();

    QByteArray data=this->readAll();
    QJsonParseError err;
    QJsonDocument parse_document = QJsonDocument::fromJson(data,&err);
    if(err.error==QJsonParseError::NoError)
    {
        handleRqt(parse_document);
    }
}

void ClientConn::handleRqt(QJsonDocument parse_document)
{
    int op=parse_document.object().value("op").toInt();
    qDebug()<<op;

    if(op==LOG_IN_USER)
    {
        int room_id=parse_document.object().value("room_id").toInt();
        QString user_id=parse_document.object().value("user_id").toString();

        QJsonObject json;
        QMap<int,QString> rooms=MainWindow::getRooms();
        if(rooms.contains(room_id)&&rooms[room_id]==user_id)
        {
            json.insert("ret", LOG_IN_SUCC);
        }
        else
        {
            json.insert("ret", LOG_IN_FAIL);
        }
        QJsonDocument document;
        document.setObject(json);

        sendData(document);
    }
    else if(op==REPORT_STATE)
    {
        int is_on=parse_document.object().value("is_on").toBool();
        qDebug()<<is_on;

        QJsonObject json;
        json.insert("ret", REPLY_CON);
        json.insert("is_valid", true);
        json.insert("cost", 0.0);
        json.insert("power",0.0);
        QJsonDocument document;
        document.setObject(json);

        sendData(document);
    }
}
