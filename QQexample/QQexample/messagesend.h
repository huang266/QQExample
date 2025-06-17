#ifndef MESSAGESEND_H
#define MESSAGESEND_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QPixmap>
#include <QListWidgetItem>
#include <QVariantMap>
#include <QDateTime>
#include <QPainter>
#include <QTcpSocket>
#include <QPaintEvent>
#include "user.h"
namespace Ui {
class MessageSend;
}

class MessageDeletge : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MessageDeletge(QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool shouldDisplayTimestamp(const QDateTime &current,const QDateTime &previous){
        //如果距离上一条信息超过 固定时间 显示时间戳
        return previous.isNull()||current.secsTo(previous)>300;
    }
    void paintTimestamp(QPainter *painter,QDateTime &timestamp,const QRect &rect){
        painter->save();//
        painter->setPen(Qt::gray);//设置颜色
        painter->setFont(QFont("Arial",8));
        painter->drawText(rect,Qt::AlignCenter,timestamp.toString("MM-dd hh:mm"));
        painter->restore();//保存绘图的状态
    }

private:
    void drawImage(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const;
    void drawMessage(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const;
    void drawFile(QPainter *painter, const QStyleOptionViewItem &option, const QVariantMap &data, bool isSelf) const;
};
class MessageManager {
private:
    QList<QVariantMap> messages;

public:
    void addMessage(const QVariantMap &message) {
        // 按时间戳排序
        auto it = std::lower_bound(messages.begin(), messages.end(), message,
            [](const QVariantMap &a, const QVariantMap &b) {
                return a["timestamp"].toDateTime() < b["timestamp"].toDateTime();
            });
        messages.insert(it, message);
    }
};
class TimeStampHelper {
public:
    static QString formatFriendlyTime(const QDateTime &timestamp) {
        QDateTime now = QDateTime::currentDateTime();
        qint64 seconds = timestamp.secsTo(now);

        if (seconds < 60) return "刚刚";
        if (seconds < 3600) return QString("%1分钟前").arg(seconds / 60);
        if (seconds < 86400) return QString("%1小时前").arg(seconds / 3600);
        if (seconds < 172800) return "昨天";

        return timestamp.toString("yyyy-MM-dd");
    }

    static bool isWithinSameDay(const QDateTime &t1, const QDateTime &t2) {
        return t1.date() == t2.date();
    }
};
class MessageSendConfig {
public:
    enum TimestampMode {
        Disabled,   // 不显示时间戳
        Always,     // 总是显示
        Interval    // 按时间间隔显示
    };

    void setTimestampMode(TimestampMode mode) {
        m_timestampMode = mode;
    }

private:
    TimestampMode m_timestampMode = Interval;
};
class MessageSend : public QWidget {
    Q_OBJECT

public:
    explicit MessageSend(QWidget *parent = nullptr,User userA={},User userB={});
    ~MessageSend();
    int count=0;
    void paintEvent(QPaintEvent* event);
    void initUi();
   // void paintEvent(QPaintEvent *event);
private slots:
    void on_sendBtn_clicked();
    void on_fileBtn_clicked();
    void on_imageBtn_clicked();
  //  void readMessage();

private:

   // void connectToServer();

    Ui::MessageSend *ui;
    MessageDeletge *delegate;
    QPixmap myAvatar;
    QPixmap otherAvatar;
public:
    User sendeR;
    User receiveR;
    void addMessage(const QString &message, bool isSelf);
    void addImage(const QString &imagePath, bool isSelf);
    void addFile(const QString &filePath, bool isSelf);
    void onMessageReceived(QString message,QString forward);

    //与服务器进行通信
    //QTcpSocket *socket;
signals:
    void sendMessageToWidget(const QString message);

};

#endif // MESSAGESEND_H
