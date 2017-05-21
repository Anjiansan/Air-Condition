#include "clientconn.h"

ClientConn::ClientConn(qintptr socketDescriptor, MainWindow *mainWindow, QObject *parent)
{
    this->setSocketDescriptor(socketDescriptor);
    this->mainWindow=mainWindow;

    connect(this,SIGNAL(readyRead()),this,SLOT(receiveData()));
    connect(this,SIGNAL(clientLogined(int)),this->mainWindow,SLOT(clientLoginedSlot(int)));
    connect(this,SIGNAL(clientOfflined(int)),this->mainWindow,SLOT(clientOfflinedSlot(int)));
    connect(this,SIGNAL(updateData(int,bool,int,int,int)),this->mainWindow,SLOT(updateDataSlot(int,bool,int,int,int)));
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

bool ClientConn::isReqValid(bool isHeatMode, int setTem, int realTem)
{
    bool mode=mainWindow->getMode();    //主控机模式

    if(mode==isHeatMode)
    {
        if(isHeatMode)  //制暖
        {
            if(realTem<setTem)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else    //制冷
        {
            if(realTem>setTem)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
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
        QMap<int,QString> rooms=this->mainWindow->getRooms();
        if(rooms.contains(room_id)&&rooms[room_id]==user_id)
        {
            json.insert("ret", LOG_IN_SUCC);

            this->room_id=room_id;  //记住room_id
            emit clientLogined(this->room_id);  //更新界面
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
        bool is_on=parse_document.object().value("is_on").toBool();
        bool is_heat_mode=parse_document.object().value("is_heat_mode").toBool();
        int set_tem=parse_document.object().value("set_temp").toInt();
        int real_tem=parse_document.object().value("real_temp").toInt();
        int speed=parse_document.object().value("speed").toInt();

        if(!is_on)  //从控机关机
        {
            emit clientOfflined(this->room_id);
            this->thread()->quit();
        }
        else
        {
            emit updateData(room_id,is_heat_mode,set_tem,real_tem,speed);   //更新客户端数据

            QJsonObject json;
            json.insert("ret", REPLY_CON);
            json.insert("is_valid", isReqValid(is_heat_mode,set_tem,real_tem));
            json.insert("cost", 0.0);
            json.insert("power",0.0);
            QJsonDocument document;
            document.setObject(json);

            sendData(document);

        }
    }
}
