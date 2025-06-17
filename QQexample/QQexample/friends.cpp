#include "friends.h"
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
int friendShip[Max_user][Max_user];
void initFriend(const QString& xmlPath){
    for(int i=0;i<Max_user;i++){
        for(int j=0;j<Max_user;j++){
            friendShip[i][j]=0;
            friendShip[j][i]=0;
        }
    }
    //从数据库中获取好友信息
    QSqlDatabase db=QSqlDatabase::database();
    //db.setDatabaseName("USERTable");
    if(db.open()){
        qDebug()<<"数据库连接成功!!";
        QSqlQuery query;
        QString question="select * from USERTable";
        query.exec(question);
        while(query.next()){
            int id=query.value(4).toInt();
            QString list_friend=query.value(5).toString();
            if(!list_friend.isEmpty()){
                QStringList friendlist=list_friend.split("+");
                for(const QString &f:friendlist){
                    int f_id=f.toInt();
                    setFriendShip(id,f_id,true);
                }
            }

        }
    }else{
        qDebug()<<"数据库连接失败!!";
    }


}
void setFriendShip(int id1,int id2,bool areFriends){
      friendShip[id1][id2]=areFriends==true? 1:0;
      friendShip[id2][id1]=areFriends==true? 1:0;
}

void setNewFriendShip(int id1,int id2,bool areFriends){
    QSqlDatabase db=QSqlDatabase::database();
    if(db.isOpen()){
        qDebug()<<"数据库打开成功in setnewFriendShip函数";
        QSqlQuery query1;
        QSqlQuery query2;
        QString id11=QString::number(id1);
        QString id22=QString::number(id2);
        //查询两个用户的好友信息
        QString question1="select * from USERTable where id="+id11;
        QString question2="select * from USERTable where id="+id22;

        //对好友1进行添加或删除朋友
        query1.exec(question1);
        QStringList friend1;
        while(query1.next()){
            QString list_friend=query1.value(5).toString();
            friend1=list_friend.split("+");
        }
        bool found1=friend1.isEmpty();
        if(found1){
            qDebug()<<"未查找到用户ID";
        }else{
            if(areFriends){
                if(!friend1.contains(id22)){
                    friend1.append(id22);
                }else{qDebug()<<"用户已添加";}
            }else{
                friend1.removeAll(id22);
            }
        }

        //对好友2进行添加或删除朋友
        query2.exec(question2);
        QStringList friend2;
        while(query2.next()){
            QString list_friend=query2.value(5).toString();
            friend2=list_friend.split("+");
        }
        bool found2=friend2.isEmpty();
        if(found2){
            qDebug()<<"未查找到用户";
        }else{
            if(areFriends){
                if(!friend2.contains(id11)){
                    friend2.append(id11);
                }else{qDebug()<<"用户已添加";}
            }else{
                friend2.removeAll(id11);
            }
        }


    }else{
        qDebug()<<"数据库打开失败in setnewFriendShip函数";
    }
}
//void reSetFriendShip(int id1,int id2,const QString& xmlPath,bool areFriends){
//    //读取xml文件
//    QFile file(xmlPath);
//    if(!file.open(QIODevice::ReadOnly)){
//        qDebug()<<"failed to open xml file: "<<xmlPath;
//        return;
//    }
//    QDomDocument doc;
//    if(!doc.setContent(&file)){
//        qDebug()<<"Fauled to parse XML file: "<<xmlPath;
//        file.close();
//    }
//    file.close();
//    //对xml文件节点进行操作
//    QDomElement root=doc.firstChildElement("Users");
//    QDomNodeList userList=root.elementsByTagName("User");
//    bool userFound1=false;
//    bool userFound2=false;
//    for(int i=0;i<userList.count();i++){
//        QDomNode userNode=userList.at(i);
//        QDomElement userElement=userNode.toElement();
//        //获取用户id；
//        int userid=userElement.firstChildElement("Id").text().toInt();
//        //查找目标用户1
//        if(userid==id1){
//            userFound1=true;
//            //获得当前好友列表
//            QString friends=userElement.firstChildElement("Friend").text();
//            QStringList friendls=friends.split("+");
//            if(areFriends){
//                //添加好友
//                if(!friendls.contains(QString::number(id2))){
//                   friendls.append(QString::number(id2));
//                }
//            }else{
//                friendls.removeAll(QString::number(id2));
//            }
//            //生成新的好友列表并更新
//           QString newFriend=friendls.join("+");
//           userElement.firstChildElement("Friend").firstChild().setNodeValue(newFriend);
//        }
//        //查找目标用户2
//        if(userid==id2){
//            userFound1=true;
//            //获得当前好友列表
//            QString friends=userElement.firstChildElement("Friend").text();
//            QStringList friendls=friends.split("+");
//            if(areFriends){
//                //添加好友
//                if(!friendls.contains(QString::number(id1))){
//                   friendls.append(QString::number(id1));
//                }
//            }else{
//                friendls.removeAll(QString::number(id1));
//            }
//            //生成新的好友列表并更新
//           QString newFriend=friendls.join("+");
//           userElement.firstChildElement("Friend").firstChild().setNodeValue(newFriend);
//        }
//        if(!userFound1||!userFound2){
//            qDebug()<<"未查找到用户id";
//        }
//        //将更新后的内容写回xml文件
//        if(!file.open(QIODevice::WriteOnly|QIODevice::Text)){
//            qDebug()<<"failed to open xml Fule:"<<xmlPath;
//            return;
//        }
//        QTextStream out(&file);
//        out<<doc.toString();
//        file.close();
//        qDebug()<<"Friendship update Successfully";
//    }
//}
