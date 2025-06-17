#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H
#include <QListWidget>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
class BackgroundImage : public QListWidget
{
    Q_OBJECT
public:
    explicit BackgroundImage(QWidget *parent = nullptr);
    void setbackGroundImage(const QString &imagePath);//设置背景图片
protected:
    void paintEvent(QPaintEvent *event);//重绘事件
private:
    QPixmap bgPix;

signals:

public slots:
};

#endif // BACKGROUNDIMAGE_H
