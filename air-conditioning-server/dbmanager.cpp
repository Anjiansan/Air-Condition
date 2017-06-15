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

//    weeks.append("周日"); //初始化
//    weeks.append("周一");
//    weeks.append("周二");
//    weeks.append("周三");
//    weeks.append("周四");
//    weeks.append("周五");
//    weeks.append("周六");
}

bool DBManager::createTable()   //创建表
{
    QSqlQuery query;
    QString sqlString=QString("create table if not exists airCondition("
                        "room_id int,"
                        "user_id char(32),"
                        "switch_num int,"
                        "year int,"
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
                        "total_power float(6,2),"
                        "isOneReq int);");

    return query.exec (sqlString);
}

bool DBManager::insertData(DBData data)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString year=time.toString("yyyy");
    QString month = time.toString("M");
    int week = time.date().weekNumber();
    QString day = time.toString("d");

    QSqlQuery query;
    QString sqlString=QObject::tr("insert into airCondition values(%1,\"%2\",%3,%4,%5,%6,%7,\"%8\",\"%9\",%10,%11,%12,%13,%14,%15,%16,%17);").
            arg (data.room_id).arg (data.user_id).arg (data.switch_num).arg (year).arg (month).arg (week).arg (day).
            arg (data.start_time).arg (data.end_time).arg (data.start_temp).arg (data.end_temp).
            arg (data.speed).arg (data.fee).arg (data.total_fee).arg (data.power).
            arg (data.total_power).arg (data.isOneReq);

    qDebug()<<sqlString;
    return query.exec (sqlString);
}

int DBManager::getSwitchNum(int room_id, QString user_id)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select max(switch_num) from airCondition where room_id=%1 and user_id=\"%2\";").
            arg(room_id).arg(user_id);
    query.exec (sqlString);

    if(!query.next ())//数据库不存在此用户
    {
        return 0;
    }
    else
    {
        return query.value(0).toInt();
    }
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

bool DBManager::genDayReport(int room_id, int year, int month, int day)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select * from airCondition where room_id=%1 and day=%2 and year=%3 and month=%4 and isOneReq=1;").
            arg(room_id).arg (day).arg (year).arg (month);
    query.exec (sqlString);

    std::ofstream out("日报表.txt");
    double totalFee=0;
    out<<std::setw(15)<<"房间号\t"<<std::setw(15)<<"开关机的次数\t"<<std::setw(28)<<"温控请求起止时间\t"
      <<"温控请求起止温度\t"<<"风量大小\t"<<"温控请求所需费用"<<std::endl;
    while(query.next())
    {
        out<<std::setw(10)<<query.value(0).toString().toStdString()<<"\t"<<std::setw(10)
          <<query.value(2).toString().toStdString()<<"\t"<<query.value(7).toString().toStdString()
          <<"~"<<query.value(8).toString().toStdString()<<"\t"<<std::setw(10)<<query.value(9).toString().toStdString()
          <<"~"<<query.value(10).toString().toStdString()<<"\t"<<std::setw(6)<<query.value(11).toString().toStdString()
          <<"\t"<<std::setw(12)<<query.value(12).toString().toStdString()<<std::endl;
        totalFee+=query.value(12).toDouble();
    }
    out<<std::endl<<"总费用:"<<totalFee;

    out.close();

    return true;
}

bool DBManager::genWeekReport(int room_id, int year, int week)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select * from airCondition where room_id=%1 and week=%2 and year=%3 and isOneReq=1;").
            arg(room_id).arg (week).arg (year);
    query.exec (sqlString);

    std::ofstream out("周报表.txt");
    double totalFee=0;
    out<<std::setw(15)<<"房间号\t"<<std::setw(15)<<"开关机的次数\t"<<std::setw(28)<<"温控请求起止时间\t"
      <<"温控请求起止温度\t"<<"风量大小\t"<<"温控请求所需费用"<<std::endl;
    while(query.next())
    {
        out<<std::setw(10)<<query.value(0).toString().toStdString()<<"\t"<<std::setw(10)
          <<query.value(2).toString().toStdString()<<"\t"<<query.value(7).toString().toStdString()
          <<"~"<<query.value(8).toString().toStdString()<<"\t"<<std::setw(10)<<query.value(9).toString().toStdString()
          <<"~"<<query.value(10).toString().toStdString()<<"\t"<<std::setw(6)<<query.value(11).toString().toStdString()
          <<"\t"<<std::setw(12)<<query.value(12).toString().toStdString()<<std::endl;
        totalFee+=query.value(12).toDouble();
    }
    out<<std::endl<<"总费用:"<<totalFee;

    out.close();

    return true;
}

bool DBManager::genMonthReport(int room_id, int year, int month)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select * from airCondition where room_id=%1 and month=%2 and year=%3 and isOneReq=1;").
            arg(room_id).arg (month).arg (year);
    query.exec (sqlString);

    std::ofstream out("月报表.txt");
    double totalFee=0;
    out<<std::setw(15)<<"房间号\t"<<std::setw(15)<<"开关机的次数\t"<<std::setw(28)<<"温控请求起止时间\t"
      <<"温控请求起止温度\t"<<"风量大小\t"<<"温控请求所需费用"<<std::endl;
    while(query.next())
    {
        out<<std::setw(10)<<query.value(0).toString().toStdString()<<"\t"<<std::setw(10)
          <<query.value(2).toString().toStdString()<<"\t"<<query.value(7).toString().toStdString()
          <<"~"<<query.value(8).toString().toStdString()<<"\t"<<std::setw(10)<<query.value(9).toString().toStdString()
          <<"~"<<query.value(10).toString().toStdString()<<"\t"<<std::setw(6)<<query.value(11).toString().toStdString()
          <<"\t"<<std::setw(12)<<query.value(12).toString().toStdString()<<std::endl;
        totalFee+=query.value(12).toDouble();
    }
    out<<std::endl<<"总费用:"<<totalFee;

    out.close();

    return true;
}

void DBManager::updateSwitchNum(int room_id, QString user_id, int switch_num)
{
    QSqlQuery query;
    QString sqlString=QObject::tr("select * from airCondition where room_id=%1 and user_id=\"%2\";").
            arg(room_id).arg(user_id);
    query.exec (sqlString);

    if(query.next () && query.value("switch_num").toInt()!=switch_num)
    {
        query.last();   //更新最后一个记录
        QString sqlString=QObject::tr("update airCondition set switch_num=%1 where room_id=%2 and "
                                      "user_id=\"%3\" and total_fee=%4 and total_power=%5").
                arg (switch_num).arg(room_id).arg(user_id).arg (query.value("total_fee").toDouble())
                .arg (query.value("total_power").toDouble());
        query.exec (sqlString);
    }
}

