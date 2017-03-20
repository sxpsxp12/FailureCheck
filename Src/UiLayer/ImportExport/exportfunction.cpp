#include "exportfunction.h"
#include "ui_exportfunction.h"
#include <QDebug>
#include <QDateTime>
#include "enumtypeheader.h"
#include "databasesoperate.h"
#include "logfile.h"
#include <QDesktopWidget>
#include "cbaseoperate.h"
#include <QDesktopWidget>

ExportFunction::ExportFunction(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportFunction)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;
    m_customMsgBox = new CustomMsgBox;
    m_customMsgBox->move((QApplication::desktop()->width() - this->width())/2,
         (QApplication::desktop()->height() - this->height())/2);
    m_customMsgBox->setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
    m_exportThread = new ExportThread;
    m_msgLabelTimer = new QTimer;
    connect(m_msgLabelTimer,SIGNAL(timeout()),this,SLOT(slot_timerTimeOut()));

    connect(m_exportThread, SIGNAL(finished()), this, SLOT(slot_exportThreadFinished()));
    connect(m_customMsgBox,SIGNAL(signal_sendOkFunctionFlag(int)),this,SLOT(slot_customMsgBoxOkFunctionFlag(int)));
    connect(m_customMsgBox,SIGNAL(signal_sendCancleFunctionFlag(int)),this,SLOT(slot_customMsgBoxCancleFunctionFlag(int)));
}

ExportFunction::~ExportFunction()
{
    delete ui;

    if(m_customMsgBox != NULL)
    {
        delete m_customMsgBox;
    }

    if(m_exportThread != NULL)
    {
        delete m_exportThread;
    }
}

void ExportFunction::getUsbPathAndDir(const QString &path, const QStringList &dir)
{
    m_usbPath = path;
    m_usbDir = dir;

    //将车辆信息进行设置
    DatabasesOperate::getVerhicleInfo(m_mapVerhicleInfoKeyAndValue);

    if(m_mapVerhicleInfoKeyAndValue.size() != 0)
    {
        ui->cbx_locomotive->setCurrentIndex(ui->cbx_locomotive->findText(m_mapVerhicleInfoKeyAndValue["locomotive"]));
        ui->cbx_verhicleType->setCurrentIndex(ui->cbx_verhicleType->findText(m_mapVerhicleInfoKeyAndValue["verhicle_type"]));
        ui->lineEdit_verhicelNum->setText(m_mapVerhicleInfoKeyAndValue["verhicle_num"]);
    }
}

