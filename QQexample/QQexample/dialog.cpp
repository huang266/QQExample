#include "dialog.h"
#include "ui_dialog.h"
#include <QPainter>
#include "widget.h"
#include <QVector>
#include "widgetthread.h"
#include <QMessageBox>
#include "user.h"
#include <QPixmap>
#include <QPainterPath>
#include "onlinestatus.h"
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    initui();
    this->setWindowTitle("登录");
    this->setWindowIcon(QIcon("..\\QQphoto\\qq.jpg"));
    this->setFixedSize(QSize(747,1036));
    createDateBase();
    userTab=loadUsersFromDb();
    initUser();//加载用户信息
    if(userInfo.isEmpty()){
        qDebug()<<"初始化失败";
    }
    query();
}
Dialog::~Dialog()
{
    //停止所有线程
    QList<QThread*>threads=findChildren<QThread*>();
    for(QThread *thread:threads){
        thread->quit();
        thread->wait();
        delete thread;
    }
    delete ui;
}
void Dialog::initui(){
    ui->logBtn->setEnabled(false);
    ui->passwordLine->setEchoMode(QLineEdit::Password);


}
void Dialog::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPixmap background("..\\QQphoto\\2.png");
    // 拖放整个图片来填充背景
    painter.drawPixmap(rect(), background);
    QDialog::paintEvent(event); // 调用基类的 paintEvent
}

void Dialog::on_checkBox_stateChanged(int arg1)
{

    if(ui->checkBox->isChecked()){
        ui->logBtn->setEnabled(true);
    }
    else{
         ui->logBtn->setEnabled(false);
    }
}

