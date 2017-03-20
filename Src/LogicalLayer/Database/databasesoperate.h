#ifndef DATABASESOPERATE_H
#define DATABASESOPERATE_H

#include <QMap>
#include <QString>
#include <QStringList>

#include "ladderdiagramclass.h"
#include "schematicclass.h"
#include "delayinfoclass.h"
#include "delaytriggerconditionclass.h"
#include "failuretriggerconditionclass.h"
#include "failureprocessclass.h"
#include "failureinfoclass.h"
#include "businfoclass.h"
#include "baseinfoclass.h"
#include "selfholdinfoclass.h"
#include "selfholdtriggerconditionclass.h"
#include "devicestateinfoclass.h"
#include "devicestatetriggerconditionclass.h"
#include "failurehistoryrecordclass.h"

class QSqlDatabase;

class DatabasesOperate
{
public:
    static QSqlDatabase db;

    //连接数据库
    static bool connectDatabase();
    static bool clearDataBaseAllTables();
    static bool createDataBaseAllTables();

    //数据导入数据库
    static bool initDatabasesLadderDiagram(LadderDiagramInfoList &ladderDiagramClassList);
    static bool initDatabasesSchematic(SchematicInfoList &schematicInfoList);
    static bool initDatabasesDelayInfo(DelayInfoList &delayInfoList);
    static bool initDatabasesDelayTriggerCondition(DelayTriggerConditionInfoList &delayTriggerConditionInfoList);
    static bool initDatabasesFailureTriggerCondition(FailureTriggerConditionInfoList &failureTriggerConditionInfoList);
    static bool initDatabasesFailureProcess(FailureProcessInfoList & failureProcessInfoList);
    static bool initDatabasesFailureInfo(FailureInfoList &failureInfoList);
    static bool initDatabasesBusInfo(BusInfoList &busInfoList);
    static bool initDatabasesBaseInfo(BaseInfoList &baseInfoList);
    static bool initDatabasesSelfHoldInfo(SelfHoldInfoList &selfHoldInfoList);
    static bool initDatabasesSelfHoldTriggerCondition(SelfHoldTriggerConditionInfoList &selfHoldTriggerConditionInfoList);
    static bool initDatabasesDeviceStateInfo(DeviceStateInfoList &deviceStateInfoList);
    static bool initDatabasesDeviceStateTriggerCondition(DeviceStateTriggerConditionInfoList &deviceStateTriggerConditionInfoList);

    //获取车辆信息
    static bool getVerhicleInfo(QMap<QString,QString> &verhicleInfoMapKeyAndValue);
    static bool insertVerhicleInfoToDataBase(QMap<QString, QString> &verhicleInfoMapKeyAndValue);
    //获取基本信息表，传入界面Id,返回对应界面的基本信息表数据
    static bool initInterfaceBaseInfoList(BaseInfoList &interfaceBaseInfoList, int interfaceName);

    //获取故障处理方法
    static bool initFailureProcessInfoList(FailureProcessInfoList &failureProcessInfoList, QString failureId);

    //获取故障信息等级
    static bool getFailureInfo(FailureInfoClass &failureInfo, QString failureName);

    static bool getLadderDiagramInfo(const int &pageNum, LadderDiagramInfoList &ladderDiagramInputInfoList,
                                     LadderDiagramInfoList &LadderDiagramOutPutInfoList);

    static bool getLadderDiagramMaxPageNum(int &maxPageNum);
    static bool getLadderDiagramAllListInfo(LadderDiagramInfoList &allListInfo); //获取梯形图搜索时的页的列表

    //解析数据使用  将数据库中的数据初始化到缓存中
    static bool initBusInfoList(BusInfoList &busInfoList);

    static bool initDelayInfoList(DelayInfoList &delayInfoList);

    static bool initMapDelayIdAndDelayTriggerConditionInfoList(
            QMap<QString, DelayTriggerConditionInfoList> &mapDelayIdAndDelayTriggerConditionInfoList,
            const DelayInfoList &delayInfoList);

    static bool initSelfHoldInfoList(SelfHoldInfoList &selfHoldInfoList);

    static bool initMapSelfHoldIdAndSelfHoldTriggerConditionInfoList(
            QMap<QString, SelfHoldTriggerConditionInfoList> &mapSelfHoldIdAndSelfHoldTriggerConditionInfoList,
            const SelfHoldInfoList &selfHoldInfoList);

    static bool initFailureInfoList(FailureInfoList &failureInfoList);

    static bool initMapFailureIdAndFailureTriggerConditionInfoList(
            QMap<QString, FailureTriggerConditionInfoList> &mapFailureIdAndFailureTriggerConditionInfoList,
            const FailureInfoList &failureInfoList);

    static bool initDeviceStateInfoList(DeviceStateInfoList &deviceStateInfoList);

    static bool initMapDeviceStateIdAndDeviceStateTriggerConditionInfoList(
            QMap<QString, DeviceStateTriggerConditionInfoList> &mapDeviceStateIdAndDeviceStateTriggerConditionInfoList,
            const DeviceStateInfoList &deviceStateInfoList);

    //初始化基本信息表 获取全部的基本信息表数据
    static bool initAllBaseInfoList(BaseInfoList &allBaseInfoList);

    //初始化梯形图信息列表，用于解析梯形图元件状态
    static bool initLadderDiagramInfoList(LadderDiagramInfoList &ladderDiagramInfoList);

    //获取故障历史列表中的数据
    static bool initFailureHistoryList(FailureHistoryRecordInfoList &failureHistoryRecordInfoList);
    static bool insertFailureHistoryListToDB(FailureHistoryRecordInfoList &failureHistoryRecordInfoList);
};

#endif // DATABASESOPERATE_H
