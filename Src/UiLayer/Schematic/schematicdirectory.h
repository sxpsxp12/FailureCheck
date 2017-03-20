#ifndef SCHEMATICDIRECTORY_H
#define SCHEMATICDIRECTORY_H

#include <QWidget>
#include <QString>
#include <QMap>
#include "enumtypeheader.h"
#include <QPalette>
#include <QPixmap>
#include <QBrush>

namespace Ui {
class SchematicDirectory;
}

class SchematicDirectory : public QWidget
{
    Q_OBJECT

public:
    explicit SchematicDirectory(QWidget *parent = 0);
    ~SchematicDirectory();
    void initMap();     //初始化键值容器

protected:
    void resizeEvent(QResizeEvent *);

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_setSchematicCurrentDiagram(QMap<int,QString>,int);

private slots:
    void on_pbt_mainDiagram_clicked();//主电路图的点击事件

    void on_pbt_assistDiagram_clicked();    ////辅助电路图

    void on_pbt_ControlDiagramOne_clicked();//控制电路图1

    void on_pbt_ControlDiagramTwo_clicked();//控制电路图2

    void on_pbt_ControlDiagramThree_clicked();//控制电路图3

    void on_pbt_ControlDiagramFour_clicked();//控制电路图4

    void on_pbt_ControlDiagramFive_clicked();//控制电路图5

    void on_pbt_ControlDiagramSix_clicked();//控制电路图6

    void on_pbt_ControlDiagramSeven_clicked();//控制电路图7

    void on_pbt_ChannalDiagramOne_clicked();//管道原理图1

    void on_pbt_ChannalDiagramTwo_clicked();//管道原理图2

    void on_pbt_detailOne_clicked();//明细1

    void on_pbt_detailTwo_clicked();//明细2

    void on_pbt_detailThree_clicked();//明细3

private:
    Ui::SchematicDirectory *ui;
    QMap<int,QString> map;
};

#endif // SCHEMATICDIRECTORY_H
