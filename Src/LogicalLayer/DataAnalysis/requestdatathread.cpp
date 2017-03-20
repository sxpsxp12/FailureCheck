#include "requestdatathread.h"

#include "enumtypeheader.h"
#include "dataanalysis.h"
#include "databasesoperate.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QString>
#include <QTime>
#include <qstringlist.h>
#include "canoperate.h"
#include "importexport.h"
#include "filesoperate.h"
#include "logfile.h"

int RequestDataThread::CurrentInterfaceEnumValue = PRIMARY_INTERFACE;
MapAddressAndCanBusDataPackage RequestDataThread::m_mapAddressAndCanBusDataPackage;

RequestDataThread::RequestDataThread(QObject *parent) :
    QThread(parent)
{
    m_dataAnalysis = new DataAnalysis;

    initParameter();

    canInit();
}

RequestDataThread::~RequestDataThread()
{
    if(m_dataAnalysis != NULL)
    {
        delete m_dataAnalysis;
    }

    if(m_timerRequestBusData != NULL)
    {
        delete m_dataAnalysis;
    }
}

void RequestDataThread::startRequestData()
{
    canInit();
}

void RequestDataThread::stopRequestData()
{
    canClose();
    initParameter();

//    m_isRequestData = false;
//    m_isImportTxt = true;
}
#if 0
void RequestDataThread::setUSBGoalPath(const QString &goalPath)
{
    m_goalPath = goalPath;
}
#endif

void RequestDataThread::run()
{
    m_timerRequestBusData = new QTimer;
    m_timerRequestBusData->start(REQUEST_BUS_DATA_TIME_INTERVAL);
    connect(m_timerRequestBusData, SIGNAL(timeout()),
            this, SLOT(slot_useThreadResources()), Qt::DirectConnection);
    this->exec();
}

void RequestDataThread::slot_useThreadResources()
{
#if LOGW
        LogFile::debug("into slot_useThreadResources()");
#endif

    static int CheckMasterTime=0;

    //请求数据
    if (!m_isCanOpened)
    {
        //没有打开can，失败
        emit signal_sendGUIContainerTitleBarStatus(HOST_TITLEBAR,FAILE_STATUS);
        emit signal_sendGUIContainerTitleBarStatus(LCU_TITLEBAR,FAILE_STATUS);
        emit signal_sendGUIContainerTitleBarStatus(DCU_TITLEBAR,FAILE_STATUS);
        emit signal_sendGUIContainerTitleBarStatus(DCU2_TITLEBAR,FAILE_STATUS);

        //重新初始化can
        canInit();
        return;
    }

    //未验证是否是主机
    if( !m_isRequestMasterMachine )
    {
        //qDebug() << "进入主机判断";
        m_isRequestMasterMachine = true;
        //在请求数据之前先进行主从判断
        if( !CBaseOperate::isMasterMachine(m_isMasterMachine) )
        {
#if DEBUG
            qDebug() << "主从机判断失败";
#endif
#if LOG
            LogFile::debug("主从机判断失败");
#endif
        }else       //验证成功
        {
            emit signal_sendMasterFlag(m_isMasterMachine);
        }
    }

    //是主机才去请求数据
    if(m_isMasterMachine)
    {
        //qDebug() << "判断是主机";
        canWriteData();
    }

    //从机只负责读取
    canReadData();

    if(CheckMasterTime++>25)
    {
      m_isRequestMasterMachine=false;
      CheckMasterTime=0;
    }

#if 0
    //导入txt文档
    if(m_isImportTxt)
    {
        m_isImportTxt = false;
        //清空数据库
        if(!DatabasesOperate::clearDataBaseAllTables())
        {
#if DEBUG
            qDebug() << "清表错误";
#endif
#if LOG
            LogFile::debug("清表错误");
#endif
        }
        //开始导入txt文档
        //init数据库
        FilesOperate *filesOperate = new FilesOperate(m_goalPath);
        //成功初始化
        FileInfoClassList fileInfoList;
        FileInfoClassList errorFileInfoList;
        errorFileInfoList.clear();
        fileInfoList.clear();

        fileInfoList = filesOperate->initDatabases();
        foreach (FileInfoClass fileInfo, fileInfoList) {
            //将出错的文档进行记录
            if(!fileInfo.getFileFormatOk() || !fileInfo.getInitDataBaseOk())
            {
                errorFileInfoList.append(fileInfo);
            }
        }
        //文档都正确
        if(errorFileInfoList.size()==0)
        {
            //重新初始化列表
            m_dataAnalysis->dataAnalysisInit();
        }
        emit signal_sendInitDataBaseIsSuccess(errorFileInfoList);
    }
#endif

#if LOGW
        LogFile::debug("out slot_useThreadResources()");
#endif

}

void RequestDataThread::initParameter()
{
    m_isMasterMachine = true;      //主从机标志
    m_isRequestMasterMachine = false;   //默认为未验证是否是主机

    m_isHandshakeSuccess = false;
    m_handshakeTime = 0;
    m_requestDataSuccessTime = 0;
    m_againRequestDataFault = 0;    //再次请求数据发生错误

    //    m_isRequestData = true;     //默认请求数据
    //    m_isImportTxt = false;      //导入txt文档

    m_LastLCUDataFlag = -1;     //LCU数据标志
    m_LastDCUDataFlag = -1;     //DCU数据标志
    m_LastDCU2DataFlag = -1;     //DCU2数据标志
    m_LastHostDataFlag = -1;     //主机数据标志
    m_LCUAbnormalTime = 0;             //LCU异常次数
    m_DCUAbnormalTime = 0;         //DCU异常次数
    m_DCU2AbnormalTime = 0;         //DCU2异常次数
    m_HostAbnormalTime = 0;         //主机异常次数
}

