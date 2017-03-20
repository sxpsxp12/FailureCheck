/***************************************************************************************************************
 * 解析总线数据 包括有以下内容（内容不全随时添加）：
 * 通过bus_info算数解析，得出线号对应的键值对关系。
 * 通过延时码表和延迟触发条件表得出延时ID对应的键值对关系。
 * 通过自锁码表和自锁触发条件得出self_hold_id对听的键值对关系。
 * 通过故障码表和故障触发条件表检测故障。
 * 通过故障码表和故障历史记录表记录故障。
 * 通过梯形图码表解析梯形图中各个线号对应的状态
 * 通过设备状态码表和设备状态触发条件和键值对关系得出每个设备的实时状态数据，包含有颜色，
 *     图片名称，设备状态描述，数据值等等。
***************************************************************************************************************/
#include "dataanalysis.h"
#include "databasesoperate.h"
#include "enumtypeheader.h"

#include <QDebug>
#include <QTimer>
#include <QStringList>
#include <QTime>

#include "logfile.h"
#include "requestdatathread.h"
#include "voiceplaythread.h"

RealtimeDataInfoList DataAnalysis::m_primaryWidgetRealtimeDataList;
FailureInfoList DataAnalysis::m_primaryWidgetFailureInfoList;
RealtimeDataInfoList DataAnalysis::m_secondaryWidgetRealtimeDataList;
FailureInfoList DataAnalysis::m_secondaryWidgetFailureInfoList;
RealtimeDataInfoList DataAnalysis::m_busIdWidgetRealtimeDataList;
RealtimeDataInfoList DataAnalysis::m_interfaceLampHoleWidgetRealtimeDataList;
RealtimeDataInfoList DataAnalysis::m_dcuWidgetRealtimeDataList;
RealtimeDataInfoList DataAnalysis::m_gcuWidgetRealtimeDataList;
RealtimeDataInfoList DataAnalysis::m_lgWidgetRealtimeDataList;
LadderDiagramInfoList DataAnalysis::m_ladderDiagramPartStateInfoList;
FailureHistoryRecordInfoList DataAnalysis::m_failureHistoryRecordInfoList;

DataAnalysis::DataAnalysis()
{
    //故障信息记录
    if( CBaseOperate::getNewDataTxtFileNumber() )
    {
        //判断是否大于文件个数的限制31个
        if(CBaseOperate::newDataTxtFileNumber > NEWDATAFILE_SIZE)
        {
            //删除旧的文件
            if(CBaseOperate::limitNewDataFileTxtNumber())
            {
                CBaseOperate::newDataTxtFileNumber--;
            }
        }
    }

    //实时数据记录
    if( CBaseOperate::getRealDataFileNumber() )
    {
        //判断是否大于文件个数的限制31个
        if(CBaseOperate::RealDataFileNumber > SDFILE_SIZE)
        {
            //删除旧的文件
            if(CBaseOperate::limitRealDataFileNumber())
            {
                CBaseOperate::RealDataFileNumber--;
            }
        }
    }

    dataAnalysisInit();
    m_timerDelay = new QTimer;
    m_timerDelay->start(1000);
    connect(m_timerDelay, SIGNAL(timeout()), this, SLOT(slot_timerDelayTimeOut()));

    m_timerRealData = new QTimer;
    m_timerRealData->start(REQUEST_BUS_DATA_TIME_INTERVAL); //以请求实时数据的时间去定时
    connect(m_timerRealData,SIGNAL(timeout()),this,SLOT(slot_timerRealDataTimeOut()));
}

DataAnalysis::~DataAnalysis()
{
    if(m_timerDelay != NULL)
    {
        delete m_timerDelay;
    }

    if(m_timerRealData != NULL)
    {
        delete m_timerRealData;
    }
}

void DataAnalysis::dataAnalysis(const QDateTime &dateTime,const QString &data)
{
    m_currentRealData = data;
    m_questDataTime = dateTime;
    //算数解析
    arithmeticAnalysis();
    delayAnalysis();
    selfHoldAnalysis();

    //逻辑解析
    failureDetectAnalysis();
    deviceStateAnalysis();
    ladderDiagramAnalysis();
    //数据打包
    dataPackage();
}

#if 0
RealtimeDataInfoList DataAnalysis::getPrimaryWidgetRealtimeDataList() const
{
    return m_primaryWidgetRealtimeDataList;
}

FailureInfoList DataAnalysis::getPrimaryWidgetFailureInfoList() const
{
    return m_primaryWidgetFailureInfoList;
}

RealtimeDataInfoList DataAnalysis::getSecondaryWidgetRealtimeDataList() const
{
    return m_secondaryWidgetRealtimeDataList;
}

FailureInfoList DataAnalysis::getSecondaryWidgetFailureInfoList() const
{
    return m_secondaryWidgetFailureInfoList;
}

RealtimeDataInfoList DataAnalysis::getBusIdWidgetRealtimeDataList() const
{
    return m_busIdWidgetRealtimeDataList;
}

RealtimeDataInfoList DataAnalysis::getInterfaceLampHoleWidgetRealtimeDataList() const
{
    return m_interfaceLampHoleWidgetRealtimeDataList;
}

RealtimeDataInfoList DataAnalysis::getDcuWidgetRealtimeDataList() const
{
    return m_dcuWidgetRealtimeDataList;
}

RealtimeDataInfoList DataAnalysis::getGcuWidgetRealtimeDataList() const
{
    return m_gcuWidgetRealtimeDataList;
}

RealtimeDataInfoList DataAnalysis::getLgWidgetRealtimeDataList() const
{
    return m_lgWidgetRealtimeDataList;
}

LadderDiagramInfoList DataAnalysis::getLadderDiagramPartStateInfoList() const
{
    return m_ladderDiagramPartStateInfoList;
}

FailureHistoryRecordInfoList DataAnalysis::getFailureHistoryRecordInfoList() const
{
    return m_failureHistoryRecordInfoList;
}
#endif

