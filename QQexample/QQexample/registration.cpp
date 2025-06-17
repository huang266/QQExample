#include "registration.h"
#include "ui_registration.h"
#include <QPainter>
#include <QFileDialog>
#include "user.h"
#include <QDebug>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
Registration::Registration(QWidget *parent,int id) :
    QWidget(parent),
    ui(new Ui::Registration)
{
    user_id=id+1;
    ui->setupUi(this);
    this->setWindowTitle("注册");
    this->setWindowIcon(QIcon("..\\QQphoto\\qq.jpg"));
    initUi();
}

Registration::~Registration()
{
    delete ui;
}
void Registration::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap background("..\\QQphoto\\2.png");
    //拖放整个图片来填充背景
    painter.drawPixmap(rect(),background);
    QWidget::paintEvent(event);
}

void Registration::on_chooseBtn_clicked()
{
    //设置打开初始目录
    QString initDir="D:/QtProject/imageSource";
    QString filter="images(*.png *.jpg *.gif);;All Files(*)";//文件过滤器

    //选择文件对话框
    QUrl fileUrl=QFileDialog::getOpenFileUrl(this,"选择图片",QUrl::fromLocalFile(initDir),filter);

    if(!fileUrl.isEmpty()){
        tx_path=fileUrl.toLocalFile();
        qDebug()<<tx_path;
        QPixmap image=imageChange(tx_path,200);
        ui->avatorBtn->setIcon(QIcon(image));
        ui->avatorBtn->setIconSize(QSize(200,200));
    }

}

void Registration::on_registBtn_clicked()
{
     // qDebug()<<"dianji";
     user_name=ui->nameLineEdit->text();
     user_password=ui->passwordLineEdit->text();
     QString rePassword=ui->rePasswordLineEdit->text();
     bool flag=user_name==""||user_password==""||user_password!=rePassword;
     if(!flag){
        user_account=getRandomAccount();
        regSuccessfully=true;
        if(insertIntoDb()){
            QMessageBox::information(this,"information","您的账号是"+user_account);
        }else{
            QMessageBox::warning(this,"Warning","插入数据库出错，请重试");
        }

     }else{
         if(user_name==""){
             QMessageBox::warning(this,"warning","用户名不能为空");
         }
         if(user_password==""){
             QMessageBox::warning(this,"warning","密码不能为空");
         }
         if(user_password!=rePassword){
             QMessageBox::warning(this,"warning","两次输入密码不一致");
         }
     }

}
QString Registration::getRandomAccount(){
    //随机生成账号
    QString account;
    for(int i=0;i<9;i++){
        int digit=QRandomGenerator::global()->bounded(10);
        account.append(QString::number(digit));
    }
    return account;
}
bool Registration::insertIntoDb(){
    //获取数据库连接
    QSqlDatabase db=QSqlDatabase::database();
    if(!db.isOpen()){
        qDebug()<<"数据库未打开";
        return false;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO USERTable(name,account,password,path,id)VALUES(?,?,?,?,?);");
    query.addBindValue(user_name);
    query.addBindValue(user_account);
    query.addBindValue(user_password);
    query.addBindValue(tx_path);
    query.addBindValue(user_id);
    u.id=user_id;
    u.name=user_name;
    u.path=tx_path;
    u.account=user_account;
    u.password=user_password;
    userInfo.append(u);
    for(User &u:userInfo){
        qDebug()<<u.name;
    }


    if(query.exec()){
        qDebug()<<"插入成功";
        return true;
    }else{
        qDebug()<<"插入失败"<<query.lastError().text();
        return false;
    }
}
void Registration::initUi(){
    ui->avatorBtn->setStyleSheet("QPushButton{"
                                "border-radius:100px;"
                                "background-repeat:no-repeat;"
                                "border:none;"
                                "border: 3px solid rgba(255, 255, 255, 0.8); "
                                "}");
    ui->avatorBtn->setFixedSize(200,200);
}
QPixmap Registration::imageChange(const QString &path, int d){
    QPixmap pixmap(path);

    // 创建一个透明的 pixmap 以填充图像
    QPixmap cirPix(d, d);
    cirPix.fill(Qt::transparent); // 填充透明背景

    QPainter painter(&cirPix);
    painter.setRenderHint(QPainter::Antialiasing); // 开启抗锯齿

    // 创建一个椭圆形路径并设置为剪裁区域
    QPainterPath path1;
    path1.addEllipse(0, 0, d, d); // 添加椭圆路径
    painter.setClipPath(path1); // 设置剪裁区域为椭圆

    // 绘制原始图像
    painter.drawPixmap(0, 0, pixmap.scaled(d, d, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    painter.end();
    return cirPix;
}
