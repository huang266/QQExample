#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "widget.h"
#include <QtSql>
#include <QtSql/QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QPainter>
#include <QSqlRecord>
#include <QXmlStreamReader>
#include "user.h"
#include "registration.h"
namespace Ui {
class Dialog;
}
class Widget;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    Widget *widget;
    Registration *registRation;
    void initui();
    void paintEvent(QPaintEvent *event);
    void createDateBase();
    void query();
    void changeStyle();//改变按钮样式
    QPixmap changeImage(const QString  &path,int d);//改变图片样式

    int getMaxId();
    //从xml文件中导入信息
    QVector<User> loadUsersFromXml(const QString &filePath);
    //从数据库中导入信息
    QVector<User> loadUsersFromDb();
    QVector<User>userTab;
    QString User_name;
    QString User_account;
    QString User_password;
    QString User_path;
    int User_id;
    QMap<int,bool>LoginMap;
private slots:

    void createWidgetInGUIThread(const QString& account, const QString& userPath, int userId, const QString& userName, const QVector<User>& userTab);

    void on_checkBox_stateChanged(int arg1);
    void on_logBtn_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_registBtn_clicked();


private:
    Ui::Dialog *ui;
    QSqlDatabase DB;//数据库名称
};

#endif // DIALOG_H