void DataAnalysis::arithmeticAnalysis()
{
#if LOGW
        LogFile::debug("into算数解析 arithmeticAnalysis()");
#endif

    //通过 `bus_info`(线号码表) 进行算数解析
    for (int i = 0; i < m_busInfoList.size(); i++)
    {
        BusInfoClass busInfo = m_busInfoList.at(i);

        double parameterA;
        double parameterB;
        bool isNegation;
        double parameterC;
        double parameterD;
        double parameterE;
        double parameterF;

        if (!busInfo.getFormulaParameter().isEmpty())
        {
            QStringList  formulaParameterList = busInfo.getFormulaParameter().split("~");
            QList<double> dblDormulaParameterList;

            for (int i = 0; i < formulaParameterList.size(); i++)
            {
                dblDormulaParameterList.append(formulaParameterList.at(i).toDouble());
            }

            parameterA = dblDormulaParameterList.at(0);
            parameterB = dblDormulaParameterList.at(1);
            isNegation = (dblDormulaParameterList.at(2) == 0)? false : true;
            parameterC = dblDormulaParameterList.at(3);
            parameterD = dblDormulaParameterList.at(4);
            parameterE = dblDormulaParameterList.at(5);
            parameterF = dblDormulaParameterList.at(6);
        }

        switch (busInfo.getAddrType())
        {
        //只有低字节地址, 计算整个字节，保存成十进制数
        case ONLY_BYTE_ADDR_L:
            m_mapKeyAndStrNumericalValue[busInfo.getBusId()] =
                    QString::number(RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getDecNumber(), 'f', 1);
            break;

            //低字节地址加位地址，保存成十进制数，0或1
        case BYTE_ADDR_L_ADD_BIT_ADDR:
            m_mapKeyAndStrNumericalValue[busInfo.getBusId()] =
                    QString::number(RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getBitAddress(busInfo.getBitAddr()));
            break;

            //高字节地址加低字节地址 使用公式解析
        case BYTE_ADDR_H_ADD_BYTE_ADDR_L:
        {
            int highByteValue;
            int lowByteValue;
            int tempParameterValue;
            double finalValue;
            int tempValue;
            int hour;
            int minute;
            int second;

            switch (busInfo.getFormulaTemplateId())
            {
            //(A/B)*((bool(H*C+ L*D))+E)*F
            case FORMULA_TEMPLATE_ONE:

                highByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrH()).getDecNumber();
                lowByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getDecNumber();
                tempParameterValue = highByteValue * parameterC+ lowByteValue * parameterD;

                if (isNegation)
                {
                    if ((int)tempParameterValue != 0)
                    {
                        finalValue = (parameterA / parameterB)
                                * ((int)(((~(int)tempParameterValue) + parameterE) + 65536) % 65536)
                                * parameterF;
                    }
                    else
                    {
                        finalValue = 0.0;
                    }
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }
                else
                {
                    finalValue = (parameterA / parameterB)
                            * (((highByteValue * parameterC+ lowByteValue * parameterD)) + parameterE)
                            * parameterF;
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }
                break;

                //(A/B)*((bool(H*C+ L*D))+E)*F 最后结果转换为时分秒
            case FORMULA_TEMPLATE_ONE_AND_SECONDS_TRANSFORM_HH_MM_SS:

                highByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrH()).getDecNumber();
                lowByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getDecNumber();
                tempParameterValue = highByteValue * parameterC+ lowByteValue * parameterD;

                if (isNegation)
                {
                    finalValue = (parameterA / parameterB)
                            * ((int)(((~(int)tempParameterValue) + parameterE) + 65536) % 65536)
                            * parameterF;
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }
                else
                {
                    finalValue = (parameterA / parameterB)
                            * (((highByteValue * parameterC+ lowByteValue * parameterD)) + parameterE)
                            * parameterF;
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }

                tempValue = (int) finalValue;

                //进行秒数到HH：MM：SS的转换
                hour = tempValue / 3600;
                tempValue %= 3600;
                minute = tempValue / 60;
                tempValue %= 60;
                second = tempValue;
                m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(hour) + "h:" +
                        QString::number(minute) + "m:" + QString::number(second) + "s";
                break;

            case FORMULA_TEMPLATE_ONE_AND_RESULT_ADD_PERCENT:

                highByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrH()).getDecNumber();
                lowByteValue = RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getDecNumber();
                tempParameterValue = highByteValue * parameterC+ lowByteValue * parameterD;

                if (isNegation)
                {
                    finalValue = (parameterA / parameterB)
                            * ((int)(((~(int)tempParameterValue) + parameterE) + 65536) % 65536)
                            * parameterF;
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }
                else
                {
                    finalValue = (parameterA / parameterB)
                            * (((highByteValue * parameterC+ lowByteValue * parameterD)) + parameterE)
                            * parameterF;
                    m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1);
                }

                m_mapKeyAndStrNumericalValue[busInfo.getBusId()] = QString::number(finalValue, 'f', 1) + "%";
                break;

            default:
                break;
            }
            break;
        }

            //只有低字节地址，计算低四位的值(工况中使用)
        case ONLY_BYTE_ADDR_L_FOUR_BIT_L:
            m_mapKeyAndStrNumericalValue[busInfo.getBusId()] =
                    RequestDataThread::m_mapAddressAndCanBusDataPackage.value(busInfo.getByteAddrL()).getDecLowBitNumberStr();
            break;

        default:
            break;
        }
    }

#if LOGW
        LogFile::debug("out算数解析 arithmeticAnalysis()");
#endif
}

