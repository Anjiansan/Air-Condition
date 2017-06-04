#include "dbmanager.h"

DBManager::DBManager()
{
    db=new QSqlDatabase();  //连接数据库
    *db=QSqlDatabase::addDatabase ("QMYSQL");
    db->setDatabaseName ("airCondition");
    db->setUserName ("huang");
    db->setPassword ("1996");
    if(!db->open ())
    {
        qDebug()<<"Failed to mysql";
    }

    if(!createTable())
    {
        qDebug()<<"Failed to create table";
    }

    weeks.append("周日"); //初始化
    weeks.append("周一");
    weeks.append("周二");
    weeks.append("周三");
    weeks.append("周四");
    weeks.append("周五");
    weeks.append("周六");
}

bool DBManager::createTable()   //创建表
{
    QSqlQuery query;
    QString sqlString=QString("create table if not exists airCondition("
                        "room_id int,"
                        "user_id char(32),"
                        "switch_num int,"
                        "month int,"
                        "week int,"
                        "day int,"
                        "start_time char(16),"
                        "end_time char(16),"
                        "start_temp int,"
                        "end_temp int,"
                        "speed int,"
                        "fee float(6,2),"
                        "total_fee float(6,2),"
                        "power float(6,2),"
                        "total_power float(6,2));");

    return query.exec (sqlString);
}

bool DBManager::insertData(DBData data)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString month = time.toString("M");
    int week = weeks.indexOf(time.toString("ddd"));
    QString day = time.toString("d");

    QSqlQuery query;
    QString sqlString=QObject::tr("insert into airCondition values(%1,\"%2\",%3,%4,%5,%6,\"%7\",\"%8\",%9,%10,%11,%12,%13,%14,%15);").
            arg (data.room_id).arg (data.user_id).arg (data.switch_num).arg (month).arg (week).arg (day).
            arg (data.start_time).arg (data.end_time).arg (data.start_temp).arg (data.end_temp).
            arg (data.speed).arg (data.fee).arg (data.total_fee).arg (data.power).arg (data.total_power);

    qDebug()<<sqlString;
    return query.exec (sqlString);
}

double DBManager::getTotalFee(int room_id, QString user_id)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select total_fee from airCondition where room_id=%1 and user_id=\"%2\";").
            arg(room_id).arg(user_id);
    query.exec (sqlString);

    if(!query.next ())//数据库不存在此用户
    {
        return 0.0;
    }
    else
    {
        query.last();
        qDebug()<<query.value("total_fee").toDouble();
        return query.value("total_fee").toDouble();
    }
}

double DBManager::getTotalPower(int room_id, QString user_id)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select total_power from airCondition where room_id=%1 and user_id=\"%2\";").
            arg(room_id).arg(user_id);
    query.exec (sqlString);

    if(!query.next ())//数据库不存在此用户
    {
        return 0.0;
    }
    else
    {
        query.last();
        return query.value("total_power").toDouble();
    }
}

