#include "schematicgraphicsitem.h"
#include "enumtypeheader.h"

SchematicGraphicsItem::SchematicGraphicsItem()
{
}

void SchematicGraphicsItem::loadSchematicGraphicsItem(QPixmap *pixmap)
{
    pix = *pixmap;
}
//重新定义图元的边界函数
QRectF SchematicGraphicsItem::boundingRect() const
{
    return QRectF(-2-pix.width()/2,-2-pix.height()/2,pix.width()+4,pix.height()+4);
}

//重画函数
void SchematicGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(-pix.width()/2,-pix.height()/2,pix);
}

//重新实现鼠标点击事件
void SchematicGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qreal s;
    setTransformOriginPoint(event->scenePos());   //设置局部放大的源点,场景大小和图元的大小一样
    if(flagBig == 1 && flagSmall == 0)              //放大图片
    {
        value++;        //设定值+1
    }else if(flagBig == 0 && flagSmall == 1)        //缩小图片
    {
        value--;        //设定值-1
    }

    if(value >= scalebaseValue) //将设定值于基准值比较
    {
        s = pow(1.1,(value-scalebaseValue)) * 0.2;        //放大 计算x的y方次 参数都是double类型
    }else
    {
        s = pow(1/1.1,(scalebaseValue-value)) * 0.2;      //缩小比例

        if(s <= 0.2)
        {
            s = 0.2;
            value++;    //当缩小比例小于0.2时，value设定值不变
        }
    }

    if(s >= 0.2)    //缩小比例不小于0.2
    {
        setScale(s);            //图片的缩放比例
        setTransformOriginPoint(0,0);       //将原点重定位为（0，0）
        emit signal_changeSceneSize(s);
    }
}
//设置缩放的标志位
void SchematicGraphicsItem::setScaleFlag(const int flagbig, const int flagsmall)
{
    if(flagbig == flagsmall)        //放大缩小标志位一样，说明是在复位图元或者刚开始加载图片初始化
    {
        setScale(SCHEMA_INITSCALE);                //设置缩放比例为默认的0.2
        scalebaseValue = 10;        //将基准值和设定值都设置为10
        value = 10;
    }
    flagBig = flagbig;
    flagSmall = flagsmall;
}
