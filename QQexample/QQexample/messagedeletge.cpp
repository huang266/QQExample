//#include "messagedeletge.h"
//#include <QPainter>
//#include <QApplication>
//#include <QFileInfo>
//#include <QtDebug>
//MessageDeletge::MessageDeletge(QObject *parent) : QStyledItemDelegate (parent)
//{

//}
//void MessageDeletge::paint(QPainter *painter,
//            const QStyleOptionViewItem &option, const QModelIndex &index) const{
//    QVariantMap data=index.data(Qt::UserRole+1).toMap();//
//    qDebug() << "Data received for painting:" << data;
//    bool isSelf=data["isSelf"].toBool();
//    QString type=data["type"].toString();
//    QPixmap avatar=data["avatar"].value<QPixmap>();
//    //绘制头像
//    painter->setRenderHint(QPainter::Antialiasing);
//    QRect avatorRect;
//    if(isSelf){
//        avatorRect=QRect(option.rect.right()-40,option.rect.top()+5,35,35);
//    }else{
//        avatorRect=QRect(option.rect.left()+5,option.rect.top()+5,35,35);
//    }
//    QPainterPath path;
//    path.addEllipse(avatorRect);
//    painter->setClipPath(path);
//    painter->drawPixmap(avatorRect,avatar);
//    painter->setClipPath(QPainterPath());
//    //根据类型绘制不同内容
//    if(type=="image"){
//        //绘制图片
//       drawImage(painter,option,data,isSelf);
//    }else if(type=="file"){
//        //绘制文件
//        drawFile(painter,option,data,isSelf);
//    }else if(type=="text"){
//        //绘制文本
//        drawMessage(painter,option,data,isSelf);
//    }

//}
//void MessageDeletge::drawMessage(QPainter *painter, const QStyleOptionViewItem &option,
//                                 const QVariantMap &data, bool isSelf) const{
//    QString message=data["content"].toString();
//    qDebug()<<message;
//    //绘制消息气泡
//    painter->setPen(Qt::NoPen);
//    painter->setBrush(isSelf?QColor("#95EC69"):QColor(Qt::black));

//    QRect messageRect;
//    if(isSelf){
//        messageRect=QRect(option.rect.right()-300,option.rect.top()+5,
//                          250,option.rect.height()-10);
//    }else{
//        messageRect=QRect(option.rect.left()+45,option.rect.top()+5,
//                          250,option.rect.height()-10);
//    }

//    //绘制文本
//    painter->drawRoundedRect(messageRect,5,5);
//    painter->setPen(Qt::black);
//    painter->drawText(messageRect.adjusted(5,5,-5,-5),
//                      Qt::TextWordWrap|Qt::AlignCenter,message);

//}
//void MessageDeletge::drawFile(QPainter *painter, const QStyleOptionViewItem &option,
//                              const QVariantMap &data, bool isSelf) const{
//    QString filePath=data["content"].toString();
//    QFileInfo fileInfo(filePath);
//    QString fileName=fileInfo.fileName();
//    //绘制文件图标和名称
//    QRect fileRect;
//    if(isSelf){
//        fileRect=QRect(option.rect.right()-250,option.rect.top()+5,
//                       200,40);
//    }else {
//        fileRect=QRect(option.rect.left()+45,option.rect.top()+5,
//                       200,40);
//    }
//    //绘制背景
//    painter->setPen(Qt::NoPen);
//    painter->setBrush(QColor("#FFFFFF"));
//    painter->drawRoundedRect(fileRect,5,5);

//    //绘制文件图标
//    QPixmap fileIcon=QApplication::style()->standardPixmap(QStyle::SP_FileIcon);
//    painter->drawPixmap(fileRect.left()+5,fileRect.top()+5,30,30,fileIcon);

//    //绘制文件名
//    painter->setPen(Qt::black);
//    painter->drawText(fileRect.adjusted(40,0,-5,0),
//                      Qt::AlignVCenter|Qt::AlignLeft,fileName);

//}
//void MessageDeletge::drawImage(QPainter *painter, const QStyleOptionViewItem &option,
//                               const QVariantMap &data, bool isSelf) const{
//    QString imagePath=data["content"].toString();
//    QPixmap image(imagePath);
//    //限制图片最大尺寸
//    QSize maxSize(200,200);
//    QSize imageSize=image.size().scaled(maxSize,Qt::KeepAspectRatio);

//    //计算图片位置
//    QRect imageRect;
//    if(isSelf){
//        imageRect=QRect(option.rect.right()-imageSize.width()-50,
//                        option.rect.top()+5,imageSize.width(),
//                        imageSize.height());
//    }else{
//        imageRect=QRect(option.rect.left()+45,
//                        option.rect.top()+5,imageSize.width(),
//                        imageSize.height());
//    }
//    //绘制背景
//    painter->setPen(Qt::NoPen);
//    painter->setBrush(QColor("#FFFFFF"));
//    painter->drawRoundedRect(imageRect,5,5);

//    //绘制图片
//    painter->drawPixmap(imageRect,image.scaled(imageSize,Qt::KeepAspectRatio,
//                                               Qt::SmoothTransformation));
//}
//QSize MessageDeletge::sizeHint(const QStyleOptionViewItem &option,
//                               const QModelIndex &index) const{
//    QVariantMap data=index.data(Qt::UserRole+1).toMap();
//    QString type=data["type"].toString();
//    if(type=="image"){
//        return  QSize(option.rect.width(),210);//图片信息高度
//    }else if(type=="file"){
//        return QSize(option.rect.width(),70); //文件信息高度
//    }else{
//        return  QSize(option.rect.width(),80);//文本信息高度
//    }
//}
