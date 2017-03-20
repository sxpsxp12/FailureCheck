#include "importfunction.h"
#include "ui_importfunction.h"
#include "enumtypeheader.h"
#include "filesoperate.h"
#include <QTimer>
#include "databasesoperate.h"
#include <QDebug>
#include <QCoreApplication>
#include <QDesktopWidget>

ImportFunction::ImportFunction(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportFunction)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;
    m_customMsgBox = new CustomMsgBox;
    m_customMsgBox->move((QApplication::desktop()->width() - this->width())/2,
         (QApplication::desktop()->height() - this->height())/2);
    m_customMsgBox->setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    m_importThread = new ImportThread;

    m_msgLabelTimer = new QTimer;
    connect(m_msgLabelTimer,SIGNAL(timeout()),this,SLOT(slot_timerTimeOut()));

    connect(m_customMsgBox,SIGNAL(signal_sendOkFunctionFlag(int)),this,SLOT(slot_customMsgBoxOkFunctionFlag(int)));
    connect(m_customMsgBox,SIGNAL(signal_sendCancleFunctionFlag(int)),this,SLOT(slot_customMsgBoxCancleFunctionFlag(int)));
    connect(m_importThread, SIGNAL(finished()), this, SLOT(slot_importThreadFinished()));
}

ImportFunction::~ImportFunction()
{
    if(m_customMsgBox != NULL)
    {
        delete m_customMsgBox;
    }

    delete ui;
}

void ImportFunction::getUsbPathAndDir(const QString &path, const QStringList &dir)
{
    m_usbPath = path;
    m_usbDir = dir;
}

void ImportFunction::on_pbt_commit_clicked()
{
    QString goalDir;
    QString goalPath;

    QString locomotive = ui->cbx_locomotive->currentText();
    QString verhicleType = ui->cbx_verhicleType->currentText();
    QString verhicelNum = ui->lineEdit_verhicelNum->text();
    //QString verhicelNum = "CT";
    QString password = ui->lineEdit_password->text();
    //QString password = "12344321";
    if(!locomotive.isEmpty() && !verhicleType.isEmpty() && !verhicelNum.isEmpty() && !password.isEmpty())
    {
        //密码正确
        if(password.compare(PASSWORD) == 0)
        {
            if(locomotive == "沈阳")
            {
                goalDir.append("SY-");
            }else if(locomotive == "北京")
            {
                goalDir.append("BJ-");
            }else if(locomotive == "广州")
            {
                goalDir.append("GZ-");
            }else if(locomotive == "上海")
            {
                goalDir.append("SH-");
            }else if(locomotive == "太原")
            {
                goalDir.append("TY-");
            }else if(locomotive == "邯郸")
            {
                goalDir.append("HD-");
            }else if(locomotive == "郑州")
            {
                goalDir.append("ZZ-");
            }else if(locomotive == "重庆")
            {
                goalDir.append("CQ-");
            }

            goalDir.append(verhicleType);

            //qDebug() <<  "m_usbDir"<< m_usbDir;
            //qDebug() << "goalDir" << goalDir;
            if(m_usbDir.contains(QString(goalDir)))
            {
                goalPath.clear();
                goalPath.append(m_usbPath);
                goalPath.append("/");
                goalPath.append(goalDir);
                //qDebug() << "goalPath" << goalPath;

                //判断U盘中的文件夹是否正确，包括文件个数，文件名
                if(CBaseOperate::checkImportPathFile(goalPath))
                {
//                    m_requestDataThread->setUSBGoalPath(goalPath);

                    //判断都正确，进行数据导入，首先停止向主机请求数据
                    m_requestDataThread->stopRequestData();
                    //将文件路径传给导入到出线程
                    m_importThread->setGoalPath(goalPath);
                    //启动导入到出线程
                    m_importThread->start();

                    //开启定时器
                    m_msgLabelTimer->start(1000);

                    m_customMsgBox->setTitle(tr("提示"));
                    m_customMsgBox->setLabelContent(tr("请稍候,导入中 ."));
                    m_customMsgBox->setOkFunctionFlag(0);
                    m_customMsgBox->setCancleFunctionFlag(0);
                    m_customMsgBox->setPushButtonName("确定","取消");
                    m_customMsgBox->setPushButtonEnable(false,false);
                    m_customMsgBox->setOkPushButtonHide(true);
                    m_customMsgBox->setCanclePushButtonHide(true);
                    m_customMsgBox->show();
                }else   //判断出错，显示出错信息
                {
                    m_customMsgBox->setTitle(tr("提示"));
                    m_customMsgBox->setLabelContent(tr("目标文件夹不满足配置要求！"));
                    m_customMsgBox->setPushButtonEnable(true,true);
                    m_customMsgBox->setPushButtonName("确定","取消");
                    m_customMsgBox->setOkFunctionFlag(0);
                    m_customMsgBox->setCancleFunctionFlag(0);
                    m_customMsgBox->setOkPushButtonHide(false);
                    m_customMsgBox->setCanclePushButtonHide(false);
                    m_customMsgBox->show();
                }

            }else
            {
                m_customMsgBox->setTitle(tr("提示"));
                m_customMsgBox->setLabelContent(tr("请确保配置文件夹存在并且车型车号正确!"));
                m_customMsgBox->setPushButtonEnable(true,true);
                m_customMsgBox->setPushButtonName("确定","取消");
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
            m_customMsgBox->setPushButtonEnable(true,true);
            m_customMsgBox->setPushButtonName("确定","取消");
            m_customMsgBox->setOkFunctionFlag(0);
            m_customMsgBox->setCancleFunctionFlag(0);
            m_customMsgBox->setOkPushButtonHide(false);
            m_customMsgBox->setCanclePushButtonHide(false);
            m_customMsgBox->show();
        }
    }else
    {
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setLabelContent(tr("请确保配置不为空!"));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
    }
}

void ImportFunction::slot_InitDataBaseIsSuccess(FileInfoClassList &fileInfoList)
{
    m_msgLabelTimer->stop();
    //导入成功
    if(fileInfoList.size() == 0)
    {
        QMap<QString,QString> mapVerhicleInfo;
        mapVerhicleInfo.clear();
        //成功后将车辆信息插入到数据库
        mapVerhicleInfo["locomotive"] = ui->cbx_locomotive->currentText();
        mapVerhicleInfo["verhicle_type"] = ui->cbx_verhicleType->currentText();
        mapVerhicleInfo["verhicle_num"] = ui->lineEdit_verhicelNum->text();
        DatabasesOperate::insertVerhicleInfoToDataBase(mapVerhicleInfo);

        m_customMsgBox->setLabelContent(tr("文件导入完成，请拔出硬件设备,系统将重启!"));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(1);
        m_customMsgBox->setCancleFunctionFlag(1);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
        m_requestDataThread->startRequestData();
    }else
    {
        QString content;
        QString rowNum;
        foreach (FileInfoClass fileInfo, fileInfoList) {
            if(!fileInfo.getInitDataBaseOk())
            {
                content.append(QString("%1:%2\n或").arg(fileInfo.geterrorFileName(),"重复数据出错!"));
            }
            //文件格式不正确
            if(!fileInfo.getFileFormatOk())
            {
                foreach (int row, fileInfo.geterrorRowNumList()) {
                    rowNum.append(QString::number(row));
                    rowNum.append(",");
                }
                content.append(QString("%1:%2\n或").arg(fileInfo.geterrorFileName(),rowNum));
            }
            QCoreApplication::processEvents();
        }
        m_customMsgBox->setTitle(tr("导入失败，文件错误!"));
        m_customMsgBox->setLabelContent(content.left(content.length()-content.right(2).length()));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(0);
        m_customMsgBox->setCancleFunctionFlag(0);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
    }
}

void ImportFunction::slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag)
{
    switch(okFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    case 1:
        CBaseOperate::rebootSystem();     //重启系统
        m_customMsgBox->hide();
        break;
    default:
        break;
    }
}

