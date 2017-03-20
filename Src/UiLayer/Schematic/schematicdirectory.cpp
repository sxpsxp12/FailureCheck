#include "schematicdirectory.h"
#include "ui_schematicdirectory.h"
#include "enumtypeheader.h"

SchematicDirectory::SchematicDirectory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchematicDirectory)
{
    ui->setupUi(this);
    initMap();//初始化键值容器
}

SchematicDirectory::~SchematicDirectory()
{
    delete ui;
}

//初始化键值容器
void SchematicDirectory::initMap()
{
    map.insert(MAINDIAGRAM,QString("%1Schematic/1_ss9xhc.png").arg(IMAGEPATH));   //主电路图
    map.insert(ASSISTDIAGRAM,QString("%1Schematic/2_ss9xhc.png").arg(IMAGEPATH));   //辅助电路图
    map.insert(CONTROLDIAGRAMONE,QString("%1Schematic/3_ss9xhc.png").arg(IMAGEPATH));   //控制电路图1
    map.insert(CONTROLDIAGRAMTWO,QString("%1Schematic/4_ss9xhc.png").arg(IMAGEPATH));   //控制电路图2
    map.insert(CONTROLDIAGRAMTHREE,QString("%1Schematic/5_ss9xhc.png").arg(IMAGEPATH));   //控制电路图3
    map.insert(CONTROLDIAGRAMFOUR,QString("%1Schematic/6_ss9xhc.png").arg(IMAGEPATH));   //控制电路图4
    map.insert(CONTROLDIAGRAMFIVE,QString("%1Schematic/7_ss9xhc.png").arg(IMAGEPATH));   //控制电路图5
    map.insert(CONTROLDIAGRAMSIX,QString("%1Schematic/8_ss9xhc.png").arg(IMAGEPATH));   //控制电路图6
    map.insert(CONTROLDIAGRAMSEVEN,QString("%1Schematic/9_ss9xhc.png").arg(IMAGEPATH));   //控制电路图7
    map.insert(CHANNALDIAGRAMONE,QString("%1Schematic/10_ss9xhc.png").arg(IMAGEPATH)); //管道原理图1
    map.insert(CHANNALDIAGRAMTWO,QString("%1Schematic/11_ss9xhc.png").arg(IMAGEPATH)); //管道原理图2
    map.insert(DETAILONE,QString("%1Schematic/12_ss9xhc.png").arg(IMAGEPATH)); //明细1
    map.insert(DETAILTWO,QString("%1Schematic/13_ss9xhc.png").arg(IMAGEPATH)); //明细2
    map.insert(DETAILTHREE,QString("%1Image/Schematic/14_ss9xhc.png").arg(IMAGEPATH)); //明细3
}

void SchematicDirectory::resizeEvent(QResizeEvent *)
{
    //初始化widget的背景图片
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(QPixmap(":/Schematic/Image/Schematic/schematicdirectory.png")));
    setPalette(palette);
}

//主电路图的点击事件
void SchematicDirectory::on_pbt_mainDiagram_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,MAINDIAGRAM);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//辅助电路图
void SchematicDirectory::on_pbt_assistDiagram_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,ASSISTDIAGRAM);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图1
void SchematicDirectory::on_pbt_ControlDiagramOne_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMONE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图2
void SchematicDirectory::on_pbt_ControlDiagramTwo_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMTWO);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图3
void SchematicDirectory::on_pbt_ControlDiagramThree_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMTHREE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图4
void SchematicDirectory::on_pbt_ControlDiagramFour_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMFOUR);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图5
void SchematicDirectory::on_pbt_ControlDiagramFive_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMFIVE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图6
void SchematicDirectory::on_pbt_ControlDiagramSix_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMSIX);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//控制电路图7
void SchematicDirectory::on_pbt_ControlDiagramSeven_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CONTROLDIAGRAMSEVEN);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//管道原理图1
void SchematicDirectory::on_pbt_ChannalDiagramOne_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CHANNALDIAGRAMONE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//管道原理图2
void SchematicDirectory::on_pbt_ChannalDiagramTwo_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,CHANNALDIAGRAMTWO);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//明细1
void SchematicDirectory::on_pbt_detailOne_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,DETAILONE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//明细2
void SchematicDirectory::on_pbt_detailTwo_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,DETAILTWO);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
//明细3
void SchematicDirectory::on_pbt_detailThree_clicked()
{
    emit signal_setSchematicCurrentDiagram(map,DETAILTHREE);
    emit signal_setGuiContainerCurrentWidget(SCHEMATIC_DISPLAY);
}
