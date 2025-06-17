#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QWidget>
#include "user.h"
#include <QPixmap>
#include <QPainterPath>
namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = nullptr,int id=0);
    ~Registration();
    void paintEvent(QPaintEvent *event);
    QString tx_path;
    QString user_name;
    QString user_password;
    QString user_account;
    QPixmap imageChange(const QString &path,int d);
    int user_id;
    User u;
    bool regSuccessfully=false;
    QString getRandomAccount();
    bool insertIntoDb();
    void initUi();
private slots:
    void on_chooseBtn_clicked();

    void on_registBtn_clicked();

private:
    Ui::Registration *ui;
};

#endif // REGISTRATION_H
