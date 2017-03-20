#ifndef DATAANALYSIS_H
#define DATAANALYSIS_H

#include "businfoclass.h"
#include "delayinfoclass.h"
#include "delaytriggerconditionclass.h"
#include "failureinfoclass.h"
#include "failuretriggerconditionclass.h"
#include "realtimedataclass.h"
#include "selfholdinfoclass.h"
#include "selfholdtriggerconditionclass.h"
#include "devicestateinfoclass.h"
#include "devicestatetriggerconditionclass.h"
#include "baseinfoclass.h"
#include "ladderdiagramclass.h"
#include "canbusdatapackageclass.h"
#include "failurehistoryrecordclass.h"
#include "realdatarecordclass.h"

#include <QMap>
#include <QObject>
#include <QStringList>
#include <QDateTime>
#include "cbaseoperate.h"

class QTimer;

class DataAnalysis : public QObject
{
    Q_OBJECT
public:
    DataAnalysis();
    ~DataAnalysis();

    void dataAnalysisInit();
    void recordRealData(const QDateTime &dateTime,const QString &data);
    void dataAnalysis(const QDateTime &dateTime,const QString &data);

    //一级界面中分为实时数据和故障信息  只显示严重、中等、轻微等级的故障
    //经过逻辑运算后的数据存放在List中，以界面为单位存放，包括算数数值，颜色，图片名称，设备状态描述
    static RealtimeDataInfoList m_primaryWidgetRealtimeDataList;
    static FailureInfoList m_primaryWidgetFailureInfoList;

    //二级界面中分为实时数据和故障信息  只显示提示等级的故障
    static RealtimeDataInfoList m_secondaryWidgetRealtimeDataList;
    static FailureInfoList m_secondaryWidgetFailureInfoList;

    static RealtimeDataInfoList m_busIdWidgetRealtimeDataList;
    static RealtimeDataInfoList m_interfaceLampHoleWidgetRealtimeDataList;

    static RealtimeDataInfoList m_dcuWidgetRealtimeDataList;
    static RealtimeDataInfoList m_gcuWidgetRealtimeDataList;

    static RealtimeDataInfoList m_lgWidgetRealtimeDataList;
    static LadderDiagramInfoList m_ladderDiagramPartStateInfoList;

    //故障历史记录
    static FailureHistoryRecordInfoList m_failureHistoryRecordInfoList;
#if 0
    RealtimeDataInfoList getPrimaryWidgetRealtimeDataList() const;
    FailureInfoList getPrimaryWidgetFailureInfoList() const;

    RealtimeDataInfoList getSecondaryWidgetRealtimeDataList() const;
    FailureInfoList getSecondaryWidgetFailureInfoList() const;

    RealtimeDataInfoList getBusIdWidgetRealtimeDataList() const;

    RealtimeDataInfoList getInterfaceLampHoleWidgetRealtimeDataList() const;

    RealtimeDataInfoList getDcuWidgetRealtimeDataList() const;

    RealtimeDataInfoList getGcuWidgetRealtimeDataList() const;

    RealtimeDataInfoList getLgWidgetRealtimeDataList() const;

    LadderDiagramInfoList getLadderDiagramPartStateInfoList() const;

    FailureHistoryRecordInfoList getFailureHistoryRecordInfoList() const;
#endif

private slots:
    void slot_timerDelayTimeOut();

    void slot_timerRealDataTimeOut();

private:

    //基本信息表全部信息
    BaseInfoList m_allBaseInfoList;

    //算数解析：完成基本算数运算
    void arithmeticAnalysis();
    //经过算数运算后，解析好的数据存放于Map中
    //键：包括真实线号(404)，延迟ID(DT0)，自锁ID(S528)
    //值：对应键的实时数据，只有算数数值
    QMap<QString, QString> m_mapKeyAndStrNumericalValue;
    BusInfoList m_busInfoList;

    //延迟解析
    void delayAnalysis();
    DelayInfoList m_delayInfoList;
    QMap<QString, DelayTriggerConditionInfoList> m_mapDelayIdAndDelayTriggerConditionInfoList;
    QTimer *m_timerDelay;

    //自锁解析
    void selfHoldAnalysis();
    SelfHoldInfoList m_selfHoldInfoList;
    QMap<QString, SelfHoldTriggerConditionInfoList> m_mapSelfHoldIdAndSelfHoldTriggerConditionInfoList;

    //逻辑解析
    //故障检测
    void failureDetectAnalysis();
    FailureInfoList m_failureInfoList;
    QMap<QString, FailureTriggerConditionInfoList> m_mapFailureIdAndFailureTriggerConditionInfoList;

    //设备状态检测
    void deviceStateAnalysis();
    DeviceStateInfoList m_deviceStateInfoList;
    QMap<QString, DeviceStateTriggerConditionInfoList> m_mapDeviceStateIdAndDeviceStateTriggerConditionInfoList;
    QMap<QString, RealtimeDataClass> m_mapBusIdAndRealtimeDataInfo;

    //梯形图解析
    void ladderDiagramAnalysis();

    void dataPackage();

    FailureInfoList m_LastsecondaryWidgetFailureInfoList;
    FailureInfoList m_LastprimaryWidgetFailureInfoList;

    //该列表中存放记录实时数据的故障
    FailureHistoryRecordInfoList m_recordRealDataFailureHistoryList;
    //实时数据列表
    QStringList m_realDataList; //记录实时数据的列表
    void HandleFailureRealTimeData(const FailureHistoryRecordInfoList &recordRealDataFailureHistoryList);  //处理故障实时数据
    QTimer *m_timerRealData;
    QString m_currentRealData;      //当前正在解析的实时数据
    QDateTime m_questDataTime;      //请求数据的时间
};

#endif // DATAANALYSIS_H
