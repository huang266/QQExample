#include "messagesend.h"
#include "ui_messagesend.h"
#include <QFileDialog>
#include <QPainter>
#include <QApplication>
#include <QFileInfo>
#include <QDebug>
#include <QFontMetrics>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>
#include "user.h"
#include "widget.h"
MessageDeletge::MessageDeletge(QObject *parent) : QStyledItemDelegate(parent) { }
void MessageDeletge::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariantMap data = index.data(Qt::UserRole + 1).toMap();
    bool isSelf = data["isSelf"].toBool();
    QString type = data["type"].toString();
    QPixmap avatar = data["avatar"].value<QPixmap>();
    QDateTime timeStamp=data["timestamp"].toDateTime();
    //绘制时间戳
    painter->setPen(Qt::gray);
    painter->drawText(
                QRect(option.rect.left(),option.rect.top()-20,option.rect.width(),20),
                Qt::AlignCenter,
                timeStamp.toString("yyyy-MM-dd hh:mm:ss")
                );

    // 绘制头像
    painter->setRenderHint(QPainter::Antialiasing);

    // 增加头像的尺寸
    const int avatarSize = 50; // 设置头像的大小
    QRect avatarRect;
    if (isSelf) {
        avatarRect = QRect(option.rect.right() - avatarSize - 10, option.rect.top() + 5, avatarSize, avatarSize);
    } else {
        avatarRect = QRect(option.rect.left() + 10, option.rect.top() + 5, avatarSize, avatarSize);
    }

    // 绘制头像
    painter->drawPixmap(avatarRect, avatar.scaled(avatarSize, avatarSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 根据类型绘制不同内容
    if (type == "image") {
        drawImage(painter, option, data, isSelf);
    } else if (type == "file") {
        drawFile(painter, option, data, isSelf);
    } else if (type == "text") {
        drawMessage(painter, option, data, isSelf);
    }
}


void MessageDeletge::drawMessage(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const {
    QString message = data["content"].toString();
    painter->setPen(Qt::NoPen);
    QColor bubbleColor = QColor("#95EC69");
    painter->setBrush(bubbleColor);

    QFontMetrics metrics(painter->font()); // 获取字体度量
    QSize textSize = metrics.size(Qt::TextWordWrap, message);

    // 设置气泡长度和宽度自适应与字数长短
    int h = textSize.height();
    int w = textSize.width();
    int count_C;
    if (w < h) count_C = h / 31;
    else count_C = w / 15;
    int line = count_C / 17;
    int maxLine = qMin(count_C - 1, 15); // 气泡最大宽度
    int bubbleWidth = 55 + 24 * maxLine;
    int bubbleHeight = 25 + 40 * (line + 1);

    QRect messageRect;
    //const int avatarSize = 50;

    // 减少头像与气泡的距离（原为 65，这里设为 40）
    int avatarPadding = 65;

    if (isSelf) {
        // 自己发送的消息
        messageRect = QRect(option.rect.right() - bubbleWidth - avatarPadding, option.rect.top() + 5, bubbleWidth, bubbleHeight);
    } else {
        // 接收的消息
        messageRect = QRect(option.rect.left()+avatarPadding, option.rect.top() + 5, bubbleWidth, bubbleHeight);
    }

    // 绘制气泡
    painter->drawRoundedRect(messageRect, 5, 5); // 圆角矩形
    painter->setPen(Qt::black);
    painter->drawText(messageRect.adjusted(10, 10, -10, -10), Qt::AlignLeft | Qt::TextWordWrap, message);
}


void MessageDeletge::drawFile(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const {
    QString filePath = data["content"].toString();
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();

    QRect fileRect;
    if (isSelf) {
        fileRect = QRect(option.rect.right() - 300, option.rect.top() + 5, 200, 40);
    } else {
        fileRect = QRect(option.rect.left() + 100, option.rect.top() + 5, 200, 40);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#FFFFFF"));
    painter->drawRoundedRect(fileRect, 5, 5);

    QPixmap fileIcon = QApplication::style()->standardPixmap(QStyle::SP_FileIcon);
    painter->drawPixmap(fileRect.left() + 5, fileRect.top() + 5, 30, 30, fileIcon);

    // 绘制文件名
    painter->setPen(Qt::black);
    painter->drawText(fileRect.adjusted(40, 0, -5, 0), Qt::AlignVCenter | Qt::AlignLeft, fileName);
}

void MessageDeletge::drawImage(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const {
    QString imagePath = data["content"].toString();
    QPixmap image(imagePath);
    QSize maxSize(200, 200);
    QSize imageSize = image.size().scaled(maxSize, Qt::KeepAspectRatio);

    QRect imageRect;
    if (isSelf) {
        imageRect = QRect(option.rect.right() - imageSize.width() - 75, option.rect.top() + 5, imageSize.width(), imageSize.height());
    } else {
        imageRect = QRect(option.rect.left() + 75, option.rect.top() + 5, imageSize.width(), imageSize.height());
    }

    // 绘制背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#FFFFFF"));
    painter->drawRoundedRect(imageRect, 5, 5);

    // 绘制图片
    painter->drawPixmap(imageRect, image.scaled(imageSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QSize MessageDeletge::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariantMap data = index.data(Qt::UserRole + 1).toMap();
    QString type = data["type"].toString();

    if (type == "image") {
        return QSize(option.rect.width(), 210); // 图片的高度
    } else if (type == "file") {
        return QSize(option.rect.width(), 70); // 文件的高度
    } else if (type == "text") {
        QString message = data["content"].toString();
        QSize textSize = QFontMetrics(QFont()).size(Qt::TextWordWrap, message);
        int height = textSize.height() + 30; // 增加空间以适应边距
        return QSize(option.rect.width(), height + 20); // 返回宽高，增加一些空间来避免重叠
    }
    return QSize(option.rect.width(), 60); // 默认返回
}
MessageSend::MessageSend(QWidget *parent,User userA,User userB) :
    QWidget(parent),
    ui(new Ui::MessageSend)
{
    // 在 MessageSend 构造函数中
    ui->setupUi(this);
    delegate = new MessageDeletge(this);
    ui->listWidget->setItemDelegate(delegate); // 设置委托
    ui->listWidget->setVisible(true);
    initUi();
    //初始化接收者和发送者
    sendeR=userB;
    receiveR=userA;
    // 加载用户头像
    myAvatar.load(userA.path);
    otherAvatar.load(userB.path);
    ui->converSationName->setText(userA.name);
}
void MessageSend::initUi(){
    ui->listWidget->setStyleSheet("background-image: url(../QQphoto/2.png)");
    ui->fileBtn->setStyleSheet("background-image: url(../QQphoto/2.png)");
    ui->sendBtn->setStyleSheet("background-image: url(../QQphoto/2.png)");
    ui->imageBtn->setStyleSheet("background-image: url(../QQphoto/2.png)");
    ui->lineEdit->setStyleSheet("background-image: url(../QQphoto/2.png)");
}
MessageSend::~MessageSend() {
    delete ui;
}

void MessageSend::on_sendBtn_clicked() {
    QString message = ui->lineEdit->text();
    if (!message.isEmpty()) {
        // 构造特殊格式的消息，包含接收者ID
        QString fullMessage = QString("SendMessage:%1:%2:%3")
            .arg(sendeR.account)     // 发送者ID
            .arg(receiveR.account)   // 接收者ID
            .arg(message);           // 消息内容

        // 发送给Widget处理
        emit sendMessageToWidget(fullMessage);

        // 在本地对话框显示消息
        addMessage(message, true);
        ui->lineEdit->clear();
    }
}

void MessageSend::on_fileBtn_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"), "", tr("所有文件(*.*)"));
    if (!fileName.isEmpty()) {
        //发送到服务器
        QString fileMessage=QString("SendFile+%1+%2+%3")
                          .arg(sendeR.account).arg(receiveR.account).arg(fileName);
        emit sendMessageToWidget(fileMessage);
        addFile(fileName, true);
    }
}

void MessageSend::on_imageBtn_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件(*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        //发送到服务器
        QString imageMessage=QString("SendImage+%1+%2+%3")
                .arg(sendeR.account).arg(receiveR.account).arg(fileName);
        emit sendMessageToWidget(imageMessage);
        addImage(fileName, true);
    }
}

void MessageSend::addMessage(const QString &message, bool isSelf) {
    qDebug() << "Adding message:" << message; // 调试
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    QVariantMap data;
    data["type"] = "text";
    data["content"] = message;
    data["isSelf"] = isSelf;
    data["avatar"] = !isSelf ? myAvatar : otherAvatar;
    //new
    data["timestamp"]=QDateTime::currentDateTime();
    QDateTime currentTime=QDateTime::currentDateTime();
    //友好时间显示
    data["friendlyTime"]=TimeStampHelper::formatFriendlyTime(currentTime);

    item->setData(Qt::UserRole + 1, data);
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToBottom();
}

void MessageSend::addImage(const QString &imagePath, bool isSelf) {
    qDebug() << "Adding image:" << imagePath; // 调试输出
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    QVariantMap data;
    data["type"] = "image";
    data["content"] = imagePath;
    data["isSelf"] = isSelf;
    data["avatar"] = !isSelf ? myAvatar : otherAvatar;
    data["timestamp"]=QDateTime::currentDateTime();

    item->setData(Qt::UserRole + 1, data);
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToBottom();


}

void MessageSend::addFile(const QString &filePath, bool isSelf) {
    qDebug() << "Adding file:" << filePath; // 调试输出
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    QVariantMap data;
    data["type"] = "file";
    data["content"] = filePath;
    data["isSelf"] = isSelf;
    data["avatar"] = !isSelf ? myAvatar : otherAvatar;
    data["timestamp"]=QDateTime::currentDateTime();

    item->setData(Qt::UserRole + 1, data);
    ui->listWidget->addItem(item);
    ui->listWidget->scrollToBottom();


}
void MessageSend::onMessageReceived(QString message,QString forward){
    if(forward=="NewMessage"){
        qDebug()<<"Received a message";
        addMessage(message,false);
    }else if(forward=="NewImage"){
        qDebug()<<"Received a image";
        addImage(message,false);
    }else if(forward=="NewFile"){
        qDebug()<<"Received a file";
        addFile(message,false);
    }
    qDebug()<<"use this function: onMessageReceived!";
}
void MessageSend::paintEvent(QPaintEvent *event){
    QPainter paint(this);
    QPixmap background("..\\QQphoto\\2.png");
    //拖放整个图片填充背景
    paint.drawPixmap(rect(),background);
    QWidget::paintEvent(event);
}
