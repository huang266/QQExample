#include "widget.h"
#include <QIcon>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPainter>
#include <QFont>
#include <QDebug>
#include <QTextEdit>
#include <QObject>
#include <QTcpSocket>
#include "messagesend.h"
#include "friends.h"
#include "onlinestatus.h"
Widget::Widget(QWidget *parent,QString account,QString path,int id,QString name,QVector<User>user)
    : QWidget(parent)
{
    userPath=path;
    userAccount=account;
    QVector<User>u1=user;
    userTab=userInfo;
    userId=id;
    userName=name;

    initUser();
    initFont();
    initui();
    //初始化客户端
    clientSocket=new QTcpSocket(this);
    connectToServer();
}
Widget::~Widget()
{
}
void Widget::initFont(){
    font.setFamily("宋体");
    font.setPixelSize(40);
    font.setBold(true);
}
void Widget::initui(){
    resize(600,1200);
    QString title="QQ "+userName;
    this->setWindowTitle(title);
    this->setWindowIcon(QIcon("..\\QQphoto\\qq.jpg"));

    //初始化第一行内容
    QHBoxLayout *lay1=new QHBoxLayout;
    //第一行控件
    QPushButton *labBtn=new QPushButton;
    QPixmap imagePix=changeImage(userPath,200);
    labBtn->setIcon(imagePix);
    labBtn->setIconSize(QSize(200,200));
    labBtn->setStyleSheet("QPushButton{"
                          "border-radius:100px;"
                          "background-repeat:no-repeat;"
                          "border:none;"
                          "border: 3px solid rgba(255, 255, 255, 0.8); "
                          "}");
    labBtn->setFixedSize(200,200);

    QPushButton *btn1=new QPushButton;
    btn1->setIcon(QIcon("..\\QQphoto\\ss.jpg"));
    btn1->setIconSize(QSize(70,70));

    QPushButton *btn2=new QPushButton;
    btn2->setIcon(QIcon("..\\QQphoto\\tj.jpg"));
    btn2->setIconSize(QSize(70,70));
    QLineEdit *edit=new QLineEdit;
    edit->setFixedHeight(70);
    //布局
    lay1->setSpacing(20);
    lay1->addWidget(labBtn);
    lay1->addWidget(btn1);
    lay1->addWidget(edit);
    lay1->addWidget(btn2);

    QVBoxLayout *vlay=new QVBoxLayout(this);
    vlay->addLayout(lay1);
    //vlay->addStretch();
    //主体部分的初始化
    QHBoxLayout *hlay=new QHBoxLayout;
    vlay->addLayout(hlay);
    //主体部分的三个模块
    //模块一：设置部分
    QPushButton *btn3=new QPushButton;
    btn3->setText("消息列表");
    QPushButton *btn4=new QPushButton;
    btn4->setText("联系人");
    QPushButton *btn5=new QPushButton;
    btn5->setText("设置");

    QVBoxLayout *lay_model1=new QVBoxLayout;
    lay_model1->addWidget(btn3);
    lay_model1->addWidget(btn4);
    lay_model1->addStretch();
    lay_model1->addWidget(btn5);
    hlay->setSpacing(20);
    hlay->addLayout(lay_model1);
    //创建联系人和消息列表界面
    stack=new QStackedWidget(this);

    messageWid=new QWidget;
    contactWid=new QWidget;
    messageWid->setStyleSheet("border:5px solid pink");
    messageWid->setFixedSize(QSize(380,1140));
    contactWid->setStyleSheet("border:5px solid pink");
    contactWid->setFixedSize(QSize(380,1140));

    //两个界面的ui
    initMessage(messageWid);
    initContact(contactWid);
    //将界面添加至stack中
    stack->addWidget(messageWid);
    stack->addWidget(contactWid);

    //绑定槽函数
    connect(btn3,SIGNAL(clicked()),this,SLOT(showMessage()));
    connect(btn4,SIGNAL(clicked()),this,SLOT(showContact()));
    hlay->addWidget(stack);
}
//绘制背景
void Widget::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap background("..\\QQphoto\\2.png");
    // 拖放整个图片来填充背景
    painter.drawPixmap(rect(), background);
    QWidget::paintEvent(event); // 调用基类的 paintEvent
}
void Widget::initMessage(QWidget *widget){
    QVBoxLayout *messageLay=new QVBoxLayout(widget);
    messageLay->setMargin(0);
    QLabel *titleLab=new QLabel("消息列表");
    titleLab->setStyleSheet("color: pink");
    titleLab->setAlignment(Qt::AlignCenter);
    titleLab->setFont(font);
    titleLab->setFixedSize(380,50);
    messageLay->addWidget(titleLab);
    messageLay->addStretch();

}
void Widget::initContact(QWidget *widget) {
    QVBoxLayout *contactLay = new QVBoxLayout(widget);
    contactLay->setMargin(0);
    QLabel *titleLab = new QLabel("联系人");
    titleLab->setStyleSheet("color: pink");
    titleLab->setAlignment(Qt::AlignCenter);
    titleLab->setFont(font);
    titleLab->setFixedSize(380, 50);
    contactLay->addWidget(titleLab);

    for (int i = 1; i < userTab.size(); i++) {
        if (friendShip[userId][i] == 1&&i!=userId) {
            // 初始化好友信息
            QPushButton *friendBtn = new QPushButton;
            friendBtn->setFixedHeight(140);
            friendBtn->setStyleSheet("QPushButton{"
                                       "border-radius:100px;"
                                       "background-repeat:no-repeat;"
                                       "border:none;"
                                       "border: 3px solid rgba(128,0,128,0); "
                                       "}");
            friendBtn->setFixedSize(140, 140);
            QPixmap imagePix = changeImage(userTab[i-1].path, 140);
            friendBtn->setIcon(imagePix);
            friendBtn->setIconSize(QSize(140, 140));

            // 布局
            QHBoxLayout *friendlay = new QHBoxLayout;
            QLabel *friendName = new QLabel(userTab[i-1].name);
            friendlay->setMargin(0);
            friendlay->setSpacing(0);
            friendlay->addWidget(friendBtn);
            friendlay->addWidget(friendName);
            friendlay->setAlignment(Qt::AlignLeft);

            QWidget *con = new QWidget;
            con->setLayout(friendlay);
            contactLay->addWidget(con);

            // 为好友按钮添加点击事件

            connect(friendBtn, &QPushButton::clicked, this, [this, i]() {
                initDialog(userTab[i-1], onlineUser);
            });
        }
    }
    contactLay->addStretch();
}
void Widget::showMessage(){
    stack->setCurrentIndex(0);
}
void Widget::showContact(){
    stack->setCurrentIndex(1);
}
QPixmap Widget::changeImage(const QString &path, int d){
    QPixmap pixmap(path);
    //创建一个透明的pixmap
    QPixmap cirPix(d,d);
    cirPix.fill(Qt::transparent);//透明填充

    QPainter painter(&cirPix);
    painter.setRenderHint(QPainter::Antialiasing);//设置抗锯齿

    //创建一个椭圆形路径并设置为裁剪区域
    QPainterPath path1;
    path1.addEllipse(0,0,d,d);
    painter.setClipPath(path1);

    //绘制原始图像
    painter.drawPixmap(0,0,pixmap.scaled(d,d,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    painter.end();

    return cirPix;

}

void Widget::initDialog(User user_A,User user_B){
    chatUi[user_A.account]=true;
    qDebug()<<onlineUser.name+"已创建和"+user_A.account<<"的聊天窗口";
    MessageSend* send=new MessageSend(nullptr,user_A,user_B);
    openChatWindow.append(send);
    send->setAttribute(Qt::WA_DeleteOnClose);
    connect(send,&MessageSend::sendMessageToWidget,this,&Widget::forwardMessageToserver);
    send->show();

}
void Widget::forwardMessageToserver(const QString message) {
    //获取对话框界面的内容。
    if(clientSocket->state() == QAbstractSocket::ConnectedState) {
        clientSocket->write(message.toUtf8());
    } else {
        qDebug() << "服务器未连接";
    }
}
void Widget::initUser(){
    onlineUser.name=userName;
    onlineUser.account=userAccount;
    onlineUser.path=userPath;
    onlineUser.id=userId;
    onlineUser.password="123456";
}
void Widget::connectToServer(){
    //连接到服务器
    clientSocket->connectToHost("127.0.0.1",12345);
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    if(clientSocket->waitForConnected()){
        QString name=onlineUser.name;
        QString uid=onlineUser.account;
        QString message="Connection "+name+" "+uid;
        clientSocket->write(message.toUtf8());//发送客户端信息
        qDebug()<<"连接服务器成功!";

    }else{
        qDebug()<<"连接服务器失败! ";
    }
}
void Widget::closeEvent(QCloseEvent *event){
    //设置离线状态
    onLine[onlineUser.account]=false;
    if(clientSocket->state()==QAbstractSocket::ConnectedState){
        clientSocket->disconnectFromHost();//与服务器断开连接
    }if(clientSocket->state()==QAbstractSocket::ConnectedState){
        clientSocket->waitForDisconnected();//等待断开连接
        qDebug()<<"与服务器断开连接!";
    }
    event->accept();
}
void Widget::readMessage(){
    User userSender;
    QString content;
    //读取从服务器接收到的数据
    QByteArray data=clientSocket->readAll();
    QString message=QString::fromUtf8(data);
    qDebug()<<onlineUser.name+" "<<"Received message from server:"<<message;
    //处理信息
    if(message.startsWith("NewMessage")){
        QStringList parts=message.split(":");
        if(parts.size()==3){
            QString senderUid=parts[1];
             content=parts[2];
            //查找发送者信息
            for(const User&u:userInfo){
                if(u.account==senderUid){
                    userSender=u;
                    break;
                }
            }
           createOrshowChatUi(userSender,content,"NewMessage");
        }

    }else if(message.startsWith("NewImage")){
        QStringList parts=message.split("+");
        if(parts.size()==3){
            QString senderUid=parts[1];
             content=parts[2];
            //查找发送者信息
            for(const User&u:userInfo){
                if(u.account==senderUid){
                    userSender=u;
                    break;
                }
            }
           createOrshowChatUi(userSender,content,"NewImage");
        }

    }else if(message.startsWith("NewFile")){
        QStringList parts=message.split("+");
        if(parts.size()==3){
            QString senderUid=parts[1];
             content=parts[2];
            //查找发送者信息
            for(const User&u:userInfo){
                if(u.account==senderUid){
                    userSender=u;
                    break;
                }
            }
           createOrshowChatUi(userSender,content,"NewFile");
        }
    }
}
void Widget::createOrshowChatUi(const User &sender, const QString &message,const QString  &forward) {
    // 如果对话框已经存在
    qDebug()<<sender.account;
    if (chatUi.contains(sender.account) && chatUi[sender.account] == true) {
        // 遍历已经打开的对话框，找到对应的窗口
        for (MessageSend* window : openChatWindow) {
            qDebug()<<"已存在窗口的id："+window->receiveR.account;
            if (window->receiveR.account == sender.account) {
                window->onMessageReceived(message,forward); //调用对应对话框的消息处理函数
                qDebug() << "窗口存在，消息已转发";
                return; // 消息已处理，退出
            }
        }
    }

    // 如果对话框不存在，则创建新窗口
//    MessageSend *messageSend = new MessageSend(nullptr, sender, onlineUser);
//    chatUi[sender.account] = true; // 确保标记更新
//    openChatWindow.append(messageSend);
//    messageSend->show();
//    messageSend->addMessage(message, false); // 更新消息显示
    qDebug() << "不存在窗口，已创建新窗口";
}