/***
  **设置容器的标题栏
  **先判断控制主机的通讯，然后判断LCU、微机柜（一架）、微机柜（二架）
**/
void RequestDataThread::setGuiContainerTitleBarStatus()
{
    bool isDataFlagSame = true;     //判断数据是否一样
    bool isDataSizeSame = true; //判断数据大小是否一样
    bool isCheckSumOk = true;   //判断校验和是否一样

    int lcuCurrentDataFlag = -1;     //LCU数据标志
    int lcuCurrentDataSize = 0;      //LCU数据大小
    int lcuCurrentDataCheckSum = 0;    //LCU数据校验和
    int dcuCurrentDataFlag = -1;     //微机柜数据标志
    int dcuCurrentDataSize = 0;      //微机柜数据大小
    int dcuCurrentDataCheckSum = 0;    //微机柜数据校验和
    int dcu2CurrentDataFlag = -1;     //微机柜2数据标志
    int dcu2CurrentDataSize = 0;      //微机柜2数据大小
    int dcu2CurrentDataCheckSum = 0;    //微机柜2数据校验和
    int hostCurrentDataFlag = -1;     //主机数据标志
    int hostCurrentDataSize = 0;      //主机数据大小
    int hostCurrentDataCheckSum = 0;    //主机数据校验和

    //遍历所有的数据,求主机 、LCU、微机柜（一架）、微机柜（二架）的数据大小和校验和
    for(int i = 0;i < m_mapAddressAndCanBusDataPackage.keys().size();i++)
    {
        switch(m_mapAddressAndCanBusDataPackage.value(i).getCanBusDataIDStr().toInt(0,16))
        {
        case LCU_CANDATA_ID:
            //计算LCU的数据的个数
            lcuCurrentDataSize++;
            if(i != 47)
            {
                //除去校验位所有的数据的和
                lcuCurrentDataCheckSum += m_mapAddressAndCanBusDataPackage.value(i).getDecNumber();
            }

            break;
        case DCU_CANDATA_ID:
            dcuCurrentDataSize++;
            if(i != 103)
            {
                dcuCurrentDataCheckSum += m_mapAddressAndCanBusDataPackage.value(i).getDecNumber();
            }

            break;
        case DCU2_CANDATA_ID:
            dcu2CurrentDataSize++;
            if(i != 159)
            { dcu2CurrentDataCheckSum += m_mapAddressAndCanBusDataPackage.value(i).getDecNumber();

            }

            break;
        case HOST_CANDATA_ID:
            hostCurrentDataSize++;
            if(i != 215)
            {
                hostCurrentDataCheckSum += m_mapAddressAndCanBusDataPackage.value(i).getDecNumber();
            }

            break;
        default:break;
        }
    }
    //获取主机 、LCU、微机柜（一架）、微机柜（二架）的数据标志
    lcuCurrentDataFlag = m_mapAddressAndCanBusDataPackage.value(46).getDecNumber();
    dcuCurrentDataFlag = m_mapAddressAndCanBusDataPackage.value(102).getDecNumber();
    dcu2CurrentDataFlag = m_mapAddressAndCanBusDataPackage.value(158).getDecNumber();
    hostCurrentDataFlag = m_mapAddressAndCanBusDataPackage.value(214).getDecNumber();

    //首先判断主机
    if(m_LastHostDataFlag == hostCurrentDataFlag)
    {
        isDataFlagSame = true;
    }else
    {
        isDataFlagSame = false;
    }

    if(hostCurrentDataSize == HOST_CANDATA_Size)
    {
        isDataSizeSame = true;
    }else
    {
        isDataSizeSame = false;
    }

    if( !(QString::number((~hostCurrentDataCheckSum+1),16)).right(2).compare(m_mapAddressAndCanBusDataPackage.value(215).getHexNumberStr()) )
    {
        isCheckSumOk = true;
    }else
    {
        isCheckSumOk = false;
    }

    //三种情况  未知，异常，正常
    if(!isDataFlagSame && isDataSizeSame && isCheckSumOk)
    {
        m_HostAbnormalTime = 0;
        //正常
        emit signal_sendGUIContainerTitleBarStatus(HOST_TITLEBAR,NORMAL_STATUS);
    }else if(isDataFlagSame || !isDataSizeSame || !isCheckSumOk)
    {
        m_HostAbnormalTime++;
        //异常时连续判断三次
        if(m_HostAbnormalTime % 10 == 0)
        {
            m_HostAbnormalTime = 0;
            //显示异常
            emit signal_sendGUIContainerTitleBarStatus(HOST_TITLEBAR,ABNORMAL_STATUS);
        }
    }

    //微机柜一架
    if(m_LastDCUDataFlag == dcuCurrentDataFlag)
    {
        isDataFlagSame = true;
    }else
    {
        isDataFlagSame = false;
    }

    if(dcuCurrentDataSize == DCU_CANDATA_Size)
    {
        isDataSizeSame = true;
    }else
    {
        isDataSizeSame = false;
    }

    if( !(QString::number((~dcuCurrentDataCheckSum+1),16)).right(2).compare(m_mapAddressAndCanBusDataPackage.value(103).getHexNumberStr()) )
    {
        isCheckSumOk = true;
    }else
    {
        isCheckSumOk = false;
    }

    //三种情况  未知，异常，正常
    if(!isDataFlagSame && isDataSizeSame && isCheckSumOk)
    {
        m_DCUAbnormalTime = 0;
        //正常
        emit signal_sendGUIContainerTitleBarStatus(DCU_TITLEBAR,NORMAL_STATUS);
    }else if(isDataFlagSame  || !isDataSizeSame || !isCheckSumOk)
    {
        m_DCUAbnormalTime++;
        //异常时连续判断三次
        if(m_DCUAbnormalTime % 10 == 0)
        {
            m_DCUAbnormalTime = 0;
            //显示异常
            emit signal_sendGUIContainerTitleBarStatus(DCU_TITLEBAR,ABNORMAL_STATUS);
        }
    }

    //微机柜二架
    if(m_LastDCU2DataFlag == dcu2CurrentDataFlag)
    {
        isDataFlagSame = true;
    }else
    {
        isDataFlagSame = false;
    }

    if(dcu2CurrentDataSize == DCU_CANDATA_Size)
    {
        isDataSizeSame = true;
    }else
    {
        isDataSizeSame = false;
    }

    if( !(QString::number((~dcu2CurrentDataCheckSum+1),16)).right(2).compare(m_mapAddressAndCanBusDataPackage.value(159).getHexNumberStr()) )
    {
        isCheckSumOk = true;
    }else
    {
        isCheckSumOk = false;
    }
    //三种情况  未知，异常，正常
    if(!isDataFlagSame && isDataSizeSame && isCheckSumOk)
    {
        m_DCU2AbnormalTime = 0;
        //正常
        emit signal_sendGUIContainerTitleBarStatus(DCU2_TITLEBAR,NORMAL_STATUS);
    }else if(isDataFlagSame  || !isDataSizeSame || !isCheckSumOk)
    {
        m_DCU2AbnormalTime++;
        //异常时连续判断三次
        if(m_DCU2AbnormalTime % 10 == 0)
        {
            m_DCU2AbnormalTime = 0;
            //显示异常
            emit signal_sendGUIContainerTitleBarStatus(DCU2_TITLEBAR,ABNORMAL_STATUS);
        }
    }

    //LCU
    if(m_LastLCUDataFlag == lcuCurrentDataFlag)
    {
        isDataFlagSame = true;
    }else
    {
        isDataFlagSame = false;
    }

    if(lcuCurrentDataSize == LCU_CANDATA_Size)
    {
        isDataSizeSame = true;
    }else
    {
        isDataSizeSame = false;
    }

    if( !(QString::number((~lcuCurrentDataCheckSum+1),16)).right(2).compare(m_mapAddressAndCanBusDataPackage.value(47).getHexNumberStr()) )
    {
        isCheckSumOk = true;
    }else
    {
        isCheckSumOk = false;
    }

    //三种情况  未知，异常，正常
    if(!isDataFlagSame && isDataSizeSame && isCheckSumOk)
    {
        //正常
        emit signal_sendGUIContainerTitleBarStatus(LCU_TITLEBAR,NORMAL_STATUS);
        m_LCUAbnormalTime = 0;
    }else if(isDataFlagSame  || !isDataSizeSame || !isCheckSumOk)
    {
        m_LCUAbnormalTime++;
        //异常时连续判断三次
        if(m_LCUAbnormalTime % 10 == 0)
        {
            m_LCUAbnormalTime = 0;
            //显示异常
            emit signal_sendGUIContainerTitleBarStatus(LCU_TITLEBAR,ABNORMAL_STATUS);
        }
    }

    //记录当前的数据标志
    m_LastHostDataFlag = hostCurrentDataFlag;
    m_LastDCUDataFlag = dcuCurrentDataFlag;
    m_LastDCU2DataFlag = dcu2CurrentDataFlag;
    m_LastLCUDataFlag = lcuCurrentDataFlag;
}

void RequestDataThread::canInit()
{
#if SERIAL_CAN
    //初始化总线
    if(CanOperate::InitCan())
    {
#if DEBUG
        qDebug() << "can open success";
#endif
#if LOG
        LogFile::debug("can open success");
#endif
        m_isCanOpened = true;
    }else
    {
#if DEBUG
        qDebug() << "can open fail";
#endif
#if LOG
        LogFile::debug("can open fail");
#endif
        m_isCanOpened = false;
    }
#endif

#if SOCKET_CAN
    if(CanOperate::openCanSocket())
    {
#if DEBUG
        qDebug() << "socket open success";
#endif
#if LOG
        LogFile::debug("socket open success");
#endif
        m_isCanOpened = true;
    }else
    {
#if DEBUG
        qDebug() << "socket open fail";
#endif
#if LOG
        LogFile::debug("socket open fail");
#endif
        m_isCanOpened = false;
    }

#endif
}

