#include "backgroundimage.h"

BackgroundImage::BackgroundImage(QWidget *parent) : QListWidget(parent)
{

}
void BackgroundImage::setbackGroundImage(const QString &imagePath){
    bgPix.load(imagePath);
}
void BackgroundImage::paintEvent(QPaintEvent *event){
    QPainter painter(viewport());
    if(!bgPix.isNull()){
        //绘制背景图像，适应小部件大小
        painter.drawPixmap(rect(),bgPix.scaled(size(),
                                  Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    }
    //调用父类的paintevent
    QListWidget::paintEvent(event);
}