void DataAnalysis::slot_timerDelayTimeOut()
{
    for (int i = 0; i < m_delayInfoList.size(); i++)
    {
        DelayInfoClass delayInfo = m_delayInfoList.at(i);

        //延时条件不成立
        if (!delayInfo.isStartTimer())
        {
            continue;
        }

        //延时条件成立
        delayInfo.setConditionsHoldSeconds(delayInfo.getConditionsHoldSeconds() + 1);
        m_delayInfoList.replace(i, delayInfo);

        switch (delayInfo.getLogicalRelationship())
        {
        case GREATER_OR_EQUAL:  //大于等于
            //成立delayInfo
            if (delayInfo.getConditionsHoldSeconds() >= delayInfo.getDelaySeconds())
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue());
            }
            else
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue() == 1?0 : 1);
            }
            break;
        case GREATER:
            //成立delayInfo
            if (delayInfo.getConditionsHoldSeconds() > delayInfo.getDelaySeconds())
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue());
            }
            else
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue() == 1?0 : 1);
            }
            break;
        case LESS:
            //成立delayInfo
            if (delayInfo.getConditionsHoldSeconds() < delayInfo.getDelaySeconds())
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue());
            }
            else
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue() == 1?0 : 1);
            }
            break;
        case LESS_OR_EQUAL:     //小于等于
            if (delayInfo.getConditionsHoldSeconds() <= delayInfo.getDelaySeconds())
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue());
            }
            else
            {
                m_mapKeyAndStrNumericalValue[delayInfo.getDelayId()] = QString::number(delayInfo.getDelayValue() == 1?0 : 1);
            }
            break;

        default:
            break;
        }
    }
}

//时间一到进行数据记录，并将整体数据存放到txt文档中
void DataAnalysis::slot_timerRealDataTimeOut()
{
    FailureHistoryRecordInfoList recordRealDataFailureHistoryList;
    FailureHistoryRecordClass failureHistoryRecord;
    recordRealDataFailureHistoryList.clear();

    for(int i = 0;i < m_recordRealDataFailureHistoryList.size();i++)
    {
        failureHistoryRecord = m_recordRealDataFailureHistoryList.at(i);
        failureHistoryRecord.setTimerHoldNum(failureHistoryRecord.getTimerHoldNum()+1);
        m_recordRealDataFailureHistoryList.replace(i,failureHistoryRecord);

        //分别判断是否已经有故障满足条件,满足条件的故障直接从列表中删除
        //故障记录的时间实际上比故障发生的时间晚FAILURE_DETECT_DELAY_TIME
        if( failureHistoryRecord.getTimerHoldNum() >= ((FAILURE_RECORDDATA_TIME_POINT-FAILURE_DETECT_DELAY_TIME)/REQUEST_BUS_DATA_TIME_INTERVAL) )    //满足条件
        {
            //将满足条件的故障实时数据记录删除,所有满足条件的故障的都是同时发生的故障
            recordRealDataFailureHistoryList.append(failureHistoryRecord);
            m_recordRealDataFailureHistoryList.removeAt(i);
        }
    }

    //有满足条件的故障才进行记录
    if(recordRealDataFailureHistoryList.size() != 0)
    {
        //将实时数据记录到txt文档中
        HandleFailureRealTimeData(recordRealDataFailureHistoryList);
    }
}

void DataAnalysis::dataAnalysisInit()
{
    DatabasesOperate::initBusInfoList(m_busInfoList);

    DatabasesOperate::initDelayInfoList(m_delayInfoList);
    DatabasesOperate::initMapDelayIdAndDelayTriggerConditionInfoList(
                m_mapDelayIdAndDelayTriggerConditionInfoList, m_delayInfoList);

    //初始化所有延迟Id对应的值为0
    for (int i = 0; i < m_delayInfoList.size(); i++)
    {
        m_mapKeyAndStrNumericalValue[m_delayInfoList.at(i).getDelayId()] = "0";
    }

    DatabasesOperate::initSelfHoldInfoList(m_selfHoldInfoList);
    DatabasesOperate::initMapSelfHoldIdAndSelfHoldTriggerConditionInfoList(
                m_mapSelfHoldIdAndSelfHoldTriggerConditionInfoList, m_selfHoldInfoList);

    //初始化所有自锁Id对应的值为0
    for (int i = 0; i < m_selfHoldInfoList.size(); i++)
    {
        m_mapKeyAndStrNumericalValue[m_selfHoldInfoList.at(i).getSelfHoldId()] = "0";
    }

    DatabasesOperate::initFailureInfoList(m_failureInfoList);
    DatabasesOperate::initMapFailureIdAndFailureTriggerConditionInfoList(
                m_mapFailureIdAndFailureTriggerConditionInfoList, m_failureInfoList);

    DatabasesOperate::initDeviceStateInfoList(m_deviceStateInfoList);
    DatabasesOperate::initMapDeviceStateIdAndDeviceStateTriggerConditionInfoList(
                m_mapDeviceStateIdAndDeviceStateTriggerConditionInfoList, m_deviceStateInfoList);

    DatabasesOperate::initAllBaseInfoList(m_allBaseInfoList);

    DatabasesOperate::initLadderDiagramInfoList(m_ladderDiagramPartStateInfoList);

    m_LastsecondaryWidgetFailureInfoList.clear();
    m_LastprimaryWidgetFailureInfoList.clear();
    m_secondaryWidgetFailureInfoList.clear();
    m_primaryWidgetFailureInfoList.clear();
    m_failureHistoryRecordInfoList.clear();
    m_recordRealDataFailureHistoryList.clear();
    m_realDataList.clear();
}

void DataAnalysis::delayAnalysis()
{
#if LOGW
        LogFile::debug("into延迟触发 delayAnalysis()");
#endif
    //通过`delay_info`(延时码表) 和 `delay_trigger_condition` (延迟触发条件)进行算数解析
    for (int i = 0; i < m_delayInfoList.size(); i++)
    {
        DelayInfoClass delayInfo = m_delayInfoList.at(i);
        //获取延迟触发条件
        DelayTriggerConditionInfoList delayTriggerConditionInfoList
                = m_mapDelayIdAndDelayTriggerConditionInfoList[delayInfo.getDelayId()];

        //所有条件成立标志位
        bool isConditionsHold = true;
        //遍历判断延迟触发条件
        for (int j = 0; j < delayTriggerConditionInfoList.size(); j++)
        {
            //单个条件成立标志位
            bool isSingleConditionHole = true;

            DelayTriggerConditionClass delayTriggerConditionClass = delayTriggerConditionInfoList.at(j);

            switch(delayTriggerConditionClass.getLogicalRelationship().toInt()){
            case GREATER_OR_EQUAL:      //大于等于
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() >= delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case GREATER:
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() > delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS_OR_EQUAL:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() <= delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() < delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case EQUAL: //等于
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() == delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case NOT_EQUAL:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[delayTriggerConditionClass.getBusId()].toDouble() != delayTriggerConditionClass.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            default:break;
            }

            //单个条件不成立，该延迟条件不满足
            if (!isSingleConditionHole)
            {
                isConditionsHold = false;
                break;
            }
        }

        //触发条件全部成立时  打开计时器标志位
        if (isConditionsHold)
        {
            delayInfo.setIsStartTimer(true);
            m_delayInfoList.replace(i, delayInfo);
        }
        //触发条件不成立时  关闭计时器标志位,保持当前值不变
        else
        {
            delayInfo.setIsStartTimer(false);
            delayInfo.setConditionsHoldSeconds(0);
            m_delayInfoList.replace(i, delayInfo);
        }
    }

#if LOGW
        LogFile::debug("out延迟触发 delayAnalysis()");
#endif
}

