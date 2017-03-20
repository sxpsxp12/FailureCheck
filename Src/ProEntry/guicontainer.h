#ifndef GUICONTAINER_H
#define GUICONTAINER_H

#include <QWidget>
#include <QPalette>
#include <QColor>

class RequestDataThread;
class PrimaryWidget;
class SecondaryWidget;
class FailureList;
class FailureDetails;

class BusIdWidget;
class InterfaceLampHole;

class DCUWidget;

class GCUWidget;
class LGWidget;

class FailureRecord;

class LadderDiagram;

class SchematicDirectory;
class SchematicDisplay;

class ImportExport;

namespace Ui {
class GuiContainer;
}

class GuiContainer : public QWidget
{
    Q_OBJECT
    
public:
    explicit GuiContainer(RequestDataThread *requestDataThread,QWidget *parent = 0);
    ~GuiContainer();

    void InitWidgetTitleBar();      //初始化窗体的标题栏

private slots:
    void slot_refreshCurrentTime();     //实时刷新时间

    void slot_setGuiContainerCurrentWidget(int);    //容器中当前的widget

    void slot_sendGUIContainerTitleBarStatus(int guiContainerTitleBarName,int guiContainerTitleBarStatus);     //标题栏状态接受槽
    
    void on_pbt_schematicDiagram_clicked(); //原理图点击

    void on_pbt_mainWidget_clicked();   //主页面点击

    void on_pbt_return_clicked();   //返回点击

    void on_pbt_LCU_clicked();      //LCU点击槽函数

    void on_pbt_troubleRecord_clicked();

    void on_pbt_GCU_clicked();

    void on_pbt_microcomputerCabinet_clicked();

    void on_pbt_ladderDiagram_clicked();

    void on_pbt_storage_clicked();

private:
    Ui::GuiContainer *ui;

    RequestDataThread *m_requestDataThread;
    //主界面
    //一级界面
    PrimaryWidget *m_primaryWidget;
    //二级界面
    SecondaryWidget *m_secondaryWidget;
    //故障列表界面
    FailureList *m_failureList;
    //故障详情界面
    FailureDetails *m_FailureDetails;

    //LCU界面
    //线号界面
    BusIdWidget *m_busIdWidget;
    //接口灯孔界面
    InterfaceLampHole *m_interfaceLampHole;

    //微机柜
    DCUWidget *m_DCUWidget;

    //GCU/列供
    //GCU
    GCUWidget *m_GCUWidget;
    //LG
    LGWidget *m_LGWidget;

    //故障记录
    FailureRecord *m_failureRecord;

    //梯形图
    LadderDiagram *m_ladderDiagram;

    //原理图
    //原理图目录界面
    SchematicDirectory *m_schematicDirectory;
    //原理图展示界面
    SchematicDisplay *m_schematicDisplay;

    //导入导出界面
    ImportExport *m_importExport;

    void initWidget();
};

#endif // GUICONTAINER_H
