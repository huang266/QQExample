#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtDebug>
#include <QList>
#include <QMap>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QMap>
class Dialog : public QDialog
{
    Q_OBJECT
struct User{
        QString name;
        QString id;
        QString onlineStatus;
    };

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    QTcpServer *server;
    QList<QTcpSocket*>clients;
    QTableWidget *userTable;
    void initServer();
    void loadUsersFromDb();
    void updateOnlineStatus(const QString account,const QString status);
    QSqlDatabase db;
    QSqlDatabase DB;
    QVector<User>user;
    QMap<QTcpSocket*,QString>ClientUserMap;
    void createDateBase();
private slots:
    void newConnection();
    void readMessage();
    void disConnection();
};

#endif // DIALOG_H
