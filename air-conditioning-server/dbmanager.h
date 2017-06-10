#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <fstream>
#include <iostream>
class DBData;
#include "clientconn.h"

class DBManager
{
public:
    DBManager();

    bool insertData(DBData data);   //插入

    double getTotalFee(int room_id,QString user_id);    //查询总费用

    double getTotalPower(int room_id,QString user_id);    //查询总功率

    bool genDayReport(int room_id);

private:
    bool createTable();

private:
    QSqlDatabase *db;
    QVector<QString> weeks; //周转换表
};

#endif // DBMANAGER_H
