#include "clientconn.h"

QVector<int> ClientConn::query;
QMutex ClientConn::mutex;

ClientConn::ClientConn(qintptr socketDescriptor, MainWindow *mainWindow, QObject *parent)
{
    this->setSocketDescriptor(socketDescriptor);
    this->mainWindow=mainWindow;

    dbData.switch_num=0;    //初始化
    isReqFinish=true;
    lastStatus=false;

    timeToPower[0]=0.8; //初始化时间-功率表
    timeToPower[1]=1;
    timeToPower[2]=1.3;

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
    QByteArray data = document.toJson(QJsonDocument::Compact);
    QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
    QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
    qDebug()<<str<<data;

    this->write(data,512);

    ClientConn::mutex.lock();   //加锁
    ClientConn::query.remove(ClientConn::query.indexOf(this->room_id));
    ClientConn::mutex.unlock(); //解锁
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
    bool isRun=mainWindow->getStatus();
    char data[512];
    this->read(data,512);

    if(isRun)
    {
        QDateTime timeNow=QDateTime::currentDateTime();//获取系统现在的时间
        QString str = timeNow.toString("hh:mm:ss"); //设置显示格式
        qDebug()<<str<<data;

    //    qDebug()<<data;
        QJsonParseError err;
        QJsonDocument parse_document = QJsonDocument::fromJson(data,&err);
        if(err.error==QJsonParseError::NoError)
        {
            handleRqt(parse_document);
        }

    }
    //    qDebug()<<this->bytesAvailable();
    if(this->bytesAvailable()>0)
    {
        emit this->readyRead();
    }
}

