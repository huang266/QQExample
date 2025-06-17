#include <QThread>
#include "widget.h"
#include <QMetaObject>
#include "dialog.h" // 确保包含 dialog.h
#include "user.h"
#include <QVector>
class WidgetThread : public QThread {
    Q_OBJECT

public:
    WidgetThread(Dialog* dialog, const QString& account, const QString& userPath, int userId, const QString& userName, const QVector<User>& userTab, QObject *parent = nullptr)
        : QThread(parent), dialog(dialog), account(account), userPath(userPath), userId(userId), userName(userName), userTab(userTab){}

signals:
    void createWidgetSignal(const QString& account, const QString& userPath, int userId, const QString& userName, const QVector<User>& userTab);

protected:
    void run() override {
        // 使用信号和槽机制，将创建 Widget 的操作发送到主线程
        QMetaObject::invokeMethod(dialog, "createWidgetInGUIThread",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, account),
                                  Q_ARG(QString, userPath),
                                  Q_ARG(int, userId),
                                  Q_ARG(QString, userName),
                                  Q_ARG(QVector<User>, userTab));

        exec(); // 启动线程的事件循环
    }

private:
    Dialog* dialog; // 添加 Dialog 类的指针
    QString account;
    QString userPath;
    int userId;
    QString userName;
    QVector<User> userTab;
};
