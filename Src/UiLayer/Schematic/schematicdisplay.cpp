#include "schematicdisplay.h"
#include "ui_schematicdisplay.h"
#include <QDebug>
#include "enumtypeheader.h"

SchematicDisplay::SchematicDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchematicDisplay)
{
    ui->setupUi(this);
    m_pixmap = new QPixmap;
    pixItem = new SchematicGraphicsItem;
    //实例化对象
    Scene = new QGraphicsScene;
    Scene->addItem(pixItem);            //将该图元对象添加到场景中
    ui->graphicsView_diagram->setScene(Scene);//将视图对象于场景相连
    page = 0;       //初始化页码
}

SchematicDisplay::~SchematicDisplay()
{
    delete ui;

    if(m_pixmap != NULL)
    {
        delete m_pixmap;
    }

    if(pixItem != NULL)
    {
        delete pixItem;
    }


    if(Scene != NULL)
    {
        delete Scene;
    }
}
//图元初始化方法
void SchematicDisplay::InitGraphicsItem()
{
    flagBig = 0;
    flagSmall = 0;
    pixItem->setScaleFlag(flagBig,flagSmall);
    Scene->setSceneRect(pixItem->boundingRect());     //设定场景对象的显示区域,场景和图元的大小一样
    pixItem->resetTransform();     //初始化图元

    //初始化场景大小
    slot_changeSceneSize(SCHEMA_INITSCALE);
}

//图元添加到场景中
void SchematicDisplay::ItemAddScene(int diagram)
{
    if(map.contains(diagram))
    {
        //将图元添加到场景中
        m_pixmap->load(map.value(diagram));
        if(Scene->selectedItems().count() != 0) //如果场景中有图元，删除旧的图元，添加新的图元
        {
//            qDebug() << Scene->selectedItems().count();
            Scene->removeItem(Scene->focusItem()); //移除上一个图元
        }
        pixItem->loadSchematicGraphicsItem(m_pixmap);
//        Scene->addItem(pixItem);            //将该图元对象添加到场景中
        pixItem->setPos(0,0);               //设置此图元在场景中的位置为中心（0，0）

        InitGraphicsItem();                 //初始化图元
        upDataPage(diagram);                //更新页码
        connect(pixItem,SIGNAL(signal_changeSceneSize(qreal)),this,SLOT(slot_changeSceneSize(qreal)));
    }
}
//更新页码
void SchematicDisplay::upDataPage(int pageNum)
{
    page = pageNum;
    ui->label_diagramPageNum->setText(tr("第%1页").arg(page+1));   //更新页码
}
//放大
void SchematicDisplay::on_pbt_bigger_clicked()
{
    flagBig = 1;
    flagSmall = 0;
    pixItem->setScaleFlag(flagBig,flagSmall);
}
//缩小
void SchematicDisplay::on_pbt_smaller_clicked()
{
    flagBig = 0;
    flagSmall = 1;
    pixItem->setScaleFlag(flagBig,flagSmall);
}
//复位
void SchematicDisplay::on_pbt_diagramReset_clicked()
{
    InitGraphicsItem();
}

//槽函数，接收当前的目录的原理图
void SchematicDisplay::slot_setSchematicCurrentDiagram(QMap<int,QString> SchematicMap,int diagram)
{
    map = SchematicMap;
    ItemAddScene(diagram);      //图元添加到场景中
}
//实时改变场景的大小
void SchematicDisplay::slot_changeSceneSize(qreal s)
{
    Scene->setSceneRect(-pixItem->boundingRect().width()*s/2,-pixItem->boundingRect().height()*s/2,pixItem->boundingRect().width()*s,pixItem->boundingRect().height()*s);
}


//返回目录
void SchematicDisplay::on_pbt_returnSchematic_clicked()
{
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DIRECTORY);
}
//上一页
void SchematicDisplay::on_pbt_upPage_clicked()
{
    page--;
    page = (page + map.count()) % map.count();
    ItemAddScene(page);//将本页的图元添加到场景中
}
//下一页
void SchematicDisplay::on_pbt_downPage_clicked()
{
    page++;
    page = page % map.count();
    ItemAddScene(page); //将本页的图元添加到场景中
}
