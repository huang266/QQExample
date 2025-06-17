#include "widget.h"
#include "dialog.h"
#include "friends.h"
#include "user.h"
#include "registr.h"
#include "onlinestatus.h"
#include <QApplication>
#include "messagesend.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<User>("User");//注册user类信息
    qRegisterMetaType<QVector<User>>("QVector<User>");
    Dialog dia;
    dia.show();
    initFriend("D:\\QtProject\\Xmls\\user.xml");
    initOnlineStatus();

//      MessageSend *m=new MessageSend();
//      m->show();

    return a.exec();
}
