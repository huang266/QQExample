#include "dialog.h"
#include<QHBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QtSql/QSql>
#include <QtSql>
Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    server=new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
    server->listen(QHostAddress::Any,12345);//监听任意ip和端口
    qDebug()<<"Server is starting! Waiting for Connection";
    //初始化界面
    //setFixedSize(750,750);
    resize(750,750);
    createDateBase();
    initServer();
}

Dialog::~Dialog()
{

}
void Dialog::initServer(){
    //创建并设置表格控件
    userTable=new QTableWidget(this);
    userTable->setColumnCount(3);//设置3列，用户名，id，在线状态
    userTable->setHorizontalHeaderLabels(QStringList()<<"用户名"<<"ID"<<"在线状态");
    //使表头可调整大小
    userTable->horizontalHeader()->setStretchLastSection(true);//
    userTable->verticalHeader()->setVisible(false);//隐藏竖直表头
    userTable->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑
    loadUsersFromDb();
    //布局
    QVBoxLayout *layout=new QVBoxLayout(this);
    layout->addWidget(userTable);
    setLayout(layout);


}
void Dialog::newConnection(){
    QTcpSocket *clientSocket=server->nextPendingConnection();//获取客户端
    //添加信号和槽连接
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(clientSocket,SIGNAL(disconnected()),this,SLOT(disConnection()));
    clients.append(clientSocket);//存储客户端连接
    qDebug()<<"New client connected";

}
void Dialog::readMessage(){
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = senderSocket->readAll();
    QString message(data);
    qDebug() << "Received message:" << message;
    //发送信息给客户端:文字
    if(message.startsWith("SendMessage:")){
        QStringList parts=message.split(":");
        if(parts.size()==4){
            QString senderUid=parts[1];
            QString receivedUid=parts[2];
            QString content=parts[3];
            //找到目标客户端并转发信息
            qDebug()<<"It is a message";
            for(QTcpSocket* client:clients){
                //发送给指定客户端
                if(ClientUserMap.value(client)==receivedUid){
                    QString forwardMessage=QString("NewMessage:%1:%2")
                            .arg(senderUid)
                            .arg(content);
                    client->write(forwardMessage.toUtf8());
                    break;
                }
            }
        }
    }else if(message.startsWith("SendImage")){
        //发送信息给客户端:图片
        qDebug()<<"It is s image";
        QStringList parts=message.split("+");
        if(parts.size()==4){
            QString senderUid=parts[1];
            QString receiveUid=parts[2];
            QString content=parts[3];
            //找到目标客户端并转发
            for(QTcpSocket* client:clients){
                if(ClientUserMap.value(client)==receiveUid){
                    QString forwardMessage=QString("NewImage+%1+%2")
                            .arg(senderUid)
                            .arg(content);
                    client->write(forwardMessage.toUtf8());
                }
            }
        }

    }else if(message.startsWith("SendFile")){
        //发送信息给客户端：文件
        qDebug()<<"It is a file";
        QStringList parts=message.split("+");
        if(parts.size()==4){
            QString senderUid=parts[1];
            QString receiveUid=parts[2];
            QString content=parts[3];
            //找到目标客户端并转发
            for(QTcpSocket* client:clients){
                if(ClientUserMap.value(client)==receiveUid){
                    QString forwardMessage=QString("NewFile+%1+%2")
                            .arg(senderUid)
                            .arg(content);
                    client->write(forwardMessage.toUtf8());
                }
            }
        }

    }
    //假设消息格式为 "Connection username uid"
    QStringList messageParts = message.split(" ");
    if (messageParts.size() == 3 && messageParts[0] == "Connection") {
        QString name = messageParts[1];
        QString uid = messageParts[2];
        //存储客户端与uid的映射
        ClientUserMap[senderSocket]=uid;
        // 更新表格内容
        updateOnlineStatus(uid,"在线");
        qDebug() << "Added user to table:" << name << uid;
    }
}
void Dialog::disConnection(){
    QTcpSocket *senderSocket=qobject_cast<QTcpSocket*>(sender());//获取断开的客户端
    //更新客户端信息
    if(ClientUserMap.contains(senderSocket)){
        QString uid=ClientUserMap.value(senderSocket);
        updateOnlineStatus(uid,"离线");
        ClientUserMap.remove(senderSocket);
    }
    clients.removeAll(senderSocket);
    senderSocket->deleteLater();//删除socket；
    qDebug()<<"A Client disconnected!";
}
void Dialog::loadUsersFromDb(){
    //从数据库中加入信息
    QSqlQuery query;
    query.exec("SELECT * from USERTable");
    while(query.next()){
        User u;
        u.name=query.value(0).toString();
        u.id=query.value(1).toString();
        u.onlineStatus="离线";
        user.append(u);
        //向表格中添加用户信息
        int row=userTable->rowCount();
        userTable->insertRow(row);
        userTable->setItem(row,0,new QTableWidgetItem(u.name));
        userTable->setItem(row,1,new QTableWidgetItem(u.id));
        userTable->setItem(row,2,new QTableWidgetItem(u.onlineStatus));

    }
}
void Dialog::updateOnlineStatus(const QString account, const QString status){
    for(int i=0;i<user.size();i++){
        if(user[i].id==account){
            //更新在线状态
            user[i].onlineStatus=status;
            userTable->item(i,2)->setText(status);
            //根据在线状态改变字体颜色
            if(status=="在线"){
                userTable->item(i,2)->setForeground(QBrush(Qt::green));
            }else{
                userTable->item(i,2)->setForeground(QBrush(Qt::black));
            }
            break;
        }
    }
}
void Dialog::createDateBase() {
    // 数据库操作
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("D:\\QtProject\\build-QQexample-Desktop_Qt_5_12_1_MinGW_64_bit-Debug\\DataBaseInfo.db");
    if (DB.open()) {
        qDebug() << "Successfully connected to the database.";
      }else {
        qDebug() << "无法打开数据库";
    }
}