void ImportFunction::slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag)
{
    switch(cancleFunctionFlag)
    {
    case 0:
        m_customMsgBox->hide();
        break;
    case 1:
        m_customMsgBox->setTitle(tr("提示"));
        m_customMsgBox->setLabelContent(tr("请重启执行配置!"));
        m_customMsgBox->setPushButtonEnable(true,true);
        m_customMsgBox->setPushButtonName("确定","取消");
        m_customMsgBox->setOkFunctionFlag(1);
        m_customMsgBox->setCancleFunctionFlag(1);
        m_customMsgBox->setOkPushButtonHide(false);
        m_customMsgBox->setCanclePushButtonHide(false);
        m_customMsgBox->show();
        break;
    default:
        break;
    }
}

void ImportFunction::slot_importThreadFinished()
{
    FileInfoClassList errorFileInfoList;
    errorFileInfoList = m_importThread->getErrorFileInfoList();

#if 0
    //文档都正确
    if(errorFileInfoList.size()==0)
    {
        //重新初始化列表
        m_requestDataThread->initdDataAnalysis();
    }
#endif

    slot_InitDataBaseIsSuccess(errorFileInfoList);
}

void ImportFunction::slot_timerTimeOut()
{
    static int pointCount = 0;

    pointCount++;
    pointCount = pointCount%3;

    switch(pointCount)
    {
    case 0:
        m_customMsgBox->setLabelContent(tr("请稍候,导入中 ."));
        break;
    case 1:
        m_customMsgBox->setLabelContent(tr("请稍候,导入中 . ."));
        break;
    case 2:
        m_customMsgBox->setLabelContent(tr("请稍候,导入中 . . ."));
        break;
    default:break;
    }
}
