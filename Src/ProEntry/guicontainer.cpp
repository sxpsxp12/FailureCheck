#include "guicontainer.h"
#include "ui_guicontainer.h"
#include <QThread>
#include "enumtypeheader.h"
#include "requestdatathread.h"

#include "primarywidget.h"
#include "secondarywidget.h"
#include "failurelist.h"
#include "failuredetails.h"

#include "busidwidget.h"
#include "interfacelamphole.h"

#include "dcuwidget.h"

#include "gcuwidget.h"
#include "lgwidget.h"

#include "failurerecord.h"

#include "ladderdiagram.h"

#include "schematicdirectory.h"
#include "schematicdisplay.h"

#include "importexport.h"

#include <QTime>
#include <QTimer>

GuiContainer::GuiContainer(RequestDataThread *requestDataThread,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GuiContainer)
{
    ui->setupUi(this);

    setFixedSize(800, 600);

    m_requestDataThread = requestDataThread;

    QTimer *timer = new QTimer;
    timer->start(1000);
    ui->lbl_dateYMD->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->lbl_dateHMS->setText(QTime::currentTime().toString("hh:mm:ss"));
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_refreshCurrentTime()));

    initWidget();
    //初始化为显示primaryWidget
    ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_mainWidget);
    ui->stackedWidget_mainWidget->setCurrentWidget(ui->page_primaryWidget);

    InitWidgetTitleBar();//初始化窗体的标题栏
}

GuiContainer::~GuiContainer()
{
    delete ui;

    if(m_requestDataThread != NULL)
    {
        delete m_requestDataThread;
    }

    if(m_primaryWidget != NULL)
    {
        delete m_primaryWidget;
    }

    if(m_secondaryWidget != NULL)
    {
        delete m_secondaryWidget;
    }

    if(m_failureList != NULL)
    {
        delete m_failureList;
    }

    if(m_FailureDetails != NULL)
    {
        delete m_FailureDetails;
    }

    if(m_busIdWidget != NULL)
    {
        delete m_busIdWidget;
    }

    if(m_interfaceLampHole != NULL)
    {
        delete m_interfaceLampHole;
    }

    if(m_DCUWidget != NULL)
    {
        delete m_DCUWidget;
    }

    if(m_GCUWidget != NULL)
    {
        delete m_GCUWidget;
    }

    if(m_LGWidget != NULL)
    {
        delete m_LGWidget;
    }

    if(m_failureRecord != NULL)
    {
        delete m_failureRecord;
    }

    if(m_ladderDiagram != NULL)
    {
        delete m_ladderDiagram;
    }

    if(m_schematicDirectory != NULL)
    {
        delete m_schematicDirectory;
    }

    if(m_schematicDisplay != NULL)
    {
        delete m_schematicDisplay;
    }

    if(m_importExport != NULL)
    {
        delete m_importExport;
    }
}

//初始化窗体的标题兰
void GuiContainer::InitWidgetTitleBar()
{
    ui->lbl_LCUState->setText(tr(""));

    ui->lbl_MicrocomputerCabinetOneState->setText(tr(""));

    ui->lbl_MicrocomputerCabinetTwoState->setText(tr(""));

    ui->lbl_mainEngineState->setText(tr(""));

    connect(m_requestDataThread,SIGNAL(signal_sendGUIContainerTitleBarStatus(int,int)),this,SLOT(slot_sendGUIContainerTitleBarStatus(int,int)));
}

void GuiContainer::slot_refreshCurrentTime()
{
    ui->lbl_dateYMD->setText(QDate::currentDate().toString("yyyy-MM-dd"));
    ui->lbl_dateHMS->setText(QTime::currentTime().toString("hh:mm:ss"));
}