void DataAnalysis::selfHoldAnalysis()
{
#if LOGW
        LogFile::debug("into自锁触发 selfHoldAnalysis()");
#endif
    //通过`self_hold_info` (自锁码表)和`self_hold_trigger_condition` (自锁触发条件)进行算数解析
    for (int i = 0; i < m_selfHoldInfoList.size(); i++)
    {
        SelfHoldInfoClass selfHoldInfo = m_selfHoldInfoList.at(i);
        //获取自锁触发条件
        SelfHoldTriggerConditionInfoList selfHoldTriggerConditionInfoList
                = m_mapSelfHoldIdAndSelfHoldTriggerConditionInfoList[selfHoldInfo.getSelfFlagId()];

        bool isConditionsHold = true;
        //遍历判断自锁触发条件
        for (int j = 0; j < selfHoldTriggerConditionInfoList.size(); j++)
        {
            //获取触发条件
            SelfHoldTriggerConditionClass selfHoldTriggerCondition = selfHoldTriggerConditionInfoList.at(j);
            //单个条件成立标志位
            bool isSingleConditionHole = true;

            switch(selfHoldTriggerCondition.getLogicalRelationship().toInt()){
            case GREATER_OR_EQUAL:      //大于等于
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() >= selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case GREATER:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() > selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS_OR_EQUAL:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() <= selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() < selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case EQUAL: //等于
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() == selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case NOT_EQUAL:
                //计算出来的值与触发值比较
                if(m_mapKeyAndStrNumericalValue[selfHoldTriggerCondition.getBusId()].toDouble() != selfHoldTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            default:break;
            }

            //单个条件不成立
            if (!isSingleConditionHole)
            {
                isConditionsHold = false;
                break;
            }
        }

        //触发条件全部成立时
        if (isConditionsHold)
        {
            m_mapKeyAndStrNumericalValue[selfHoldInfo.getSelfHoldId()] = QString::number(selfHoldInfo.getSelfHoldValue());
        }
        //触发条件不成立时
        //        else
        //        {
        //            m_mapKeyAndStrNumericalValue[selfHoldInfo.getSelfHoldId()] = QString::number((selfHoldInfo.getSelfHoldValue() == 0)? 1:0);
        //        }
    }

#if LOGW
        LogFile::debug("out自锁触发 selfHoldAnalysis()");
#endif
}

//故障
void DataAnalysis::failureDetectAnalysis()
{
#if LOGW
        LogFile::debug("into故障检测解析 failureDetectAnalysis()");
#endif

    qDebug() << "into failureDetectAnalysis()";

    FailureInfoList *failureInfoList = NULL;
    //缓冲区
    //每次判断故障的时候都要将以前的列表清空,防止故障已经解决还会报出故障
    FailureInfoList primaryWidgetFailureInfoList;
    FailureInfoList secondaryWidgetFailureInfoList;
    FailureHistoryRecordInfoList  failureHistoryRecordInfoList;
    FailureHistoryRecordInfoList recordRealDataFailureHistoryList;

    recordRealDataFailureHistoryList.clear();
    primaryWidgetFailureInfoList.clear();
    secondaryWidgetFailureInfoList.clear();
    failureHistoryRecordInfoList.clear();
    //每次都更新满足条件的故障列表
    m_primaryWidgetFailureInfoList.clear();
    m_secondaryWidgetFailureInfoList.clear();

    qDebug() << "111111111111";
    //通过`failure_info`(故障码表) 和 `failure_trigger_condition`(故障触发条件)进行故障检测解析
    for (int i = 0; i < m_failureInfoList.size(); i++)
    {
        FailureInfoClass failureInfo = m_failureInfoList.at(i);
        //获取故障触发条件
        FailureTriggerConditionInfoList failureTriggerConditionInfoList
                = m_mapFailureIdAndFailureTriggerConditionInfoList[failureInfo.getFailureId()];

        bool isConditionsHold = true;
        //遍历判断故障触发条件
        for (int j = 0; j < failureTriggerConditionInfoList.size(); j++)
        {
            FailureTriggerConditionClass failureTriggerCondition = failureTriggerConditionInfoList.at(j);
            //单个条件成立标志位
            bool isSingleConditionHole = true;

            switch(failureTriggerCondition.getLogicalRelationship().toInt())
            {
            case GREATER_OR_EQUAL:      //大于等于
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() >= failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case GREATER:       //大于
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() > failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS_OR_EQUAL:     //小于等于
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() <= failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS:          //小于
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() < failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case EQUAL: //等于
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() == failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case NOT_EQUAL:
                if(m_mapKeyAndStrNumericalValue[failureTriggerCondition.getBusId()].toDouble() != failureTriggerCondition.getTriggerValue().toDouble())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            default:break;
            }

            //单个条件不成立
            if (!isSingleConditionHole)
            {
                isConditionsHold = false;
                break;
            }
        }

        //所有条件成立
        if (isConditionsHold)
        {
            switch (failureInfo.getFailureLv())
            {
            case SERIOUS:
            case MEDIUM:
            case SLIGHT:
                failureInfoList = &primaryWidgetFailureInfoList;
                break;

            case PROMPT:
                failureInfoList = &secondaryWidgetFailureInfoList;
                break;

            default:
                break;
            }

            //最新发生的故障
            if (!failureInfoList->contains(failureInfo))
            {
                failureInfo.setFailureOccurTime(m_questDataTime);
                failureInfo.setFailureDurationNum(1);   //每次新的故障发生都记为发生一次
                failureInfo.setCurrentRealData(m_currentRealData);  //当前的实时数据
                failureInfoList->append(failureInfo);
            }
        }
    }
    qDebug() << "22222222222222222222:count():" << primaryWidgetFailureInfoList.size() << secondaryWidgetFailureInfoList.size();
    //在筛选列表之前，遍历旧的故障列表
    foreach (FailureInfoClass lastPrimaryFailureInfo, m_LastprimaryWidgetFailureInfoList) {
        //通过查询最新的故障列表，将旧的故障列表中已经解决的故障删除, 防止旧的故障列表导致内存溢出
        if( !primaryWidgetFailureInfoList.contains(lastPrimaryFailureInfo) )
        {
            m_LastprimaryWidgetFailureInfoList.removeOne(lastPrimaryFailureInfo);
        }
    }
    qDebug() << "3333333333333333333333";
    foreach (FailureInfoClass lastSecondFailureInfo, m_LastsecondaryWidgetFailureInfoList) {
        //通过查询最新的故障列表，将旧的故障列表中已经解决的故障删除, 防止旧的故障列表导致内存溢出
        if( !secondaryWidgetFailureInfoList.contains(lastSecondFailureInfo) )
        {
            m_LastsecondaryWidgetFailureInfoList.removeOne(lastSecondFailureInfo);
        }
    }
    qDebug() << "4444444444444444444";
    //筛选列表，将满足条件的故障进行保存        遍历一级界面的故障列表
    for(int i = 0;i < primaryWidgetFailureInfoList.size();i++)
    {
        //上次的故障列表为空，退出并将本次故障列表进行保存
        if(m_LastprimaryWidgetFailureInfoList.size() == 0)
        {
            m_LastprimaryWidgetFailureInfoList = primaryWidgetFailureInfoList;
            break;
        }else
        {
            FailureInfoClass  primaryFailureInfo = primaryWidgetFailureInfoList.at(i);
            if(m_LastprimaryWidgetFailureInfoList.contains(primaryFailureInfo))
            {
                //将本次故障依次与上次故障列表进行比对，如果满足故障确定的条件，进行保存，可以在界面显示出来
                int pos = m_LastprimaryWidgetFailureInfoList.indexOf(primaryFailureInfo);
                //将本地的故障列表中的故障发生次数加1，并进行判断是否满足显示和播报的条件
                FailureInfoClass failure =  m_LastprimaryWidgetFailureInfoList.at(pos);
                failure.setFailureDurationNum(failure.getFailureDurationNum()+1);
                m_LastprimaryWidgetFailureInfoList.replace(pos,failure);
                if( (failure.getFailureDurationNum()-1) * REQUEST_BUS_DATA_TIME_INTERVAL >= FAILURE_DETECT_DELAY_TIME)
                {
                    m_primaryWidgetFailureInfoList.append(failure);
                    //将满足条件的故障保存到故障历史缓冲列表中,该列表中只有本次故障
                    failureHistoryRecordInfoList.append(FailureHistoryRecordClass(failure.getFailureId(),failure.getFailureLv(),
                                                                                  failure.getFailureOccurTime().toString("yyyy-MM-dd hh:mm:ss"),
                                                                                  QString(""),failure.getFailureName(),
                                                                                  failure.getCurrentRealData() ));
                    //当有故障时，将该故障记录到内存中的列表中，同时打开定时器，以便记录后半段的实时数据
                    //将实时记录存放到列表中
                    recordRealDataFailureHistoryList.append(failureHistoryRecordInfoList.last());
                }
            }else       //不包含说明是新的故障
            {
                m_LastprimaryWidgetFailureInfoList.append(primaryFailureInfo);
            }
        }
    }
    qDebug() << "555555555555555555";
    //排序,故障按故障等级排
    if(m_primaryWidgetFailureInfoList.size() != 0)
    {
        qSort(m_primaryWidgetFailureInfoList.begin(),m_primaryWidgetFailureInfoList.end(),FailureInfoClass::failureInfoListSort);
    }

    qDebug() << "6666666666666666666666";
    //遍历二级界面的故障列表
    foreach (FailureInfoClass secondFailureInfo, secondaryWidgetFailureInfoList)
    {
        //上次的故障列表为空，退出并将本次故障列表进行保存
        if(m_LastsecondaryWidgetFailureInfoList.size() == 0)
        {
            m_LastsecondaryWidgetFailureInfoList = secondaryWidgetFailureInfoList;
            break;
        }else
        {
            //将本次故障依次与上次故障列表进行比对，如果出现一样的，进行保存，可以在界面显示出来
            if(m_LastsecondaryWidgetFailureInfoList.contains(secondFailureInfo))
            {
                int pos = m_LastsecondaryWidgetFailureInfoList.indexOf(secondFailureInfo);
                //将本地的故障列表中的故障发生次数加1，并进行判断是否满足显示和播报的条件
                FailureInfoClass failure = m_LastsecondaryWidgetFailureInfoList.at(pos);
                failure.setFailureDurationNum(failure.getFailureDurationNum()+1);
                m_LastsecondaryWidgetFailureInfoList.replace(pos,failure);

                if( (failure.getFailureDurationNum()-1) * REQUEST_BUS_DATA_TIME_INTERVAL >= FAILURE_DETECT_DELAY_TIME)
                {
                    m_secondaryWidgetFailureInfoList.append(failure);
                    failureHistoryRecordInfoList.append(FailureHistoryRecordClass(failure.getFailureId(),failure.getFailureLv(),
                                                                                  failure.getFailureOccurTime().toString("yyyy-MM-dd hh:mm:ss"),
                                                                                  QString(""),failure.getFailureName(),
                                                                                  failure.getCurrentRealData() ));

                    //当有故障时，将该故障记录到内存中的列表中，同时打开定时器，以便记录后半段的实时数据
                    //将实时记录存放到列表中
                    recordRealDataFailureHistoryList.append(failureHistoryRecordInfoList.last());
                }
            }else       //追加新的故障
            {
                m_LastsecondaryWidgetFailureInfoList.append(secondFailureInfo);
            }
        }
    }
    qDebug() << "777777777777777777777";
    //遍历记录实时故障列表
    foreach (FailureHistoryRecordClass failureHistoryRecord, recordRealDataFailureHistoryList) {
        //只有在不包含的情况下才追加，避免同一时间满足条件的故障记录多次
        if( !m_recordRealDataFailureHistoryList.contains(failureHistoryRecord) )
        {
            m_recordRealDataFailureHistoryList.append(failureHistoryRecord);
        }
    }
    qDebug() << "888888888888888888888888888";
    //所有的故障筛选完毕，将符合条件的故障存放到列表中，以便定时器到时进行遍历，将数据写到文件中
   // m_recordRealDataFailureHistoryList.append(recordRealDataFailureHistoryList);

    //排序,提示的故障按时间排序
    if(m_secondaryWidgetFailureInfoList.size() != 0)
    {
        qSort(m_secondaryWidgetFailureInfoList.begin(),m_secondaryWidgetFailureInfoList.end(),FailureInfoClass::failureInfoListPromptSort);
    }
    qDebug() << "999999999999999999999";
    //遍历旧的列表判断是否有可以整和的故障
    if(m_failureHistoryRecordInfoList.size() != 0)
    {
        qDebug() << "aaaaaaaaaaaaaaaaaaaaaa";
        for(int i = 0;i < m_failureHistoryRecordInfoList.size();i++)
        {
            FailureHistoryRecordClass failureHistoryRecordClass = m_failureHistoryRecordInfoList.at(i);
            //结束时间已经存在的故障已经处理,删除故障记录列表中的该条故障
            if(!failureHistoryRecordClass.getEndTime().isEmpty())
            {
                //                qDebug() << "故障End,从列表中删除:" <<"故障ID：" << failureHistoryRecordClass.getFailureId() << " 故障名字:" << failureHistoryRecordClass.getFailureName()  << " 故障发生时间:" << failureHistoryRecordClass.getStartTime()
                //                         << " 故障结束时间:" << failureHistoryRecordClass.getEndTime();
                m_failureHistoryRecordInfoList.removeAt(i);
            }
        }
        qDebug() << "bbbbbbbbbbbbbbbbbbbbbbb";
        //删除已经解决的故障后，列表中存在的都是没有解决的故障.有以下两种情况
        //将已经存在的故障进行整合
        for(int i = 0;i < m_failureHistoryRecordInfoList.size();i++ )
        {
            FailureHistoryRecordClass failureHistoryRecordClass = m_failureHistoryRecordInfoList.at(i);

            if(!failureHistoryRecordInfoList.contains(failureHistoryRecordClass))
            {
                //本次没有出现的故障已经被解决，将结束时间写上
                failureHistoryRecordClass.setEndTime(m_questDataTime.toString("yyyy-MM-dd hh:mm:ss"));
                m_failureHistoryRecordInfoList.replace(i,failureHistoryRecordClass);
                //qDebug() << "故障End:" <<"故障ID：" << failureHistoryRecordClass.getFailureId() << " 故障名字:" << failureHistoryRecordClass.getFailureName()  << " 故障发生时间:" << failureHistoryRecordClass.getStartTime()
                //       << " 故障结束时间:" << failureHistoryRecordClass.getEndTime();
            }
        }
        qDebug() << "ccccccccccccccccccccccc";
        //将新出现的故障进行记录
        foreach (FailureHistoryRecordClass failureHistoryRecordClass,failureHistoryRecordInfoList)
        {
            if(!m_failureHistoryRecordInfoList.contains(failureHistoryRecordClass)) //新故障
            {
                // qDebug() << "故障发生:" <<"故障ID：" << failureHistoryRecordClass.getFailureId() << " 故障名字:" << failureHistoryRecordClass.getFailureName()  << " 故障发生时间:" << failureHistoryRecordClass.getStartTime()
                //          << " 故障结束时间:" << failureHistoryRecordClass.getEndTime();
                m_failureHistoryRecordInfoList.append(failureHistoryRecordClass);

                //播报故障
                //播报一级界面新的故障
                switch(failureHistoryRecordClass.getFailureLv())
                {
                case SERIOUS:
                case MEDIUM:
                case SLIGHT:
                    VoicePlayThread::m_playContentList.append(failureHistoryRecordClass.getFailureName());
                    break;
                case PROMPT:
                    break;
                }
#if 0
                //将故障前后的数据进行记录
                recordRealDataFailureHistoryList.append(failureHistoryRecordClass);
 #endif
            }
        }
        qDebug() << "dddddddddddddddddddddddddd";
#if 0
        //所有的故障筛选完毕，将符合条件的故障存放到列表中，以便定时器到时进行遍历，将数据写到文件中
        m_recordRealDataFailureHistoryList.append(recordRealDataFailureHistoryList);
#endif
        //处理完成后,现在的列表中存在的都是可以进行展示和存储的故障
        DatabasesOperate::insertFailureHistoryListToDB(m_failureHistoryRecordInfoList);

    }else       //旧的故障列表为空
    {
        qDebug() << "eeeeeeeeeeeeeeee";
        m_failureHistoryRecordInfoList = failureHistoryRecordInfoList;
        foreach (FailureHistoryRecordClass failureHistoryRecordClass, m_failureHistoryRecordInfoList) {
            //播报故障
            //播报一级界面新的故障
            switch(failureHistoryRecordClass.getFailureLv())
            {
            case SERIOUS:
            case MEDIUM:
            case SLIGHT:
                VoicePlayThread::m_playContentList.append(failureHistoryRecordClass.getFailureName());
                break;
            case PROMPT:
                break;
            }
        }
    }
    qDebug() << "out failureDetectAnalysis()";
#if LOGW
        LogFile::debug("out故障检测解析 failureDetectAnalysis()");
#endif
}

void DataAnalysis::deviceStateAnalysis()
{
#if LOGW
        LogFile::debug("into设备状态检测解析 deviceStateAnalysis()");
#endif

    foreach (DeviceStateInfoClass deviceStateInfo, m_deviceStateInfoList) {

        if( m_mapBusIdAndRealtimeDataInfo.keys().contains(deviceStateInfo.getBusId()))
        {
            m_mapBusIdAndRealtimeDataInfo[deviceStateInfo.getBusId()].setSetDeviceState(false);
        }
    }

    //通过`device_state_info`(设备状态码表) 和 `device_state_trigger_condition`(设备状态触发条件)进行设备状态检测解析
    for (int i = 0; i < m_deviceStateInfoList.size(); i++)
    {
        DeviceStateInfoClass deviceStateInfo = m_deviceStateInfoList.at(i);

        //获取设备状态触发条件
        DeviceStateTriggerConditionInfoList deviceStateTriggerConditionInfoList
                = m_mapDeviceStateIdAndDeviceStateTriggerConditionInfoList[deviceStateInfo.getDeviceStateId()];

        bool isConditionsHold = true;
        //遍历判断设备状态触发条件
        for (int j = 0; j < deviceStateTriggerConditionInfoList.size(); j++)
        {
            DeviceStateTriggerConditionClass deviceStateTriggerCondition = deviceStateTriggerConditionInfoList.at(j);
            //单个条件成立标志位
            bool isSingleConditionHole = true;

            switch(deviceStateTriggerCondition.getLogicalRelationship())
            {
            case GREATER_OR_EQUAL:      //大于等于
                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() >= deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case GREATER:       //大于
                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() > deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS_OR_EQUAL:     //小于等于
                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() <= deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case LESS:          //小于
                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() < deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case EQUAL: //等于

                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() == deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            case NOT_EQUAL:
                if(m_mapKeyAndStrNumericalValue[deviceStateTriggerCondition.getBusId()].toDouble() != deviceStateTriggerCondition.getTriggerValue())
                {
                    isSingleConditionHole = true;
                }else
                {
                    isSingleConditionHole = false;
                }
                break;
            default:break;
            }

            //单个条件不成立
            if (!isSingleConditionHole)
            {
                isConditionsHold = false;
                break;
            }
        }

        //条件成立
        if (isConditionsHold)
        {
            m_mapBusIdAndRealtimeDataInfo[deviceStateInfo.getBusId()] = RealtimeDataClass(
                        deviceStateInfo.getBusId(),
                        deviceStateInfo.getColor(),
                        deviceStateInfo.getImageName(),
                        deviceStateInfo.getDeviceStateDesc());
            m_mapBusIdAndRealtimeDataInfo[deviceStateInfo.getBusId()].setSetDeviceState(true);

        }else
        {
            if( m_mapBusIdAndRealtimeDataInfo.keys().contains(deviceStateInfo.getBusId()) && !m_mapBusIdAndRealtimeDataInfo[deviceStateInfo.getBusId()].getSetDeviceState())
            {
                m_mapBusIdAndRealtimeDataInfo[deviceStateInfo.getBusId()].setColor(0);
            }
        }
    }

#if LOGW
        LogFile::debug("out设备状态检测解析 deviceStateAnalysis()");
#endif
}

void DataAnalysis::ladderDiagramAnalysis()
{
#if LOGW
        LogFile::debug("into梯形图状态解析 ladderDiagramAnalysis()");
#endif
    //通过`ladder_diagram`(梯形图码表)进行梯形图元件状态解析
    for (int i = 0; i < m_ladderDiagramPartStateInfoList.size(); i++)
    {
        LadderDiagramClass ladderDiagramInfo = m_ladderDiagramPartStateInfoList.at(i);

        switch(ladderDiagramInfo.getPartTypeInt())
        {
        case NORMALLY_CLOSED_CONTACT:
        case NORMALLY_CLOSED_CONTACT_ADD_VERTICAL_LINE:
            ladderDiagramInfo.setColor(
                        m_mapKeyAndStrNumericalValue[ladderDiagramInfo.getBusId()].toInt() == 0?GREEN:WHITE);
            break;

        case NORMALLY_OPEN_CONTACT:
        case NORMALLY_OPEN_CONTACT_ADD_VERTICAL_LINE:
        case REPEATER:
        case OUTPUT:
            ladderDiagramInfo.setColor(
                        m_mapKeyAndStrNumericalValue[ladderDiagramInfo.getBusId()].toInt() == 0?WHITE:GREEN);
            break;

        default:
            break;
        }

        m_ladderDiagramPartStateInfoList.replace(i, ladderDiagramInfo);
    }

#if LOGW
        LogFile::debug("out梯形图状态解析 ladderDiagramAnalysis()");
#endif
}

/***********************************************************************************************
 * dblNumericalValue 在m_mapKeyAndStrNumericalValue中保存
 * color、imageName和deviceStateDesc 在m_mapBusIdAndRealtimeDataInfo中保存
 * dataKey、categoryId、pageNum、xCoord、yCoord、dataDisplayMethod 在m_allBaseInfoList中保存
 * 以界面为单位打包数据，存储到对应界面的缓存
***********************************************************************************************/
void DataAnalysis::dataPackage()
{
#if LOGW
        LogFile::debug("into打包数据 dataPackage()");
#endif
    for (int i = 0; i < m_allBaseInfoList.size(); i++)
    {
        BaseInfoClass baseInfo = m_allBaseInfoList.at(i);
        RealtimeDataClass realtimeDataInfo = RealtimeDataClass(
                    baseInfo.getBusId(),
                    baseInfo.getCategoryId(),
                    baseInfo.getPageNum(),
                    baseInfo.getXCoord(),
                    baseInfo.getYCoord(),
                    baseInfo.getDataDisplayMethod(),
                    m_mapKeyAndStrNumericalValue.value(baseInfo.getBusId()),
                    m_mapBusIdAndRealtimeDataInfo.value(baseInfo.getBusId()).getColor(),
                    m_mapBusIdAndRealtimeDataInfo.value(baseInfo.getBusId()).getImageName(),
                    m_mapBusIdAndRealtimeDataInfo.value(baseInfo.getBusId()).getDeviceStateDesc());

        //处理以16进制数形式显示的数据
        if (baseInfo.getDataDisplayMethod() == ONLY_DISPLAY_HEX_NUMERICAL_VALUE)
        {
            realtimeDataInfo.setStrNumericalValue("0x"
                                                  + QString::number((int)realtimeDataInfo.getStrNumericalValue().trimmed().toDouble(), 16)
                                                  .rightJustified(2, '0').toUpper());
        }

        RealtimeDataInfoList *singleWidgetRealtimeDataInfoList = NULL;
        switch(baseInfo.getCategoryId())
        {
        case PRIMARY_INTERFACE:
            singleWidgetRealtimeDataInfoList = &m_primaryWidgetRealtimeDataList;
            break;

        case SECONDARY_INTERFACE:
            singleWidgetRealtimeDataInfoList = &m_secondaryWidgetRealtimeDataList;
            break;

        case BUS_ID_INTERFACE:
            //Can总线数据   0:白色   1:绿色
            realtimeDataInfo.setColor(m_mapKeyAndStrNumericalValue.value(baseInfo.getBusId()).toInt() == 0?WHITE:GREEN);
            singleWidgetRealtimeDataInfoList = &m_busIdWidgetRealtimeDataList;
            break;

        case INTERFACE_LAMP_HOLE:
            //Can总线数据   0:白色   1:绿色
            realtimeDataInfo.setColor(m_mapKeyAndStrNumericalValue.value(baseInfo.getBusId()).toInt() == 0?WHITE:GREEN);
            singleWidgetRealtimeDataInfoList = &m_interfaceLampHoleWidgetRealtimeDataList;
            break;

        case DCU:
        case DCU2:
            singleWidgetRealtimeDataInfoList = &m_dcuWidgetRealtimeDataList;
            break;

        case GCU:
            singleWidgetRealtimeDataInfoList = &m_gcuWidgetRealtimeDataList;
            break;

        case LG:
            singleWidgetRealtimeDataInfoList = &m_lgWidgetRealtimeDataList;
            break;

        default:
            break;
        }
        if( singleWidgetRealtimeDataInfoList->contains(realtimeDataInfo) )
        {
            singleWidgetRealtimeDataInfoList->replace(singleWidgetRealtimeDataInfoList->indexOf(realtimeDataInfo),realtimeDataInfo);
        }else
        {
            singleWidgetRealtimeDataInfoList->append(realtimeDataInfo);
        }
    }

#if LOGW
        LogFile::debug("out打包数据 dataPackage()");
#endif
}

void DataAnalysis::recordRealData(const QDateTime &dateTime,const QString &data)
{
    QString recordContent;
    recordContent.clear();
    recordContent.append(dateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    recordContent.append(",");
    recordContent.append(data);

    //先将超出时间间隔的数据删除，在给定时间间隔内保留最新的实时数据。该时间间隔为:记录故障发生前后的数据的时间
    //FAILURE_RECORDDATA_TIME_POINT*2/REQUEST_BUS_DATA_TIME_INTERVAL*REALDATALIST_LENGTH_MULTIPLE  时间间隔除请求数据的间隔即为实时数据列表的大小,同时扩大5倍，保证能够取得足够的数据
    if(m_realDataList.size() >= ((FAILURE_RECORDDATA_TIME_POINT*2/REQUEST_BUS_DATA_TIME_INTERVAL+1)*REALDATALIST_LENGTH_MULTIPLE) )
    {
        //将最旧的数据给删除了
        m_realDataList.removeFirst();
    }
    //追加新的数据
    m_realDataList.append(recordContent);

    //将数据记录到SD卡
#if WRITEREALDATA_TOSD
     CBaseOperate::writeRealDataToSD(recordContent);
#endif
}

void DataAnalysis::HandleFailureRealTimeData(const FailureHistoryRecordInfoList &recordRealDataFailureHistoryList)
{
    QStringList frontQStringTimeAndRealDataList;      //前段时间间隔的格式化时间和实时数据
    QStringList afterQStringTimeAndRealDataList;      //后段时间间隔的格式化时间和实时数据
    RealDataRecordClass realDataRecord;

    QStringList realDataList = m_realDataList;      //获得当前的实时数据列表
    int realDataListLength = realDataList.size();
    QString failureOccurData;
    int failureOccurPos = 0;        //标记故障发生时，该数据在列表中的位置
    QString recordContent;
    QStringList contentList;

    failureOccurData.clear();

    //从列表中获取一个故障信息，所有的故障信息中，实时数据是一样的
    failureOccurData = recordRealDataFailureHistoryList.at(0).getCurrentRealData();

    //现在实时数据列表中的数据都是需要保存的数据,取出数据并保存到RealDataRecordClass中
    for(int i = 0; i < realDataListLength;i++)  //遍历列表
    {
        recordContent.clear();
        contentList.clear();
        recordContent = realDataList.at(i);
        contentList = recordContent.split(",",QString::SkipEmptyParts);

        //利用故障发生当时的实时数据在列表中找到该故障发生的时间
        if( failureOccurData.compare(contentList.at(1)) == 0 )
        {
            failureOccurPos = i;
            break;
        }
    }
    //后半段的数据
    for(int i = failureOccurPos+1;i <= FAILURE_RECORDDATA_TIME_POINT/REQUEST_BUS_DATA_TIME_INTERVAL+failureOccurPos;i++)
    {
        if(i >= realDataListLength)
        {
            break;
        }
        afterQStringTimeAndRealDataList.append(realDataList.at(i));
    }

    for(int i = failureOccurPos-1;i >= failureOccurPos - FAILURE_RECORDDATA_TIME_POINT/REQUEST_BUS_DATA_TIME_INTERVAL;i--)
    {
        if(i < 0)
        {
            break;
        }
        frontQStringTimeAndRealDataList.prepend(realDataList.at(i));//向前追加
    }

    realDataRecord.setFrontQStringTimeAndRealDataList(frontQStringTimeAndRealDataList);
    realDataRecord.setFailureHistoryInfoList(recordRealDataFailureHistoryList);
    realDataRecord.setAfterQStringTimeAndRealDataList(afterQStringTimeAndRealDataList);

    //将数据写入文件中
    CBaseOperate::writeRealDataToFile(realDataRecord);
}


