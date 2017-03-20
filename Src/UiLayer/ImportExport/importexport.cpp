#include "importexport.h"
#include "ui_importexport.h"
#include "enumtypeheader.h"

ImportExport::ImportExport(RequestDataThread *requestDataThread,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportExport)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;

    initWidget();
    //初始化为导入界面
    initImportExportWidget();

    m_customMsgBox = new CustomMsgBox();

    connect(m_customMsgBox,SIGNAL(signal_sendOkFunctionFlag(int)),this,SLOT(slot_customMsgBoxOkFunctionFlag(int)));
    connect(m_customMsgBox,SIGNAL(signal_sendCancleFunctionFlag(int)),this,SLOT(slot_customMsgBoxCancleFunctionFlag(int)));
}

ImportExport::~ImportExport()
{
    delete ui;

    if(m_customMsgBox != NULL)
    {
        delete m_customMsgBox;
    }

    if(m_importFunction != NULL)
    {
        delete m_importFunction;
    }

    if(m_exportFunction != NULL)
    {
        delete m_exportFunction;
    }
}

void ImportExport::initImportExportWidget()
{
    //初始化为导入界面
    ui->stackWidget_functionContainer->setCurrentWidget(ui->page_tip);
}

void ImportExport::on_pbt_import_clicked()
{
    if(detectUSB())          //检测到U盘
    {
        ui->stackWidget_functionContainer->setCurrentWidget(ui->page_import);
        QStringList list;
        CBaseOperate::getUSBDir(m_usbPath,list);
        m_importFunction->getUsbPathAndDir(m_usbPath,list);
    }
}

void ImportExport::on_pbt_export_clicked()
{
    //检测优盘
    if(detectUSB())     //检测到U盘
    {
        ui->stackWidget_functionContainer->setCurrentWidget(ui->page_export);
        QStringList list;
        CBaseOperate::getUSBDir(m_usbPath,list);
        m_exportFunction->getUsbPathAndDir(m_usbPath,list);
    }
}

void ImportExport::slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag)
{
    switch(okFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    default:
        break;
    }
}

void ImportExport::slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag)
{
    switch(cancleFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    default:
        break;
    }
}

void ImportExport::initWidget()
{
    m_importFunction = new ImportFunction(m_requestDataThread,this);
//    connect(this,SIGNAL(signal_sendInitDataBaseIsSuccess(FileInfoClassList)),m_importFunction,SLOT(slot_InitDataBaseIsSuccess(FileInfoClassList)));
    ui->gridLayout_import->addWidget(m_importFunction);

    m_exportFunction = new ExportFunction(m_requestDataThread,this);
    ui->gridLayout_export->addWidget(m_exportFunction);
}

bool ImportExport::detectUSB()
{
    m_usbPath.clear();
    if(!CBaseOperate::getUSBMountPath(m_usbPath)) //成功
    {
        m_customMsgBox->setLabelContent(tr("硬件设备连接成功!"));
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
        return true;
    }else   // 失败
    {
        m_customMsgBox->setLabelContent(tr("找不到硬件设备,请重新连接设备!"));
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
        return false;
    }
}