//切换界面的槽函数
void GuiContainer::slot_setGuiContainerCurrentWidget(int currentWidget)
{
    switch (currentWidget)
    {
    //主界面
    //一级界面
    case PRIMARY_INTERFACE:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_mainWidget);
        ui->stackedWidget_mainWidget->setCurrentWidget(ui->page_primaryWidget);
        m_requestDataThread->CurrentInterfaceEnumValue = PRIMARY_INTERFACE;
        break;
        //二级界面
    case SECONDARY_INTERFACE:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_mainWidget);
        ui->stackedWidget_mainWidget->setCurrentWidget(ui->page_secondaryWidget);
        m_requestDataThread->CurrentInterfaceEnumValue = SECONDARY_INTERFACE;
        break;

        //故障列表界面
    case FAILURE_LIST:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_mainWidget);
        ui->stackedWidget_mainWidget->setCurrentWidget(ui->page_failureList);
        m_requestDataThread->CurrentInterfaceEnumValue = FAILURE_LIST;
        break;
        //故障详情界面
    case FAILURE_DETAILS:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_mainWidget);
        ui->stackedWidget_mainWidget->setCurrentWidget(ui->page_failureDetails);
        m_requestDataThread->CurrentInterfaceEnumValue = FAILURE_DETAILS;
        break;

        //LCU界面
        //线号界面
    case BUS_ID_INTERFACE:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_LCU);
        ui->stackedWidget_LCU->setCurrentWidget(ui->page_busIdWidget);
        m_requestDataThread->CurrentInterfaceEnumValue = BUS_ID_INTERFACE;
        break;

        //接口灯孔界面
    case INTERFACE_LAMP_HOLE:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_LCU);
        ui->stackedWidget_LCU->setCurrentWidget(ui->page_interfaceLampHole);
        m_requestDataThread->CurrentInterfaceEnumValue = INTERFACE_LAMP_HOLE;
        break;

        //微机柜
    case DCU:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_DCU);
        m_requestDataThread->CurrentInterfaceEnumValue = DCU;
        break;

        //GCU
    case GCU:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_GCU);
        ui->stackedWidget_GCUAndLG->setCurrentWidget(ui->page_gcuWidget);
        m_requestDataThread->CurrentInterfaceEnumValue = GCU;
        break;

        //LG
    case LG:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_GCU);
        ui->stackedWidget_GCUAndLG->setCurrentWidget(ui->page_lgWidget);
        m_requestDataThread->CurrentInterfaceEnumValue = LG;
        break;

        //故障记录
    case FAILURE_RECORD:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_failureRecord);
        m_requestDataThread->CurrentInterfaceEnumValue = FAILURE_RECORD;
        break;

        //梯形图
    case LADDER_DIAGRAM:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_ladderDiagram);
        m_requestDataThread->CurrentInterfaceEnumValue = LADDER_DIAGRAM;
        break;

        //原理图
        //原理图目录界面
    case SCHEMATIC_DIRECTORY:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_schematic);
        ui->stackedWidget_schematic->setCurrentWidget(ui->page_schematicDirectory);
        m_requestDataThread->CurrentInterfaceEnumValue = SCHEMATIC_DIRECTORY;
        break;
        //原理图展示界面
    case SCHEMATIC_DISPLAY:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_schematic);
        ui->stackedWidget_schematic->setCurrentWidget(ui->page_schematicDisplay);
        m_requestDataThread->CurrentInterfaceEnumValue = SCHEMATIC_DISPLAY;
        break;

        //导入导出界面
    case IMPORT_AND_EXPORT:
        ui->stackedWidget_guiContainer->setCurrentWidget(ui->page_importAndExport);
        //将导入导出界面初始化为tip界面
        m_importExport->initImportExportWidget();
        m_requestDataThread->CurrentInterfaceEnumValue = IMPORT_AND_EXPORT;
        break;

    default:
        break;
    }
}