void ClientConn::handleRqt(QJsonDocument parse_document)
{
    int op=parse_document.object().value("op").toInt();

    if(op==LOG_IN_USER)
    {
        int room_id=parse_document.object().value("room_id").toInt();
        QString user_id=parse_document.object().value("user_id").toString();

        ClientConn::mutex.lock();   //加锁
        ClientConn::query.append(room_id);

        bool isDeal=false;
        if(ClientConn::query.size()<3)  //可以处理
            isDeal=true;
        ClientConn::mutex.unlock(); //解锁

        QJsonObject json;
        if(isDeal)  //可以处理
        {
            QMap<int,QString> rooms=this->mainWindow->getRooms();
            bool mode=mainWindow->getMode();
            int temp=mainWindow->getTemp();
            if(rooms.contains(room_id)&&rooms[room_id]==user_id)
            {
                json.insert("ret", LOG_IN_SUCC);
                json.insert("is_heat_mode", mode);
                json.insert("default", temp);
                this->room_id=room_id;  //记住room_id
                dbData.room_id=this->room_id;
                dbData.user_id=user_id;
                dbData.total_fee=mainWindow->getDBManager()->getTotalFee(room_id,user_id);
                dbData.total_power=mainWindow->getDBManager()->getTotalPower(room_id,user_id);
                qDebug()<<dbData.total_fee;

                emit clientLogined(this->room_id);  //更新界面
            }
            else
            {
                json.insert("ret", LOG_IN_FAIL);
            }
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
        ClientConn::mutex.lock();   //加锁
        ClientConn::query.append(this->room_id);

        bool isDeal=false;
        if(ClientConn::query.size()<3)  //可以处理
            isDeal=true;
        ClientConn::mutex.unlock(); //解锁

        if(isDeal)  //可以处理
        {
            bool is_on=parse_document.object().value("is_on").toBool();
            bool is_heat_mode=parse_document.object().value("is_heat_mode").toBool();
            int set_tem=parse_document.object().value("set_temp").toInt();
            int real_tem=parse_document.object().value("real_temp").toInt();
            int speed=parse_document.object().value("speed").toInt();

            if(!is_on)  //从控机关机
            {
                if(lastStatus)  //更新开关机计数
                {
                    lastStatus=false;
                    switch_num++;
                }

                emit clientOfflined(this->room_id);
                ClientConn::mutex.lock();   //加锁
                ClientConn::query.remove(ClientConn::query.indexOf(this->room_id));
                ClientConn::mutex.unlock(); //解锁
                this->thread()->quit();
            }
            else
            {
                lastStatus=true;    //开机
                bool isFinish=false;

                bool isValid=isReqValid(is_heat_mode,set_tem,real_tem);
                if(isReqFinish && isValid) //新请求
                {
                    isReqFinish=false;

                    QTime time = QTime::currentTime();//获取系统现在的时间
                    dbData.start_time=time.toString("hh:mm:ss");
                    dbData.start_temp=real_tem;
                    dbData.end_temp=set_tem;
                    dbData.speed=speed;
                    dbData.switch_num=switch_num;
                }

                if(!isReqFinish && dbData.end_temp==set_tem && real_tem==set_tem) //本次请求完成
                {
                    isReqFinish=true;
                    isFinish=true;

                    QTime time = QTime::currentTime();//获取系统现在的时间
                    dbData.end_time=time.toString("hh:mm:ss");
                    QTime t=QTime::fromString(dbData.start_time,"hh:mm:ss");
                    double minite=t.msecsTo(time)/1000.0/60.0;
//                    qDebug()<<t.msecsTo(time);
                    dbData.power=minite*timeToPower[dbData.speed-1];
//                    qDebug()<<dbData.power;
                    dbData.fee=dbData.power*5;
                    dbData.total_fee+=dbData.fee;
                    dbData.total_power+=dbData.power;
                    dbData.isOneReq=1;

                    DBManager *db=mainWindow->getDBManager();   //写入数据库
                    db->insertData(dbData);
//                    qDebug()<<"insert";
                }

                if(!isReqFinish && isValid && dbData.start_temp!=real_tem && dbData.end_temp!=set_tem) //本次请求未完成更改请求
                {
                    isFinish=true;

                    QTime time = QTime::currentTime();//获取系统现在的时间
                    dbData.end_time=time.toString("hh:mm:ss");
                    QTime t=QTime::fromString(dbData.start_time,"hh:mm:ss");
                    double minite=t.msecsTo(time)/1000.0/60.0;
                    dbData.power=minite*timeToPower[dbData.speed-1];
                    dbData.fee=dbData.power*5;
                    dbData.total_fee+=dbData.fee;
                    dbData.total_power+=dbData.power;
                    dbData.end_temp=real_tem;
                    dbData.isOneReq=1;

                    DBManager *db=mainWindow->getDBManager();   //写入数据库
                    db->insertData(dbData);

                    time = QTime::currentTime();//获取系统现在的时间
                    dbData.start_time=time.toString("hh:mm:ss");    //新请求
                    dbData.start_temp=real_tem;
                    dbData.end_temp=set_tem;
                    dbData.speed=speed;
                    dbData.switch_num=switch_num;
                }

                if(!isFinish && isValid && real_tem!=dbData.start_temp)   //实时记录,防止主机突然出错,未写入数据库
                {
                    QTime time = QTime::currentTime();//获取系统现在的时间
                    dbData.end_time=time.toString("hh:mm:ss");
                    QTime t=QTime::fromString(dbData.start_time,"hh:mm:ss");
                    double minite=t.msecsTo(time)/1000.0/60.0;
                    dbData.power=minite*timeToPower[dbData.speed-1];
                    dbData.fee=dbData.power*5;
                    dbData.total_fee+=dbData.fee;
                    dbData.total_power+=dbData.power;
                    dbData.isOneReq=0;
                    qDebug()<<dbData.power<<dbData.fee;

                    DBManager *db=mainWindow->getDBManager();   //写入数据库
                    db->insertData(dbData);
                }

                emit updateData(room_id,is_heat_mode,set_tem,real_tem,speed);   //更新客户端数据

                int frequence=mainWindow->getFlashSpeed();
                QJsonObject json;
                json.insert("ret", REPLY_CON);
                json.insert("is_valid", isValid);
                json.insert("cost", dbData.total_fee);
                json.insert("power",dbData.total_power);
                json.insert("frequence",frequence);
                QJsonDocument document;
                document.setObject(json);

                sendData(document);

                if(!isFinish && isValid && real_tem!=dbData.start_temp)
                {
                    dbData.total_fee-=dbData.fee;
                    dbData.total_power-=dbData.power;
                }
            }
        }
        else
        {
            QJsonObject json;
            json.insert("ret", REPLY_CON);
            json.insert("is_valid", false);
            json.insert("cost", dbData.total_fee);
            json.insert("power",dbData.total_power);
            QJsonDocument document;
            document.setObject(json);

            sendData(document);
        }
    }
}
