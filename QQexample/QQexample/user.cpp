#include "user.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
QVector<User>userInfo;
#include <QtDebug>
void initUser(){
    QSqlDatabase db=QSqlDatabase::database();
    QSqlQuery query;
    QString q="SELECT * From USERTable";
    query.exec(q);
    while(query.next()){
        User user;
        user.name=query.value(0).toString();
        user.account=query.value(1).toString();
        user.password=query.value(2).toString();
        user.path=query.value(3).toString();
        user.id=query.value(4).toInt();
        userInfo.append(user);
    }
    qDebug()<<"初始化了userInfo";

}