void GuiContainer::slot_sendGUIContainerTitleBarStatus(int guiContainerTitleBarName, int guiContainerTitleBarStatus)
{
    switch(guiContainerTitleBarName)
    {
    case LCU_TITLEBAR:
        switch(guiContainerTitleBarStatus)
        {
        case NORMAL_STATUS:
            ui->lbl_LCUState->setText(tr("正常"));
            ui->lbl_LCUState->setStyleSheet(tr("color:#00fe00;"));
            break;
        case ABNORMAL_STATUS:
            ui->lbl_LCUState->setText(tr("异常"));
            ui->lbl_LCUState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case FAILE_STATUS:
            ui->lbl_LCUState->setText(tr("失败"));
            ui->lbl_LCUState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case UNKNOWN_STATUS:
            ui->lbl_LCUState->setText(tr("未知"));
            ui->lbl_LCUState->setStyleSheet(tr("color:#fefe00;"));
            break;
        default:break;
        }
        break;
    case DCU_TITLEBAR:
        switch(guiContainerTitleBarStatus)
        {
        case NORMAL_STATUS:
            ui->lbl_MicrocomputerCabinetOneState->setText(tr("正常"));
            ui->lbl_MicrocomputerCabinetOneState->setStyleSheet(tr("color:#00fe00;"));
            break;
        case ABNORMAL_STATUS:
            ui->lbl_MicrocomputerCabinetOneState->setText(tr("异常"));
            ui->lbl_MicrocomputerCabinetOneState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case FAILE_STATUS:
            ui->lbl_MicrocomputerCabinetOneState->setText(tr("失败"));
            ui->lbl_MicrocomputerCabinetOneState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case UNKNOWN_STATUS:
            ui->lbl_MicrocomputerCabinetOneState->setText(tr("未知"));
            ui->lbl_MicrocomputerCabinetOneState->setStyleSheet(tr("color:#fefe00;"));
            break;
        default:break;
        }
        break;
    case DCU2_TITLEBAR:
        switch(guiContainerTitleBarStatus)
        {
        case NORMAL_STATUS:
            ui->lbl_MicrocomputerCabinetTwoState->setText(tr("正常"));
            ui->lbl_MicrocomputerCabinetTwoState->setStyleSheet("color:#00fe00;");
            break;
        case ABNORMAL_STATUS:
            ui->lbl_MicrocomputerCabinetTwoState->setText(tr("异常"));
            ui->lbl_MicrocomputerCabinetTwoState->setStyleSheet("color:#fe0000;");
            break;
        case FAILE_STATUS:
            ui->lbl_MicrocomputerCabinetTwoState->setText(tr("失败"));
            ui->lbl_MicrocomputerCabinetTwoState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case UNKNOWN_STATUS:
            ui->lbl_MicrocomputerCabinetTwoState->setText(tr("未知"));
            ui->lbl_MicrocomputerCabinetTwoState->setStyleSheet("color:#fefe00;");
            break;
        default:break;
        }
        break;
    case HOST_TITLEBAR:
        switch(guiContainerTitleBarStatus)
        {
        case NORMAL_STATUS:
            ui->lbl_mainEngineState->setText(tr("正常"));
            ui->lbl_mainEngineState->setStyleSheet(tr("color:#00fe00;"));
            break;
        case ABNORMAL_STATUS:
            ui->lbl_mainEngineState->setText(tr("异常"));
            ui->lbl_mainEngineState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case FAILE_STATUS:
            ui->lbl_mainEngineState->setText(tr("失败"));
            ui->lbl_mainEngineState->setStyleSheet(tr("color:#fe0000;"));
            break;
        case UNKNOWN_STATUS:
            ui->lbl_mainEngineState->setText(tr("未知"));
            ui->lbl_mainEngineState->setStyleSheet(tr("color:#fefe00;"));
            break;
        default:break;
        }
        break;

    default:break;
    }
}

void GuiContainer::initWidget()
{
    //主界面
    //一级界面
    m_primaryWidget = new PrimaryWidget(m_requestDataThread, this);
    //从一级界面切换到二级界面
    connect(m_primaryWidget,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    connect(m_requestDataThread,SIGNAL(signal_sendMasterFlag(bool)),m_primaryWidget,SLOT(slot_receiveMasterFlag(bool)));

    ui->gridLayout_primaryWidget->addWidget(m_primaryWidget);
    //发送实时数据信号槽
//    connect(m_requestDataThread, SIGNAL(signal_sendPrimaryWidgetData(RealtimeDataInfoList, FailureInfoList)),
//            m_primaryWidget, SLOT(slot_receivePrimaryWidgetData(RealtimeDataInfoList, FailureInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendPrimaryWidgetData()),m_primaryWidget,SLOT(slot_receivePrimaryWidgetData()));

    //二级界面
    m_secondaryWidget = new SecondaryWidget(m_requestDataThread, this);
    //从二级界面切换到一级界面
    connect(m_secondaryWidget,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    ui->gridLayout_secondaryWidget->addWidget(m_secondaryWidget);
    //发送实时数据信号槽
//    connect(m_requestDataThread, SIGNAL(signal_sendSecondaryWidgetData(RealtimeDataInfoList, FailureInfoList)),
//            m_secondaryWidget, SLOT(slot_receiveSecondaryWidgetData(RealtimeDataInfoList, FailureInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendSecondaryWidgetData()),
            m_secondaryWidget, SLOT(slot_receiveSecondaryWidgetData()));

    //故障列表界面
    m_failureList = new FailureList(this);
    //跳转界面
    connect(m_failureList,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    //初级界面
    connect(m_primaryWidget,SIGNAL(signal_setClickedListItem(int)),
            m_failureList,SLOT(slot_setClickedListItem(int)));

    ui->gridLayout_failureList->addWidget(m_failureList);

    //故障详情界面
    m_FailureDetails = new FailureDetails(this);
    ui->gridLayout_failureDetails->addWidget(m_FailureDetails);
    connect(m_failureList, SIGNAL(signal_sendImageName(QString)),
            m_FailureDetails, SLOT(slot_receiveImageName(QString)));

    //LCU界面
    //线号界面
    m_busIdWidget = new BusIdWidget(m_requestDataThread, this);
    connect(m_busIdWidget,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));                    //跳转界面信号槽
    ui->gridLayout_busIdWidget->addWidget(m_busIdWidget);
//    connect(m_requestDataThread, SIGNAL(signal_sendBusIdWidgetData(RealtimeDataInfoList)),
//            m_busIdWidget, SLOT(slot_receiveBusIdWidgetData(RealtimeDataInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendBusIdWidgetData()),
            m_busIdWidget, SLOT(slot_receiveBusIdWidgetData()));

    //接口灯孔界面
    m_interfaceLampHole = new InterfaceLampHole(m_requestDataThread, this);
    connect(m_interfaceLampHole,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));                      //跳转界面信号槽
    ui->gridLayout_interfaceLampHole->addWidget(m_interfaceLampHole);
