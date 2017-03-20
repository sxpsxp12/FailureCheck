#ifndef SCHEMATICDISPLAY_H
#define SCHEMATICDISPLAY_H

#include <QWidget>
#include "schematicgraphicsitem.h"  //自定义图元类
#include <QGraphicsScene>
#include <QPixmap>
#include <QMap>
#include <QMessageBox>
#include "enumtypeheader.h"

namespace Ui {
class SchematicDisplay;
}

class SchematicDisplay : public QWidget
{
    Q_OBJECT
    
public:
    explicit SchematicDisplay(QWidget *parent = 0);
    ~SchematicDisplay();
    void InitGraphicsItem();    //图元初始化方法
    void ItemAddScene(int);        //将图元添加到场景中
    void upDataPage(int);          //更新页码
signals:
    void signal_setGuiContainerCurrentWidget(int);

private slots:

    void on_pbt_bigger_clicked();       //放大

    void on_pbt_smaller_clicked();      //缩小

    void on_pbt_diagramReset_clicked(); //复位

    void slot_setSchematicCurrentDiagram(QMap<int,QString>,int);  //原理图

    void slot_changeSceneSize(qreal);         //改变场景的大小

    void on_pbt_returnSchematic_clicked();

    void on_pbt_upPage_clicked();

    void on_pbt_downPage_clicked();

private:
    Ui::SchematicDisplay *ui;
    QMap<int,QString> map;
    SchematicGraphicsItem *pixItem;
    QGraphicsScene *Scene;
    QPixmap *m_pixmap;
    int flagBig;    //放大的标志位
    int flagSmall;  //缩小的标志位
    int page;       //页码
};

#endif // SCHEMATICDISPLAY_H
