#ifndef USER_H
#define USER_H
#include <QString>
#include <QVector>
#include <QVariant>
struct User{
    QString name;
    QString account;
    QString password;
    QString path;
    int id;
};
extern QVector<User>userInfo;
void initUser();
#endif // USER_H
