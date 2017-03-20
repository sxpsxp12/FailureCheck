#ifndef REQUESTDATATHREAD_H
#define REQUESTDATATHREAD_H

#include <QThread>
#include <QMap>
#include <QStringList>

#include "realtimedataclass.h"
#include "failureinfoclass.h"
#include "ladderdiagramclass.h"
#include "canbusdatapackageclass.h"
#include "failurehistoryrecordclass.h"
#include "fileinfoclass.h"

class DataAnalysis;
class QTimer;

class RequestDataThread : public QThread
{
    Q_OBJECT
public:
    explicit RequestDataThread(QObject *parent = 0);
    ~RequestDataThread();


    //界面跳转后要对这个标志位重新赋值
    static int CurrentInterfaceEnumValue;
    static MapAddressAndCanBusDataPackage m_mapAddressAndCanBusDataPackage;

    void startRequestData();
    void stopRequestData();

//    void setUSBGoalPath(const QString &goalPath);

protected:
    void run();

signals:
//    void signal_sendPrimaryWidgetData(RealtimeDataInfoList realtimeDataList,
//                                      FailureInfoList failureInfoList);
    void signal_sendPrimaryWidgetData();

//    void signal_sendSecondaryWidgetData(RealtimeDataInfoList realtimeDataList,
//                                        FailureInfoList failureInfoList);
    void signal_sendSecondaryWidgetData();

//    void signal_sendBusIdWidgetData(RealtimeDataInfoList realtimeDataList);
    void signal_sendBusIdWidgetData();

    //    void signal_sendInterfaceLampHoleWidgetData(RealtimeDataInfoList realtimeDataList);
    void signal_sendInterfaceLampHoleWidgetData();

//    void signal_sendDcuWidgetData(RealtimeDataInfoList realtimeDataList);
    void signal_sendDcuWidgetData();

//    void signal_sendGcuWidgetData(RealtimeDataInfoList realtimeDataList);
    void signal_sendGcuWidgetData();

    //void signal_sendLgWidgetData(RealtimeDataInfoList realtimeDataList);
    void signal_sendLgWidgetData();

//    void signal_sendLadderDiagramWidgetData(LadderDiagramInfoList realtimeDataList);
    void signal_sendLadderDiagramWidgetData();

    void signal_sendGUIContainerTitleBarStatus(int guiContainerTitleBarName,int guiContainerTitleBarStatus);

//    void signal_sendFailureRecordWidgetData(FailureHistoryRecordInfoList failureHistoryRecordInfoList);
    void signal_sendFailureRecordWidgetData();

    //void signal_sendInitDataBaseIsSuccess(FileInfoClassList fileInfoList);

    void signal_sendMasterFlag(bool flag);

private slots:
    void slot_useThreadResources();

private:
    DataAnalysis *m_dataAnalysis;

    void initParameter();

    int m_LastLCUDataFlag;     //LCU数据标志
    int m_LastDCUDataFlag;     //DCU数据标志
    int m_LastDCU2DataFlag;     //DCU2数据标志
    int m_LastHostDataFlag;     //主机数据标志
    int m_LCUAbnormalTime;         //LCU异常次数
    int m_DCUAbnormalTime;         //DCU异常次数
    int m_DCU2AbnormalTime;         //DCU2异常次数
    int m_HostAbnormalTime;         //主机异常次数

    bool m_isHandshakeSuccess;
    int m_handshakeTime;
    int m_requestDataSuccessTime;
    int m_againRequestDataFault;

    void InitCanData(QString &data);

    void canInit();
    bool m_isCanOpened;

    void canReadData();

    void canWriteData();

    void canClose();

    QTimer *m_timerRequestBusData;

    void setGuiContainerTitleBarStatus();   //设置容器的标题栏

    //是否请求数据标志位
//    bool m_isRequestData;
//    bool m_isImportTxt;
//    QString m_goalPath;

    bool m_isMasterMachine;
    bool m_isRequestMasterMachine;

};

#endif // REQUESTDATATHREAD_H