void RequestDataThread::canReadData()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    QString canData;
    bool isReadCanHandshakeSuccess = false;
    bool isReadCanRequestDataSuccess = false;

#if SOCKET_CAN      //socket can读

    if(!m_isMasterMachine)      //是主机，将请求握手的标志置true，只读27侦事实数据
    {
        m_isHandshakeSuccess = true;
        m_handshakeTime = 0;
    }

    //握手
    if(!m_isHandshakeSuccess)
    {
        //  读握手数据
        isReadCanHandshakeSuccess = CanOperate::readCanHandshake(canData);
        //握手失败
        if(!isReadCanHandshakeSuccess)
        {
            m_handshakeTime++;
            if (m_handshakeTime * REQUEST_BUS_DATA_TIME_INTERVAL >= REQUEST_BUS_DATA_TIMEOUT)
            {
                //一级界面状态栏写为:失败(红色)
                m_handshakeTime = 0;
                m_isHandshakeSuccess = false;  //重新开始握手
                emit signal_sendGUIContainerTitleBarStatus(HOST_TITLEBAR,FAILE_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(LCU_TITLEBAR,FAILE_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(DCU_TITLEBAR,FAILE_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(DCU2_TITLEBAR,FAILE_STATUS);
            }
        }
    }else   //请求数据
    {
        //读实时数据
        isReadCanRequestDataSuccess = CanOperate::readCanRequestData(canData,m_isMasterMachine);
        //请求数据失败
        if(!isReadCanRequestDataSuccess)
        {
            m_requestDataSuccessTime++;
            if (m_requestDataSuccessTime * REQUEST_BUS_DATA_TIME_INTERVAL >= REQUEST_BUS_DATA_TIMEOUT)
            {
                //请求数据超时  一级界面写为：未知(红色)
                m_requestDataSuccessTime = 0;
                m_againRequestDataFault++;
                if(m_againRequestDataFault == 2)
                {
                    m_handshakeTime = 0;
                    m_againRequestDataFault = 0;
                    m_isHandshakeSuccess = false;  //重新开始握手
                    return;
                }
                emit signal_sendGUIContainerTitleBarStatus(HOST_TITLEBAR,UNKNOWN_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(LCU_TITLEBAR,UNKNOWN_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(DCU_TITLEBAR,UNKNOWN_STATUS);
                emit signal_sendGUIContainerTitleBarStatus(DCU2_TITLEBAR,UNKNOWN_STATUS);
            }
        }
    }

#endif

    /*读取到 握手响应  FF FF FF FF FF FF FF FF*/
    if (isReadCanHandshakeSuccess && canData.compare("ffffffffffffffff") == 0)
    {
        isReadCanHandshakeSuccess = false;
        m_isHandshakeSuccess = true;
        m_handshakeTime = 0;
    }else if (isReadCanRequestDataSuccess && m_isHandshakeSuccess && !canData.isEmpty())
    {
        isReadCanRequestDataSuccess = false;
        m_requestDataSuccessTime = 0;
        m_againRequestDataFault = 0;

       // qDebug() << "请求数据成功;" << m_mapAddressAndCanBusDataPackage.count();
        //初始化Can数据
        //InitCanData(canData);

        //记录实时数据，用于记录故障前后一段时间的实时数据
        m_dataAnalysis->recordRealData(dateTime,canData);
        //接收到数据以后首先设置容器的标题栏
        setGuiContainerTitleBarStatus();

        //解析27帧实时数据
        //需要判断每个设备的正常还是异常
        m_dataAnalysis->dataAnalysis(dateTime,canData);
        // 每次解析完数据都得将故障历史记录推到界面中
        //        emit signal_sendFailureRecordWidgetData(m_dataAnalysis->getFailureHistoryRecordInfoList());
        emit signal_sendFailureRecordWidgetData();

        switch(CurrentInterfaceEnumValue)
        {
        case PRIMARY_INTERFACE:

            emit signal_sendPrimaryWidgetData();
            //emit signal_sendPrimaryWidgetData(m_dataAnalysis->getPrimaryWidgetRealtimeDataList(),
            //                                  m_dataAnalysis->getPrimaryWidgetFailureInfoList());
            break;
        case SECONDARY_INTERFACE:
            emit signal_sendSecondaryWidgetData();
            //            emit signal_sendSecondaryWidgetData(m_dataAnalysis->getSecondaryWidgetRealtimeDataList(),
            //                                                m_dataAnalysis->getSecondaryWidgetFailureInfoList());
            break;

        case BUS_ID_INTERFACE:
            emit signal_sendBusIdWidgetData();
            //            emit signal_sendBusIdWidgetData(m_dataAnalysis->getBusIdWidgetRealtimeDataList());

            break;
        case INTERFACE_LAMP_HOLE:
            emit signal_sendInterfaceLampHoleWidgetData();
            //            emit signal_sendInterfaceLampHoleWidgetData(
            //                        m_dataAnalysis->getInterfaceLampHoleWidgetRealtimeDataList());
            break;

        case DCU:
        case DCU2:
            emit signal_sendDcuWidgetData();
            //            emit signal_sendDcuWidgetData(m_dataAnalysis->getDcuWidgetRealtimeDataList());
            break;

        case GCU:
            emit signal_sendGcuWidgetData();
            //            emit signal_sendGcuWidgetData(m_dataAnalysis->getGcuWidgetRealtimeDataList());
            break;

        case LG:
            emit signal_sendLgWidgetData();
            //emit signal_sendLgWidgetData(m_dataAnalysis->getLgWidgetRealtimeDataList());
            break;

        case LADDER_DIAGRAM:
            emit signal_sendLadderDiagramWidgetData();
            //            emit signal_sendLadderDiagramWidgetData(m_dataAnalysis->getLadderDiagramPartStateInfoList());
        default:
            break;
        }
    }
}

void RequestDataThread::canWriteData()
{
    if (!m_isHandshakeSuccess)
    {
        //发送握手指令  FF FF FF FF FF FF FF FF
        if(CanOperate::writeCanHandshake())
        {
            //qDebug() << "写握手";
#if DEBUG
            qDebug() << "writeCanHandshake success";
#endif
#if LOG
            LogFile::debug("writeCanHandshake success");
#endif
        }else
        {
#if DEBUG
            qDebug() << "writeCanHandshake false";
#endif
#if LOG
            LogFile::debug("writeCanHandshake false");
#endif
        }
    }else
    {
        //发送4帧数据请求命令
        if(CanOperate::writeCanRequestData())
        {
            //qDebug() << "写请求数据";
#if DEBUG
            qDebug() << "writeCanRequestData success";
#endif
#if LOG
            LogFile::debug("writeCanRequestData success");
#endif
        }else
        {
#if DEBUG
            qDebug() << "writeCanRequestData false";
#endif
#if LOG
            LogFile::debug("writeCanRequestData false");
#endif
        }
    }
}

void RequestDataThread::canClose()
{
#if SOCKET_CAN
    CanOperate::closeCanSocket();
#endif
}

void RequestDataThread::InitCanData(QString &data)
{
#if 0
    QStringList dataList;
    dataList.clear();
    int len = data.length();

    QString temp;
    for(int i = 0;i < len;i+=2)
    {
        temp.clear();
        temp = data.mid(i,2);
        dataList.append(temp);
    }
#endif
#if 0
    m_mapAddressAndCanBusDataPackage.clear();
    m_mapAddressAndCanBusDataPackage[0] = CanBusDataPackageClass("0x0000006c",dataList.at(0));
    m_mapAddressAndCanBusDataPackage[1] = CanBusDataPackageClass("0x0000006c",dataList.at(1));
    m_mapAddressAndCanBusDataPackage[2] = CanBusDataPackageClass("0x0000006c",dataList.at(2));
    m_mapAddressAndCanBusDataPackage[3] = CanBusDataPackageClass("0x0000006c",dataList.at(3));
    m_mapAddressAndCanBusDataPackage[4] = CanBusDataPackageClass("0x0000006c",dataList.at(4));
    m_mapAddressAndCanBusDataPackage[5] = CanBusDataPackageClass("0x0000006c",dataList.at(5));
    m_mapAddressAndCanBusDataPackage[6] = CanBusDataPackageClass("0x0000006c",dataList.at(6));
    m_mapAddressAndCanBusDataPackage[7] = CanBusDataPackageClass("0x0000006c",dataList.at(7));

    m_mapAddressAndCanBusDataPackage[8] = CanBusDataPackageClass("0x0000006c",dataList.at(8));
    m_mapAddressAndCanBusDataPackage[9] = CanBusDataPackageClass("0x0000006c",dataList.at(9));
    m_mapAddressAndCanBusDataPackage[10] = CanBusDataPackageClass("0x0000006c",dataList.at(10));
    m_mapAddressAndCanBusDataPackage[11] = CanBusDataPackageClass("0x0000006c",dataList.at(11));
    m_mapAddressAndCanBusDataPackage[12] = CanBusDataPackageClass("0x0000006c",dataList.at(12));
    m_mapAddressAndCanBusDataPackage[13] = CanBusDataPackageClass("0x0000006c",dataList.at(13));
    m_mapAddressAndCanBusDataPackage[14] = CanBusDataPackageClass("0x0000006c",dataList.at(14));
    m_mapAddressAndCanBusDataPackage[15] = CanBusDataPackageClass("0x0000006c",dataList.at(15));

    m_mapAddressAndCanBusDataPackage[16] = CanBusDataPackageClass("0x0000006c",dataList.at(16));
    m_mapAddressAndCanBusDataPackage[17] = CanBusDataPackageClass("0x0000006c",dataList.at(17));
    m_mapAddressAndCanBusDataPackage[18] = CanBusDataPackageClass("0x0000006c",dataList.at(18));
    m_mapAddressAndCanBusDataPackage[19] = CanBusDataPackageClass("0x0000006c",dataList.at(19));
    m_mapAddressAndCanBusDataPackage[20] = CanBusDataPackageClass("0x0000006c",dataList.at(20));
    m_mapAddressAndCanBusDataPackage[21] = CanBusDataPackageClass("0x0000006c",dataList.at(21));
    m_mapAddressAndCanBusDataPackage[22] = CanBusDataPackageClass("0x0000006c",dataList.at(22));
    m_mapAddressAndCanBusDataPackage[23] = CanBusDataPackageClass("0x0000006c",dataList.at(23));

    m_mapAddressAndCanBusDataPackage[24] = CanBusDataPackageClass("0x0000006c",dataList.at(24));
    m_mapAddressAndCanBusDataPackage[25] = CanBusDataPackageClass("0x0000006c",dataList.at(25));
    m_mapAddressAndCanBusDataPackage[26] = CanBusDataPackageClass("0x0000006c",dataList.at(26));
    m_mapAddressAndCanBusDataPackage[27] = CanBusDataPackageClass("0x0000006c",dataList.at(27));
    m_mapAddressAndCanBusDataPackage[28] = CanBusDataPackageClass("0x0000006c",dataList.at(28));
    m_mapAddressAndCanBusDataPackage[29] = CanBusDataPackageClass("0x0000006c",dataList.at(29));
    m_mapAddressAndCanBusDataPackage[30] = CanBusDataPackageClass("0x0000006c",dataList.at(30));
    m_mapAddressAndCanBusDataPackage[31] = CanBusDataPackageClass("0x0000006c",dataList.at(31));

    m_mapAddressAndCanBusDataPackage[32] = CanBusDataPackageClass("0x0000006c",dataList.at(32));
    m_mapAddressAndCanBusDataPackage[33] = CanBusDataPackageClass("0x0000006c",dataList.at(33));
    m_mapAddressAndCanBusDataPackage[34] = CanBusDataPackageClass("0x0000006c",dataList.at(34));
    m_mapAddressAndCanBusDataPackage[35] = CanBusDataPackageClass("0x0000006c",dataList.at(35));
    m_mapAddressAndCanBusDataPackage[36] = CanBusDataPackageClass("0x0000006c",dataList.at(36));
    m_mapAddressAndCanBusDataPackage[37] = CanBusDataPackageClass("0x0000006c",dataList.at(37));
    m_mapAddressAndCanBusDataPackage[38] = CanBusDataPackageClass("0x0000006c",dataList.at(38));
    m_mapAddressAndCanBusDataPackage[39] = CanBusDataPackageClass("0x0000006c",dataList.at(39));

    m_mapAddressAndCanBusDataPackage[40] = CanBusDataPackageClass("0x0000006c",dataList.at(40));
    m_mapAddressAndCanBusDataPackage[41] = CanBusDataPackageClass("0x0000006c",dataList.at(41));
    m_mapAddressAndCanBusDataPackage[42] = CanBusDataPackageClass("0x0000006c",dataList.at(42));
    m_mapAddressAndCanBusDataPackage[43] = CanBusDataPackageClass("0x0000006c",dataList.at(43));
    m_mapAddressAndCanBusDataPackage[44] = CanBusDataPackageClass("0x0000006c",dataList.at(44));
    m_mapAddressAndCanBusDataPackage[45] = CanBusDataPackageClass("0x0000006c",dataList.at(45));
    m_mapAddressAndCanBusDataPackage[46] = CanBusDataPackageClass("0x0000006c",dataList.at(46));
    m_mapAddressAndCanBusDataPackage[47] = CanBusDataPackageClass("0x0000006c",dataList.at(47));

    m_mapAddressAndCanBusDataPackage[48] = CanBusDataPackageClass("0x00000045",dataList.at(48));
    m_mapAddressAndCanBusDataPackage[49] = CanBusDataPackageClass("0x00000045",dataList.at(49));
    m_mapAddressAndCanBusDataPackage[50] = CanBusDataPackageClass("0x00000045",dataList.at(50));
    m_mapAddressAndCanBusDataPackage[51] = CanBusDataPackageClass("0x00000045",dataList.at(51));
    m_mapAddressAndCanBusDataPackage[52] = CanBusDataPackageClass("0x00000045",dataList.at(52));
    m_mapAddressAndCanBusDataPackage[53] = CanBusDataPackageClass("0x00000045",dataList.at(53));
    m_mapAddressAndCanBusDataPackage[54] = CanBusDataPackageClass("0x00000045",dataList.at(54));
    m_mapAddressAndCanBusDataPackage[55] = CanBusDataPackageClass("0x00000045",dataList.at(55));

    m_mapAddressAndCanBusDataPackage[56] = CanBusDataPackageClass("0x00000045",dataList.at(56));
    m_mapAddressAndCanBusDataPackage[57] = CanBusDataPackageClass("0x00000045",dataList.at(57));
    m_mapAddressAndCanBusDataPackage[58] = CanBusDataPackageClass("0x00000045",dataList.at(58));
    m_mapAddressAndCanBusDataPackage[59] = CanBusDataPackageClass("0x00000045",dataList.at(59));
    m_mapAddressAndCanBusDataPackage[60] = CanBusDataPackageClass("0x00000045",dataList.at(60));
    m_mapAddressAndCanBusDataPackage[61] = CanBusDataPackageClass("0x00000045",dataList.at(61));
    m_mapAddressAndCanBusDataPackage[62] = CanBusDataPackageClass("0x00000045",dataList.at(62));
    m_mapAddressAndCanBusDataPackage[63] = CanBusDataPackageClass("0x00000045",dataList.at(63));

    m_mapAddressAndCanBusDataPackage[64] = CanBusDataPackageClass("0x00000045",dataList.at(64));
    m_mapAddressAndCanBusDataPackage[65] = CanBusDataPackageClass("0x00000045",dataList.at(65));
    m_mapAddressAndCanBusDataPackage[66] = CanBusDataPackageClass("0x00000045",dataList.at(66));
    m_mapAddressAndCanBusDataPackage[67] = CanBusDataPackageClass("0x00000045",dataList.at(67));
    m_mapAddressAndCanBusDataPackage[68] = CanBusDataPackageClass("0x00000045",dataList.at(68));
    m_mapAddressAndCanBusDataPackage[69] = CanBusDataPackageClass("0x00000045",dataList.at(69));
    m_mapAddressAndCanBusDataPackage[70] = CanBusDataPackageClass("0x00000045",dataList.at(70));
    m_mapAddressAndCanBusDataPackage[71] = CanBusDataPackageClass("0x00000045",dataList.at(71));

    m_mapAddressAndCanBusDataPackage[72] = CanBusDataPackageClass("0x00000045",dataList.at(72));
    m_mapAddressAndCanBusDataPackage[73] = CanBusDataPackageClass("0x00000045",dataList.at(73));
    m_mapAddressAndCanBusDataPackage[74] = CanBusDataPackageClass("0x00000045",dataList.at(74));
    m_mapAddressAndCanBusDataPackage[75] = CanBusDataPackageClass("0x00000045",dataList.at(75));
    m_mapAddressAndCanBusDataPackage[76] = CanBusDataPackageClass("0x00000045",dataList.at(76));
    m_mapAddressAndCanBusDataPackage[77] = CanBusDataPackageClass("0x00000045",dataList.at(77));
    m_mapAddressAndCanBusDataPackage[78] = CanBusDataPackageClass("0x00000045",dataList.at(78));
    m_mapAddressAndCanBusDataPackage[79] = CanBusDataPackageClass("0x00000045",dataList.at(79));

    m_mapAddressAndCanBusDataPackage[80] = CanBusDataPackageClass("0x00000045",dataList.at(80));
    m_mapAddressAndCanBusDataPackage[81] = CanBusDataPackageClass("0x00000045",dataList.at(81));
    m_mapAddressAndCanBusDataPackage[82] = CanBusDataPackageClass("0x00000045",dataList.at(82));
    m_mapAddressAndCanBusDataPackage[83] = CanBusDataPackageClass("0x00000045",dataList.at(83));
    m_mapAddressAndCanBusDataPackage[84] = CanBusDataPackageClass("0x00000045",dataList.at(84));
    m_mapAddressAndCanBusDataPackage[85] = CanBusDataPackageClass("0x00000045",dataList.at(85));
    m_mapAddressAndCanBusDataPackage[86] = CanBusDataPackageClass("0x00000045",dataList.at(86));
    m_mapAddressAndCanBusDataPackage[87] = CanBusDataPackageClass("0x00000045",dataList.at(87));

    m_mapAddressAndCanBusDataPackage[88] = CanBusDataPackageClass("0x00000045",dataList.at(88));
    m_mapAddressAndCanBusDataPackage[89] = CanBusDataPackageClass("0x00000045",dataList.at(89));
    m_mapAddressAndCanBusDataPackage[90] = CanBusDataPackageClass("0x00000045",dataList.at(90));
    m_mapAddressAndCanBusDataPackage[91] = CanBusDataPackageClass("0x00000045",dataList.at(91));
    m_mapAddressAndCanBusDataPackage[92] = CanBusDataPackageClass("0x00000045",dataList.at(92));
    m_mapAddressAndCanBusDataPackage[93] = CanBusDataPackageClass("0x00000045",dataList.at(93));
    m_mapAddressAndCanBusDataPackage[94] = CanBusDataPackageClass("0x00000045",dataList.at(94));
    m_mapAddressAndCanBusDataPackage[95] = CanBusDataPackageClass("0x00000045",dataList.at(95));

    m_mapAddressAndCanBusDataPackage[96] = CanBusDataPackageClass("0x00000045",dataList.at(96));
    m_mapAddressAndCanBusDataPackage[97] = CanBusDataPackageClass("0x00000045",dataList.at(97));
    m_mapAddressAndCanBusDataPackage[98] = CanBusDataPackageClass("0x00000045",dataList.at(98));
    m_mapAddressAndCanBusDataPackage[99] = CanBusDataPackageClass("0x00000045",dataList.at(99));
    m_mapAddressAndCanBusDataPackage[100] = CanBusDataPackageClass("0x00000045",dataList.at(100));
    m_mapAddressAndCanBusDataPackage[101] = CanBusDataPackageClass("0x00000045",dataList.at(101));
    m_mapAddressAndCanBusDataPackage[102] = CanBusDataPackageClass("0x00000045",dataList.at(102));
    m_mapAddressAndCanBusDataPackage[103] = CanBusDataPackageClass("0x00000045",dataList.at(103));

    m_mapAddressAndCanBusDataPackage[104] = CanBusDataPackageClass("0x00000046",dataList.at(104));
    m_mapAddressAndCanBusDataPackage[105] = CanBusDataPackageClass("0x00000046",dataList.at(105));
    m_mapAddressAndCanBusDataPackage[106] = CanBusDataPackageClass("0x00000046",dataList.at(106));
    m_mapAddressAndCanBusDataPackage[107] = CanBusDataPackageClass("0x00000046",dataList.at(107));
    m_mapAddressAndCanBusDataPackage[108] = CanBusDataPackageClass("0x00000046",dataList.at(108));
    m_mapAddressAndCanBusDataPackage[109] = CanBusDataPackageClass("0x00000046",dataList.at(109));
    m_mapAddressAndCanBusDataPackage[110] = CanBusDataPackageClass("0x00000046",dataList.at(110));
    m_mapAddressAndCanBusDataPackage[111] = CanBusDataPackageClass("0x00000046",dataList.at(111));

    m_mapAddressAndCanBusDataPackage[112] = CanBusDataPackageClass("0x00000046",dataList.at(112));
    m_mapAddressAndCanBusDataPackage[113] = CanBusDataPackageClass("0x00000046",dataList.at(113));
    m_mapAddressAndCanBusDataPackage[114] = CanBusDataPackageClass("0x00000046",dataList.at(114));
    m_mapAddressAndCanBusDataPackage[115] = CanBusDataPackageClass("0x00000046",dataList.at(115));
    m_mapAddressAndCanBusDataPackage[116] = CanBusDataPackageClass("0x00000046",dataList.at(116));
    m_mapAddressAndCanBusDataPackage[117] = CanBusDataPackageClass("0x00000046",dataList.at(117));
    m_mapAddressAndCanBusDataPackage[118] = CanBusDataPackageClass("0x00000046",dataList.at(118));
    m_mapAddressAndCanBusDataPackage[119] = CanBusDataPackageClass("0x00000046",dataList.at(119));

    m_mapAddressAndCanBusDataPackage[120] = CanBusDataPackageClass("0x00000046",dataList.at(120));
    m_mapAddressAndCanBusDataPackage[121] = CanBusDataPackageClass("0x00000046",dataList.at(121));
    m_mapAddressAndCanBusDataPackage[122] = CanBusDataPackageClass("0x00000046",dataList.at(122));
    m_mapAddressAndCanBusDataPackage[123] = CanBusDataPackageClass("0x00000046",dataList.at(123));
    m_mapAddressAndCanBusDataPackage[124] = CanBusDataPackageClass("0x00000046",dataList.at(124));
    m_mapAddressAndCanBusDataPackage[125] = CanBusDataPackageClass("0x00000046",dataList.at(125));
    m_mapAddressAndCanBusDataPackage[126] = CanBusDataPackageClass("0x00000046",dataList.at(126));
    m_mapAddressAndCanBusDataPackage[127] = CanBusDataPackageClass("0x00000046",dataList.at(127));

    m_mapAddressAndCanBusDataPackage[128] = CanBusDataPackageClass("0x00000046",dataList.at(128));
    m_mapAddressAndCanBusDataPackage[129] = CanBusDataPackageClass("0x00000046",dataList.at(129));
    m_mapAddressAndCanBusDataPackage[130] = CanBusDataPackageClass("0x00000046",dataList.at(130));
    m_mapAddressAndCanBusDataPackage[131] = CanBusDataPackageClass("0x00000046",dataList.at(131));
    m_mapAddressAndCanBusDataPackage[132] = CanBusDataPackageClass("0x00000046",dataList.at(132));
    m_mapAddressAndCanBusDataPackage[133] = CanBusDataPackageClass("0x00000046",dataList.at(133));
    m_mapAddressAndCanBusDataPackage[134] = CanBusDataPackageClass("0x00000046",dataList.at(134));
    m_mapAddressAndCanBusDataPackage[135] = CanBusDataPackageClass("0x00000046",dataList.at(135));

    m_mapAddressAndCanBusDataPackage[136] = CanBusDataPackageClass("0x00000046",dataList.at(136));
    m_mapAddressAndCanBusDataPackage[137] = CanBusDataPackageClass("0x00000046",dataList.at(137));
    m_mapAddressAndCanBusDataPackage[138] = CanBusDataPackageClass("0x00000046",dataList.at(138));
    m_mapAddressAndCanBusDataPackage[139] = CanBusDataPackageClass("0x00000046",dataList.at(139));
    m_mapAddressAndCanBusDataPackage[140] = CanBusDataPackageClass("0x00000046",dataList.at(140));
    m_mapAddressAndCanBusDataPackage[141] = CanBusDataPackageClass("0x00000046",dataList.at(141));
    m_mapAddressAndCanBusDataPackage[142] = CanBusDataPackageClass("0x00000046",dataList.at(142));
    m_mapAddressAndCanBusDataPackage[143] = CanBusDataPackageClass("0x00000046",dataList.at(143));

    m_mapAddressAndCanBusDataPackage[144] = CanBusDataPackageClass("0x00000046",dataList.at(144));
    m_mapAddressAndCanBusDataPackage[145] = CanBusDataPackageClass("0x00000046",dataList.at(145));
    m_mapAddressAndCanBusDataPackage[146] = CanBusDataPackageClass("0x00000046",dataList.at(146));
    m_mapAddressAndCanBusDataPackage[147] = CanBusDataPackageClass("0x00000046",dataList.at(147));
    m_mapAddressAndCanBusDataPackage[148] = CanBusDataPackageClass("0x00000046",dataList.at(148));
    m_mapAddressAndCanBusDataPackage[149] = CanBusDataPackageClass("0x00000046",dataList.at(149));
    m_mapAddressAndCanBusDataPackage[150] = CanBusDataPackageClass("0x00000046",dataList.at(150));
    m_mapAddressAndCanBusDataPackage[151] = CanBusDataPackageClass("0x00000046",dataList.at(151));

    m_mapAddressAndCanBusDataPackage[152] = CanBusDataPackageClass("0x00000046",dataList.at(152));
    m_mapAddressAndCanBusDataPackage[153] = CanBusDataPackageClass("0x00000046",dataList.at(153));
    m_mapAddressAndCanBusDataPackage[154] = CanBusDataPackageClass("0x00000046",dataList.at(154));
    m_mapAddressAndCanBusDataPackage[155] = CanBusDataPackageClass("0x00000046",dataList.at(155));
    m_mapAddressAndCanBusDataPackage[156] = CanBusDataPackageClass("0x00000046",dataList.at(156));
    m_mapAddressAndCanBusDataPackage[157] = CanBusDataPackageClass("0x00000046",dataList.at(157));
    m_mapAddressAndCanBusDataPackage[158] = CanBusDataPackageClass("0x00000046",dataList.at(158));
    m_mapAddressAndCanBusDataPackage[159] = CanBusDataPackageClass("0x00000046",dataList.at(159));

    m_mapAddressAndCanBusDataPackage[160] = CanBusDataPackageClass("0x0000005a",dataList.at(160));
    m_mapAddressAndCanBusDataPackage[161] = CanBusDataPackageClass("0x0000005a",dataList.at(161));
    m_mapAddressAndCanBusDataPackage[162] = CanBusDataPackageClass("0x0000005a",dataList.at(162));
    m_mapAddressAndCanBusDataPackage[163] = CanBusDataPackageClass("0x0000005a",dataList.at(163));
    m_mapAddressAndCanBusDataPackage[164] = CanBusDataPackageClass("0x0000005a",dataList.at(164));
    m_mapAddressAndCanBusDataPackage[165] = CanBusDataPackageClass("0x0000005a",dataList.at(165));
    m_mapAddressAndCanBusDataPackage[166] = CanBusDataPackageClass("0x0000005a",dataList.at(166));
    m_mapAddressAndCanBusDataPackage[167] = CanBusDataPackageClass("0x0000005a",dataList.at(167));

    m_mapAddressAndCanBusDataPackage[168] = CanBusDataPackageClass("0x0000005b",dataList.at(168));
    m_mapAddressAndCanBusDataPackage[169] = CanBusDataPackageClass("0x0000005b",dataList.at(169));
    m_mapAddressAndCanBusDataPackage[170] = CanBusDataPackageClass("0x0000005b",dataList.at(170));
    m_mapAddressAndCanBusDataPackage[171] = CanBusDataPackageClass("0x0000005b",dataList.at(171));
    m_mapAddressAndCanBusDataPackage[172] = CanBusDataPackageClass("0x0000005b",dataList.at(172));
    m_mapAddressAndCanBusDataPackage[173] = CanBusDataPackageClass("0x0000005b",dataList.at(173));
    m_mapAddressAndCanBusDataPackage[174] = CanBusDataPackageClass("0x0000005b",dataList.at(174));
    m_mapAddressAndCanBusDataPackage[175] = CanBusDataPackageClass("0x0000005b",dataList.at(175));

    m_mapAddressAndCanBusDataPackage[176] = CanBusDataPackageClass("0x000000a0",dataList.at(176));
    m_mapAddressAndCanBusDataPackage[177] = CanBusDataPackageClass("0x000000a0",dataList.at(177));
    m_mapAddressAndCanBusDataPackage[178] = CanBusDataPackageClass("0x000000a0",dataList.at(178));
    m_mapAddressAndCanBusDataPackage[179] = CanBusDataPackageClass("0x000000a0",dataList.at(179));
    m_mapAddressAndCanBusDataPackage[180] = CanBusDataPackageClass("0x000000a0",dataList.at(180));
    m_mapAddressAndCanBusDataPackage[181] = CanBusDataPackageClass("0x000000a0",dataList.at(181));
    m_mapAddressAndCanBusDataPackage[182] = CanBusDataPackageClass("0x000000a0",dataList.at(182));
    m_mapAddressAndCanBusDataPackage[183] = CanBusDataPackageClass("0x000000a0",dataList.at(183));

    m_mapAddressAndCanBusDataPackage[184] = CanBusDataPackageClass("0x000000a0",dataList.at(184));
    m_mapAddressAndCanBusDataPackage[185] = CanBusDataPackageClass("0x000000a0",dataList.at(185));
    m_mapAddressAndCanBusDataPackage[186] = CanBusDataPackageClass("0x000000a0",dataList.at(186));
    m_mapAddressAndCanBusDataPackage[187] = CanBusDataPackageClass("0x000000a0",dataList.at(187));
    m_mapAddressAndCanBusDataPackage[188] = CanBusDataPackageClass("0x000000a0",dataList.at(188));
    m_mapAddressAndCanBusDataPackage[189] = CanBusDataPackageClass("0x000000a0",dataList.at(189));
    m_mapAddressAndCanBusDataPackage[190] = CanBusDataPackageClass("0x000000a0",dataList.at(190));
    m_mapAddressAndCanBusDataPackage[191] = CanBusDataPackageClass("0x000000a0",dataList.at(191));

    m_mapAddressAndCanBusDataPackage[192] = CanBusDataPackageClass("0x000000a0",dataList.at(192));
    m_mapAddressAndCanBusDataPackage[193] = CanBusDataPackageClass("0x000000a0",dataList.at(193));
    m_mapAddressAndCanBusDataPackage[194] = CanBusDataPackageClass("0x000000a0",dataList.at(194));
    m_mapAddressAndCanBusDataPackage[195] = CanBusDataPackageClass("0x000000a0",dataList.at(195));
    m_mapAddressAndCanBusDataPackage[196] = CanBusDataPackageClass("0x000000a0",dataList.at(196));
    m_mapAddressAndCanBusDataPackage[197] = CanBusDataPackageClass("0x000000a0",dataList.at(197));
    m_mapAddressAndCanBusDataPackage[198] = CanBusDataPackageClass("0x000000a0",dataList.at(198));
    m_mapAddressAndCanBusDataPackage[199] = CanBusDataPackageClass("0x000000a0",dataList.at(199));

    m_mapAddressAndCanBusDataPackage[200] = CanBusDataPackageClass("0x000000a0",dataList.at(200));
    m_mapAddressAndCanBusDataPackage[201] = CanBusDataPackageClass("0x000000a0",dataList.at(201));
    m_mapAddressAndCanBusDataPackage[202] = CanBusDataPackageClass("0x000000a0",dataList.at(202));
    m_mapAddressAndCanBusDataPackage[203] = CanBusDataPackageClass("0x000000a0",dataList.at(203));
    m_mapAddressAndCanBusDataPackage[204] = CanBusDataPackageClass("0x000000a0",dataList.at(204));
    m_mapAddressAndCanBusDataPackage[205] = CanBusDataPackageClass("0x000000a0",dataList.at(205));
    m_mapAddressAndCanBusDataPackage[206] = CanBusDataPackageClass("0x000000a0",dataList.at(206));
    m_mapAddressAndCanBusDataPackage[207] = CanBusDataPackageClass("0x000000a0",dataList.at(207));

    m_mapAddressAndCanBusDataPackage[208] = CanBusDataPackageClass("0x000000a0",dataList.at(208));
    m_mapAddressAndCanBusDataPackage[209] = CanBusDataPackageClass("0x000000a0",dataList.at(209));
    m_mapAddressAndCanBusDataPackage[210] = CanBusDataPackageClass("0x000000a0",dataList.at(210));
    m_mapAddressAndCanBusDataPackage[211] = CanBusDataPackageClass("0x000000a0",dataList.at(211));
    m_mapAddressAndCanBusDataPackage[212] = CanBusDataPackageClass("0x000000a0",dataList.at(212));
    m_mapAddressAndCanBusDataPackage[213] = CanBusDataPackageClass("0x000000a0",dataList.at(213));
    m_mapAddressAndCanBusDataPackage[214] = CanBusDataPackageClass("0x000000a0",dataList.at(214));
    m_mapAddressAndCanBusDataPackage[215] = CanBusDataPackageClass("0x000000a0",dataList.at(215));
#endif
#if 0
    m_mapAddressAndCanBusDataPackage.clear();
    m_mapAddressAndCanBusDataPackage[0] = CanBusDataPackageClass("0x0000006c","40");
    m_mapAddressAndCanBusDataPackage[1] = CanBusDataPackageClass("0x0000006c","05");
    m_mapAddressAndCanBusDataPackage[2] = CanBusDataPackageClass("0x0000006c","40");
    m_mapAddressAndCanBusDataPackage[3] = CanBusDataPackageClass("0x0000006c","05");
    m_mapAddressAndCanBusDataPackage[4] = CanBusDataPackageClass("0x0000006c","40");
    m_mapAddressAndCanBusDataPackage[5] = CanBusDataPackageClass("0x0000006c","05");
    m_mapAddressAndCanBusDataPackage[6] = CanBusDataPackageClass("0x0000006c","40");
    m_mapAddressAndCanBusDataPackage[7] = CanBusDataPackageClass("0x0000006c","05");

    m_mapAddressAndCanBusDataPackage[8] = CanBusDataPackageClass("0x0000006c","40");
    m_mapAddressAndCanBusDataPackage[9] = CanBusDataPackageClass("0x0000006c","05");
    m_mapAddressAndCanBusDataPackage[10] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[11] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[12] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[13] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[14] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[15] = CanBusDataPackageClass("0x0000006c","00");

    m_mapAddressAndCanBusDataPackage[16] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[17] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[18] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[19] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[20] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[21] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[22] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[23] = CanBusDataPackageClass("0x0000006c","00");

    m_mapAddressAndCanBusDataPackage[24] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[25] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[26] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[27] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[28] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[29] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[30] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[31] = CanBusDataPackageClass("0x0000006c","00");

    m_mapAddressAndCanBusDataPackage[32] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[33] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[34] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[35] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[36] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[37] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[38] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[39] = CanBusDataPackageClass("0x0000006c","00");

    m_mapAddressAndCanBusDataPackage[40] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[41] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[42] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[43] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[44] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[45] = CanBusDataPackageClass("0x0000006c","00");
    m_mapAddressAndCanBusDataPackage[46] = CanBusDataPackageClass("0x0000006c","99");
    m_mapAddressAndCanBusDataPackage[47] = CanBusDataPackageClass("0x0000006c","0e");

    m_mapAddressAndCanBusDataPackage[48] = CanBusDataPackageClass("0x00000045","06");
    m_mapAddressAndCanBusDataPackage[49] = CanBusDataPackageClass("0x00000045","2a");
    m_mapAddressAndCanBusDataPackage[50] = CanBusDataPackageClass("0x00000045","09");
    m_mapAddressAndCanBusDataPackage[51] = CanBusDataPackageClass("0x00000045","cc");
    m_mapAddressAndCanBusDataPackage[52] = CanBusDataPackageClass("0x00000045","fc");
    m_mapAddressAndCanBusDataPackage[53] = CanBusDataPackageClass("0x00000045","ff");
    m_mapAddressAndCanBusDataPackage[54] = CanBusDataPackageClass("0x00000045","fb");
    m_mapAddressAndCanBusDataPackage[55] = CanBusDataPackageClass("0x00000045","ff");

    m_mapAddressAndCanBusDataPackage[56] = CanBusDataPackageClass("0x00000045","f7");
    m_mapAddressAndCanBusDataPackage[57] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[58] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[59] = CanBusDataPackageClass("0x00000045","66");
    m_mapAddressAndCanBusDataPackage[60] = CanBusDataPackageClass("0x00000045","fe");
    m_mapAddressAndCanBusDataPackage[61] = CanBusDataPackageClass("0x00000045","30");
    m_mapAddressAndCanBusDataPackage[62] = CanBusDataPackageClass("0x00000045","03");
    m_mapAddressAndCanBusDataPackage[63] = CanBusDataPackageClass("0x00000045","08");

    m_mapAddressAndCanBusDataPackage[64] = CanBusDataPackageClass("0x00000045","10");
    m_mapAddressAndCanBusDataPackage[65] = CanBusDataPackageClass("0x00000045","27");
    m_mapAddressAndCanBusDataPackage[66] = CanBusDataPackageClass("0x00000045","38");
    m_mapAddressAndCanBusDataPackage[67] = CanBusDataPackageClass("0x00000045","13");
    m_mapAddressAndCanBusDataPackage[68] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[69] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[70] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[71] = CanBusDataPackageClass("0x00000045","00");

    m_mapAddressAndCanBusDataPackage[72] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[73] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[74] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[75] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[76] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[77] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[78] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[79] = CanBusDataPackageClass("0x00000045","00");

    m_mapAddressAndCanBusDataPackage[80] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[81] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[82] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[83] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[84] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[85] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[86] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[87] = CanBusDataPackageClass("0x00000045","00");

    m_mapAddressAndCanBusDataPackage[88] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[89] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[90] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[91] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[92] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[93] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[94] = CanBusDataPackageClass("0x00000045","b2");
    m_mapAddressAndCanBusDataPackage[95] = CanBusDataPackageClass("0x00000045","00");

    m_mapAddressAndCanBusDataPackage[96] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[97] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[98] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[99] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[100] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[101] = CanBusDataPackageClass("0x00000045","00");
    m_mapAddressAndCanBusDataPackage[102] = CanBusDataPackageClass("0x00000045","9a");
    m_mapAddressAndCanBusDataPackage[103] = CanBusDataPackageClass("0x00000045","a2");

    m_mapAddressAndCanBusDataPackage[104] = CanBusDataPackageClass("0x00000046","06");
    m_mapAddressAndCanBusDataPackage[105] = CanBusDataPackageClass("0x00000046","2a");
    m_mapAddressAndCanBusDataPackage[106] = CanBusDataPackageClass("0x00000046","09");
    m_mapAddressAndCanBusDataPackage[107] = CanBusDataPackageClass("0x00000046","66");
    m_mapAddressAndCanBusDataPackage[108] = CanBusDataPackageClass("0x00000046","fa");
    m_mapAddressAndCanBusDataPackage[109] = CanBusDataPackageClass("0x00000046","99");
    m_mapAddressAndCanBusDataPackage[110] = CanBusDataPackageClass("0x00000046","f9");
    m_mapAddressAndCanBusDataPackage[111] = CanBusDataPackageClass("0x00000046","ff");

    m_mapAddressAndCanBusDataPackage[112] = CanBusDataPackageClass("0x00000046","f3");
    m_mapAddressAndCanBusDataPackage[113] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[114] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[115] = CanBusDataPackageClass("0x00000046","99");
    m_mapAddressAndCanBusDataPackage[116] = CanBusDataPackageClass("0x00000046","fd");
    m_mapAddressAndCanBusDataPackage[117] = CanBusDataPackageClass("0x00000046","30");
    m_mapAddressAndCanBusDataPackage[118] = CanBusDataPackageClass("0x00000046","03");
    m_mapAddressAndCanBusDataPackage[119] = CanBusDataPackageClass("0x00000046","08");

    m_mapAddressAndCanBusDataPackage[120] = CanBusDataPackageClass("0x00000046","10");
    m_mapAddressAndCanBusDataPackage[121] = CanBusDataPackageClass("0x00000046","27");
    m_mapAddressAndCanBusDataPackage[122] = CanBusDataPackageClass("0x00000046","38");
    m_mapAddressAndCanBusDataPackage[123] = CanBusDataPackageClass("0x00000046","13");
    m_mapAddressAndCanBusDataPackage[124] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[125] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[126] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[127] = CanBusDataPackageClass("0x00000046","00");

    m_mapAddressAndCanBusDataPackage[128] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[129] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[130] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[131] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[132] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[133] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[134] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[135] = CanBusDataPackageClass("0x00000046","00");

    m_mapAddressAndCanBusDataPackage[136] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[137] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[138] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[139] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[140] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[141] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[142] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[143] = CanBusDataPackageClass("0x00000046","00");

    m_mapAddressAndCanBusDataPackage[144] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[145] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[146] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[147] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[148] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[149] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[150] = CanBusDataPackageClass("0x00000046","54");
    m_mapAddressAndCanBusDataPackage[151] = CanBusDataPackageClass("0x00000046","00");

    m_mapAddressAndCanBusDataPackage[152] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[153] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[154] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[155] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[156] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[157] = CanBusDataPackageClass("0x00000046","00");
    m_mapAddressAndCanBusDataPackage[158] = CanBusDataPackageClass("0x00000046","99");
    m_mapAddressAndCanBusDataPackage[159] = CanBusDataPackageClass("0x00000046","a3");

    m_mapAddressAndCanBusDataPackage[160] = CanBusDataPackageClass("0x0000005a","5a");
    m_mapAddressAndCanBusDataPackage[161] = CanBusDataPackageClass("0x0000005a","7a");
    m_mapAddressAndCanBusDataPackage[162] = CanBusDataPackageClass("0x0000005a","00");
    m_mapAddressAndCanBusDataPackage[163] = CanBusDataPackageClass("0x0000005a","33");
    m_mapAddressAndCanBusDataPackage[164] = CanBusDataPackageClass("0x0000005a","01");
    m_mapAddressAndCanBusDataPackage[165] = CanBusDataPackageClass("0x0000005a","10");
    m_mapAddressAndCanBusDataPackage[166] = CanBusDataPackageClass("0x0000005a","99");
    m_mapAddressAndCanBusDataPackage[167] = CanBusDataPackageClass("0x0000005a","4f");

    m_mapAddressAndCanBusDataPackage[168] = CanBusDataPackageClass("0x0000005b","5b");
    m_mapAddressAndCanBusDataPackage[169] = CanBusDataPackageClass("0x0000005b","f5");
    m_mapAddressAndCanBusDataPackage[170] = CanBusDataPackageClass("0x0000005b","00");
    m_mapAddressAndCanBusDataPackage[171] = CanBusDataPackageClass("0x0000005b","99");
    m_mapAddressAndCanBusDataPackage[172] = CanBusDataPackageClass("0x0000005b","01");
    m_mapAddressAndCanBusDataPackage[173] = CanBusDataPackageClass("0x0000005b","20");
    m_mapAddressAndCanBusDataPackage[174] = CanBusDataPackageClass("0x0000005b","ba");
    m_mapAddressAndCanBusDataPackage[175] = CanBusDataPackageClass("0x0000005b","3c");

    m_mapAddressAndCanBusDataPackage[176] = CanBusDataPackageClass("0x000000a0","30");
    m_mapAddressAndCanBusDataPackage[177] = CanBusDataPackageClass("0x000000a0","03");
    m_mapAddressAndCanBusDataPackage[178] = CanBusDataPackageClass("0x000000a0","37");
    m_mapAddressAndCanBusDataPackage[179] = CanBusDataPackageClass("0x000000a0","08");
    m_mapAddressAndCanBusDataPackage[180] = CanBusDataPackageClass("0x000000a0","ff");
    m_mapAddressAndCanBusDataPackage[181] = CanBusDataPackageClass("0x000000a0","0f");
    m_mapAddressAndCanBusDataPackage[182] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[183] = CanBusDataPackageClass("0x000000a0","00");

    m_mapAddressAndCanBusDataPackage[184] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[185] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[186] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[187] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[188] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[189] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[190] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[191] = CanBusDataPackageClass("0x000000a0","00");

    m_mapAddressAndCanBusDataPackage[192] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[193] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[194] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[195] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[196] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[197] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[198] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[199] = CanBusDataPackageClass("0x000000a0","00");

    m_mapAddressAndCanBusDataPackage[200] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[201] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[202] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[203] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[204] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[205] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[206] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[207] = CanBusDataPackageClass("0x000000a0","00");

    m_mapAddressAndCanBusDataPackage[208] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[209] = CanBusDataPackageClass("0x000000a0","ff");
    m_mapAddressAndCanBusDataPackage[210] = CanBusDataPackageClass("0x000000a0","f0");
    m_mapAddressAndCanBusDataPackage[211] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[212] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[213] = CanBusDataPackageClass("0x000000a0","00");
    m_mapAddressAndCanBusDataPackage[214] = CanBusDataPackageClass("0x000000a0","6f");
    m_mapAddressAndCanBusDataPackage[215] = CanBusDataPackageClass("0x000000a0","22");
#endif
}