void ExportFunction::on_pbt_commit_clicked()
{
    QString locomotive = ui->cbx_locomotive->currentText();
    QString verhicleType = ui->cbx_verhicleType->currentText();
    QString verhicelNum = ui->lineEdit_verhicelNum->text();
    //QString verhicelNum = "CT";
    QString password = ui->lineEdit_password->text();
    //QString password = "12344321";

    m_outputPath.clear();
    QDateTime date = QDateTime::currentDateTime();
    m_outputPath.append(m_usbPath);
    m_outputPath.append("/");

    if(locomotive == "沈阳")
    {
        m_outputPath.append("shenyang");
    }else if(locomotive == "北京")
    {
        m_outputPath.append("beijing");
    }else if(locomotive == "广州")
    {
        m_outputPath.append("guangzhou");
    }else if(locomotive == "上海")
    {
        m_outputPath.append("shanghai");
    }else if(locomotive == "太原")
    {
        m_outputPath.append("taiyuan");
    }else if(locomotive == "邯郸")
    {
        m_outputPath.append("handan");
    }else if(locomotive == "郑州")
    {
        m_outputPath.append("zhengzhou");
    }else if(locomotive == "重庆")
    {
        m_outputPath.append("chongqin");
    }

   // m_outputPath.append(locomotive);

    m_outputPath.append("-");
    m_outputPath.append(verhicleType);
    m_outputPath.append("-");
    m_outputPath.append(verhicelNum);
    m_outputPath.append("-");
    m_outputPath.append(date.toString("yyyy-MM-dd-hh-mm-ss"));

    if(!locomotive.isEmpty() && !verhicleType.isEmpty() && !verhicelNum.isEmpty() && !password.isEmpty())
    {
        //密码正确
        if(password.compare(PASSWORD) == 0)
        {
            //删除密码
            ui->lineEdit_password->setText("");
            //为故障记录创建目录
            if(CBaseOperate::createFileDir(m_outputPath))
            {
                //选择导入最新的数据和上次的数据
                m_customMsgBox->setTitle(tr("提示"));
                m_customMsgBox->setLabelContent(tr("请选择导出的数据!"));
                m_customMsgBox->setPushButtonName("上一次数据","最新数据");
                m_customMsgBox->setPushButtonEnable(true,true);
                m_customMsgBox->setOkFunctionFlag(1);
                m_customMsgBox->setCancleFunctionFlag(1);
                m_customMsgBox->setOkPushButtonHide(false);
                m_customMsgBox->setCanclePushButtonHide(false);
                m_customMsgBox->show();

            }else
            {
#if DEBUG
                qDebug() << "创建文件夹出错";
#endif
#if LOG
                LogFile::debug("创建文件夹出错");
#endif
                //选择导入最新的数据和上次的数据
                m_customMsgBox->setTitle(tr("提示"));
                m_customMsgBox->setLabelContent(tr("创建目录失败,请确保U盘已经插入!!!"));
                m_customMsgBox->setPushButtonName("确定","取消");
                m_customMsgBox->setPushButtonEnable(true,true);
                m_customMsgBox->setOkFunctionFlag(0);
                m_customMsgBox->setCancleFunctionFlag(0);
                m_customMsgBox->setOkPushButtonHide(false);
                m_customMsgBox->setCanclePushButtonHide(false);
                m_customMsgBox->show();
            }
        }else
        {
            m_customMsgBox->setTitle(tr("提示"));
            m_customMsgBox->setLabelContent(tr("密码错误，请重新输入!"));
            m_customMsgBox->setOkFunctionFlag(0);
            m_customMsgBox->setPushButtonEnable(true,true);
            m_customMsgBox->setCancleFunctionFlag(0);
            m_customMsgBox->setOkPushButtonHide(false);
            m_customMsgBox->setCanclePushButtonHide(false);
            m_customMsgBox->show();
        }
    }else
    {
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setLabelContent(tr("请确保配置不为空!"));
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
    }
}

void ExportFunction::slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag)
{
    switch(okFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    case 1:
        m_msgLabelTimer->start(1000);
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setLabelContent(tr("请稍候,正在导出 ."));
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setPushButtonEnable(false,false);
        m_customMsgBox->setOkPushButtonHide(true);
        m_customMsgBox->setCanclePushButtonHide(true);
        m_customMsgBox->show();

        //开启线程导出数据
        m_exportThread->setOutputPath(m_outputPath);
        m_exportThread->isExportNewData(false);
        m_exportThread->start();

        break;
    default:
        break;
    }
}

void ExportFunction::slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag)
{
    switch(cancleFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    case 1:
        m_msgLabelTimer->start(1000);

        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setLabelContent(tr("请稍候,正在导出 ."));
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setPushButtonEnable(false,false);
        m_customMsgBox->setOkPushButtonHide(true);
        m_customMsgBox->setCanclePushButtonHide(true);
        m_customMsgBox->show();

        //开启线程导出数据
        m_exportThread->setOutputPath(m_outputPath);
        m_exportThread->isExportNewData(true);
        m_exportThread->start();
        break;
    default:
        break;
    }
}

void ExportFunction::slot_exportThreadFinished()
{
    m_msgLabelTimer->stop();

    m_customMsgBox->setTitle(tr("提示"));
    m_customMsgBox->setLabelContent(tr("文件导出完成，请拔出硬件设备!"));
    m_customMsgBox->setPushButtonName("确定","取消");
    m_customMsgBox->setOkFunctionFlag(0);
    m_customMsgBox->setCancleFunctionFlag(0);
    m_customMsgBox->setOkPushButtonHide(false);
    m_customMsgBox->setCanclePushButtonHide(false);
    m_customMsgBox->setPushButtonEnable(true,true);
    m_customMsgBox->show();
}

void ExportFunction::slot_timerTimeOut()
{
    static int pointCount = 0;
    pointCount++;
    pointCount = pointCount%3;
    switch(pointCount)
    {
    case 0:
        m_customMsgBox->setLabelContent(tr("请稍候,正在导出 ."));
        break;
    case 1:
        m_customMsgBox->setLabelContent(tr("请稍候,正在导出 . ."));
        break;
    case 2:
        m_customMsgBox->setLabelContent(tr("请稍候,正在导出 . . ."));
        break;
    default:break;
    }
}