//    connect(m_requestDataThread, SIGNAL(signal_sendInterfaceLampHoleWidgetData(RealtimeDataInfoList)),
//            m_interfaceLampHole, SLOT(slot_receiveInterfaceLampHoleWidgetData(RealtimeDataInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendInterfaceLampHoleWidgetData()),
            m_interfaceLampHole, SLOT(slot_receiveInterfaceLampHoleWidgetData()));

    connect(m_busIdWidget,SIGNAL(signal_tableWidgetItemClicked(QTableWidgetItem*)),
            m_interfaceLampHole,SLOT(slot_tableWidgetItemClicked(QTableWidgetItem*)));
    //LCU界面修改按钮焦点信号槽
    connect(m_busIdWidget, SIGNAL(signal_setFocusToInterfaceLampHole()), m_interfaceLampHole, SLOT(slot_setFocus()));
    connect(m_interfaceLampHole, SIGNAL(signal_setFocusToBusIdWidget()), m_busIdWidget, SLOT(slot_setFocus()));

    //微机柜
    m_DCUWidget = new DCUWidget(m_requestDataThread, this);
    ui->gridLayout_DCU->addWidget(m_DCUWidget);
//    connect(m_requestDataThread, SIGNAL(signal_sendDcuWidgetData(RealtimeDataInfoList)),
//            m_DCUWidget, SLOT(slot_receiveDcuWidgetData(RealtimeDataInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendDcuWidgetData()),
            m_DCUWidget, SLOT(slot_receiveDcuWidgetData()));
    //GCU
    m_GCUWidget = new GCUWidget(m_requestDataThread, this);
    connect(m_GCUWidget,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    ui->gridLayout_gcuWidget->addWidget(m_GCUWidget);
//    connect(m_requestDataThread, SIGNAL(signal_sendGcuWidgetData(RealtimeDataInfoList)),
//            m_GCUWidget, SLOT(slot_receiveGcuWidgetData(RealtimeDataInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendGcuWidgetData()),
            m_GCUWidget, SLOT(slot_receiveGcuWidgetData()));
    //LG
    m_LGWidget = new LGWidget(m_requestDataThread, this);
    connect(m_LGWidget,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    ui->gridLayout_lgWidget->addWidget(m_LGWidget);
//    connect(m_requestDataThread, SIGNAL(signal_sendLgWidgetData(RealtimeDataInfoList)),
//            m_LGWidget, SLOT(slot_receiveLgWidgetData(RealtimeDataInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendLgWidgetData()),
            m_LGWidget, SLOT(slot_receiveLgWidgetData()));

    //GCU/列供界面改变按钮焦点信号槽
    connect(m_GCUWidget, SIGNAL(signal_setFocusToLGWidget()), m_LGWidget, SLOT(slot_setFocus()));
    connect(m_LGWidget, SIGNAL(signal_setFocusToGCUWidget()), m_GCUWidget, SLOT(slot_setFocus()));

    //故障记录
    m_failureRecord = new FailureRecord(m_requestDataThread,this);
    ui->gridLayout_failureRecord->addWidget(m_failureRecord);
