#ifndef SCHEMATICGRAPHICSITEM_H
#define SCHEMATICGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QRectF>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include <QString>

class SchematicGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    SchematicGraphicsItem();
    void loadSchematicGraphicsItem(QPixmap *pixmap);
    QRectF boundingRect() const;//重新定义图元的边界函数
    //重画函数
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);//重新实现鼠标点击事件
    void setScaleFlag(const int flagbig,const int flagsmall);    //设置缩放的标志位

signals:
    void signal_changeSceneSize(qreal);

private:
    QPixmap pix;        //图元显示的图片
    int flagBig;        //放大的标志位
    int flagSmall;      //缩小的标志位
    int scalebaseValue; //缩放的基准值
    int value;          //缩放的设定值
};

#endif // SCHEMATICGRAPHICSITEM_H
