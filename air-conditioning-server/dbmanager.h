#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <fstream>
#include <iostream>
#include <string>
#include<iomanip>
class DBData;
#include "clientconn.h"

class DBManager
{
public:
    DBManager();

    bool insertData(DBData data);   //插入

    int getSwitchNum(int room_id,QString user_id);    //查询总费用

    double getTotalFee(int room_id,QString user_id);    //查询总费用

    double getTotalPower(int room_id,QString user_id);    //查询总功率

    bool genDayReport(int room_id,int day);

    bool genWeekReport(int room_id,int week);

    bool genMonthReport(int room_id,int month);

    void updateSwitchNum(int room_id,QString user_id,int switch_num);

private:
    bool createTable();

private:
    QSqlDatabase *db;
//    QVector<QString> weeks; //周转换表
};

#endif // DBMANAGER_H
