#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QStackedWidget>
#include "user.h"
#include <QTcpServer>
#include <QTcpServer>
#include <QCloseEvent>
#include "messagesend.h"
#include <QList>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0,QString account=" ",QString path=" ",int id=0,QString name="",QVector<User>user={});
    ~Widget();
    void initui();
    void paintEvent(QPaintEvent *event);
    void initMessage(QWidget *widget);
    void initContact(QWidget *widget);
    void initFont();
    void initUser();
    void initChat(QWidget *chatWidget);//聊天界面实现
    void closeEvent(QCloseEvent *event);
    void createOrshowChatUi(const User&sender,const QString &message,const QString &forward);
    QPixmap changeImage(const QString &path,int d);
    QStackedWidget *stack;
    QWidget *messageWid;
    QWidget *contactWid;
    QFont font;
    QString userAccount;
    QString userPath;
    int userId;
    QString userName;
    User onlineUser;
    QVector<User>userTab;
    QMap<QString,bool>chatUi;
    QList<MessageSend*>openChatWindow;

private:
private slots:
    void showMessage();
    void showContact();
    void initDialog(User user_A,User user_B);
public slots:
    void forwardMessageToserver(const QString message);
private:
    QTcpSocket *clientSocket;
private slots:
    void connectToServer();
    void readMessage();

};

#endif // WIDGET_H