//    connect(m_requestDataThread,SIGNAL(signal_sendFailureRecordWidgetData(FailureHistoryRecordInfoList)),m_failureRecord,SLOT(slot_ReceiveFailureRecordWidgetData(FailureHistoryRecordInfoList)));
    connect(m_requestDataThread,SIGNAL(signal_sendFailureRecordWidgetData()),m_failureRecord,SLOT(slot_ReceiveFailureRecordWidgetData()));

    //梯形图
    m_ladderDiagram = new LadderDiagram(m_requestDataThread, this);
    ui->gridLayout_ladderDiagram->addWidget(m_ladderDiagram);
    connect(m_failureList,SIGNAL(signal_sendOutputBusId(QString)),m_ladderDiagram,SLOT(slot_receiveOutputBusId(QString)));
//    connect(m_requestDataThread, SIGNAL(signal_sendLadderDiagramWidgetData(LadderDiagramInfoList)),
//            m_ladderDiagram, SLOT(slot_receiveLadderDiagramWidgetData(LadderDiagramInfoList)));
    connect(m_requestDataThread, SIGNAL(signal_sendLadderDiagramWidgetData()),
            m_ladderDiagram, SLOT(slot_receiveLadderDiagramWidgetData()));
    //原理图
    //原理图目录界面
    m_schematicDirectory = new SchematicDirectory(this);
    connect(m_schematicDirectory, SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this, SLOT(slot_setGuiContainerCurrentWidget(int)));
    ui->gridLayout_schematicDirectory->addWidget(m_schematicDirectory);

    //原理图展示界面
    m_schematicDisplay = new SchematicDisplay(this);
    //返回目录
    connect(m_schematicDisplay,SIGNAL(signal_setGuiContainerCurrentWidget(int)),
            this,SLOT(slot_setGuiContainerCurrentWidget(int)));
    //接收点击目录时发送的原理图
    connect(m_schematicDirectory,SIGNAL(signal_setSchematicCurrentDiagram(QMap<int,QString>,int)),
            m_schematicDisplay,SLOT(slot_setSchematicCurrentDiagram(QMap<int,QString>,int)));
    ui->gridLayout_schematicDisplay->addWidget(m_schematicDisplay);

    //导入导出界面
    m_importExport = new ImportExport(m_requestDataThread,this);
    //connect(m_requestDataThread,SIGNAL(signal_sendInitDataBaseIsSuccess(FileInfoClassList)),m_importExport,SLOT(slot_InitDataBaseIsSuccess(FileInfoClassList)));
    ui->gridLayout_importAndExport->addWidget(m_importExport);
}

//原理图点击
void GuiContainer::on_pbt_schematicDiagram_clicked()
{
    slot_setGuiContainerCurrentWidget(SCHEMATIC_DIRECTORY);
}

//主页面点击
void GuiContainer::on_pbt_mainWidget_clicked()
{
    slot_setGuiContainerCurrentWidget(PRIMARY_INTERFACE);
}
//返回点击
void GuiContainer::on_pbt_return_clicked()
{
    slot_setGuiContainerCurrentWidget(PRIMARY_INTERFACE);
}
//LCU点击槽函数
void GuiContainer::on_pbt_LCU_clicked()
{
    slot_setGuiContainerCurrentWidget(BUS_ID_INTERFACE);
}

//故障记录点击槽函数
void GuiContainer::on_pbt_troubleRecord_clicked()
{
    slot_setGuiContainerCurrentWidget(FAILURE_RECORD);
}

//GCU点击槽函数
void GuiContainer::on_pbt_GCU_clicked()
{
    slot_setGuiContainerCurrentWidget(GCU);
}

//微机柜界面点击槽函数
void GuiContainer::on_pbt_microcomputerCabinet_clicked()
{
    slot_setGuiContainerCurrentWidget(DCU);
}

void GuiContainer::on_pbt_ladderDiagram_clicked()
{
    slot_setGuiContainerCurrentWidget(LADDER_DIAGRAM);
}

void GuiContainer::on_pbt_storage_clicked()
{
    slot_setGuiContainerCurrentWidget(IMPORT_AND_EXPORT);
}