void Dialog::on_logBtn_clicked()
{
    //登录逻辑：判断密码->判断是否已经登录。
    QString nowPass=ui->passwordLine->text();
    if(nowPass!=User_password){
        qDebug()<<"登录失败：密码错误";
        QMessageBox::warning(this,"登录失败","密码错误");
    }
    else{
        if(onLine[User_account]==false){
            onLine[User_account]=true;
            //开启线程
            WidgetThread *thread = new WidgetThread(this, ui->accountLine->text(), User_path, User_id, User_name, userTab, this);
            thread->start();
        }
        else{
            qDebug()<<"登录失败:该账号已登录";
            QMessageBox::warning(this,"登录失败","该账号已登录");
        }
    }
}
void Dialog::createDateBase() {
    // 数据库操作
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("DataBaseInfo.db");
    if (DB.open()) {
        qDebug() << "Successfully connected to the database.";

        QSqlQuery checkTable;
        checkTable.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='USERTable';");

        // 检查表是否存在
        if (!checkTable.next()) {
            // 表不存在，创建新表
            QSqlQuery createTab;
            const QString createTableQuery = R"(
                CREATE TABLE USERTable (
                    name TEXT,
                    account TEXT,
                    password TEXT,
                    Path TEXT,
                    id INTEGER
                );)";
            if (createTab.exec(createTableQuery)) {
                qDebug() << "Table USERS created successfully.";

                // 从 XML 加载数据
                for (const User &user : userTab) {
                    createTab.prepare("INSERT INTO USERTable(name, account, password, path, id) VALUES(?,?,?,?,?);");
                    createTab.addBindValue(user.name);       // name
                    createTab.addBindValue(user.account);    // account
                    createTab.addBindValue(user.password);   // password
                    createTab.addBindValue(user.path);       // path
                    createTab.addBindValue(user.id);         // id

                    if (!createTab.exec()) {
                        qDebug() << "Insert error:" << createTab.lastError().text();  // 添加错误输出
                    }
                }
            } else {
                qDebug() << "Create table error:" << createTab.lastError().text();
            }
        } else {
            qDebug() << "Table USERS already exists.";

        }
    } else {
        qDebug() << "无法打开数据库";
    }
}
void Dialog::query(){

    QSqlQuery query;
    ui->comboBox->clear();
    query.exec("SELECT * From USERTable where name='霜降'");
    while(query.next()){
         User_name=query.value(0).toString();
         User_account=query.value(1).toString();
         User_password=query.value(2).toString();
         User_path=query.value(3).toString();
         User_id=query.value(4).toInt();
         QString friendlist=query.value(5).toString();
       //  qDebug()<<friendlist;
    }
    QString p;
    for(const User &user:userTab){
        if(user.name=="霜降"){
            p=user.path;
            break;
        }
    }
    QPixmap imagePix=changeImage(p,200);
    ui->photoBtn->setIcon(QIcon(imagePix));
    ui->photoBtn->setIconSize(QSize(200,200));
    changeStyle();
    //向comBoBox添加数据
    for(User &user:userTab){
        ui->comboBox->addItem(user.account);
    }
    ui->accountLine->setText( ui->comboBox->currentText());

}
void Dialog::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString selectedAccount = ui->comboBox->currentText();
    ui->accountLine->setText(selectedAccount);
    QSqlQuery query;
    // 使用参数绑定来避免 SQL 注入和拼接错误
    query.prepare("SELECT * FROM USERTable WHERE account = :account");
    query.bindValue(":account", selectedAccount);

    if (query.exec()) {
        if (query.next()) {
            User_name = query.value(0).toString();
            User_account = query.value(1).toString();
            User_password = query.value(2).toString();
            User_path = query.value(3).toString();
            User_id = query.value(4).toInt();
            qDebug() << "User path:" << User_path << "User name:" << User_name;

            // 更新头像
            QPixmap imagePix=changeImage(User_path,200);
            ui->photoBtn->setIcon(QIcon(imagePix));
            ui->photoBtn->setIconSize(QSize(200, 200));
            changeStyle();
        }
    } else {
        qDebug() << "Query error:" << query.lastError().text();
    }
}
QVector<User> Dialog::loadUsersFromXml(const QString &filePath) {
    QVector<User> users;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件";
        return users;
    }
    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "User") {
            User user;
            while (!(xml.isEndElement() && xml.name() == "User")) {
                xml.readNext(); // 确保在读取下一个元素
                if (xml.isStartElement()) {
                    if (xml.name() == "Name") {
                        user.name = xml.readElementText();
                        qDebug() << "读取到名字:" << user.name;
                    } else if (xml.name() == "Account") {
                        user.account = xml.readElementText();
                        qDebug() << "读取到账号:" << user.account;  // 打印账号以确认
                    } else if (xml.name() == "Password") {
                        user.password = xml.readElementText();
                        qDebug() << "读取到密码:" << user.password;
                    } else if (xml.name() == "Path") {
                        user.path = xml.readElementText();
                        qDebug() << "读取到路径:" << user.path;
                    } else if (xml.name() == "Id") {
                        user.id = xml.readElementText().toInt();
                        qDebug() << "读取到ID:" << user.id;
                    }
                }
            }
            users.append(user);
        }
    }
    if (xml.hasError()) {
        qDebug() << "XML错误:" << xml.errorString();
    } else if (users.isEmpty()) {
        qDebug() << "未能读取到任何用户数据。请确认 XML 文件内容。";
    }
    file.close();
    return users;
}
QVector<User> Dialog::loadUsersFromDb(){
    QVector<User>users;
    QSqlQuery nowQuery;
    QString q="Select * from USERTable";
    nowQuery.exec(q);
    while(nowQuery.next()){
        User user;
        user.name=nowQuery.value(0).toString();
        user.account=nowQuery.value(1).toString();
        user.password=nowQuery.value(2).toString();
        user.path=nowQuery.value(3).toString();
        user.id=nowQuery.value(4).toInt();
        users.append(user);
    }
    if(!users.isEmpty()){
        qDebug()<<"加载成功";
    }
    return users;
}
void Dialog::createWidgetInGUIThread(const QString& account, const QString& userPath, int userId, const QString& userName, const QVector<User>& userTab) {
    Widget *widget = new Widget(nullptr, account, userPath, userId, userName, userTab);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    qDebug()<<"调用了createWidgetInGUIThread函数";
    widget->show();
}

void Dialog::on_registBtn_clicked()
{
    int id=getMaxId();
    registRation=new Registration(nullptr,id);
    registRation->show();

}
int Dialog::getMaxId(){
    int maxId=-1;
    for(User &u:userTab){
        if(u.id>maxId)maxId=u.id;
    }
    return maxId;
}
void Dialog::changeStyle(){
    ui->photoBtn->setStyleSheet("QPushButton{"
                                "border-radius:100px;"
                                "background-repeat:no-repeat;"
                                "border:none;"
                                "border: 3px solid rgba(255, 255, 255, 0.8); "
                                "}");
    ui->photoBtn->setFixedSize(200,200);
}
QPixmap Dialog::changeImage(const QString &path, int d) {
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
