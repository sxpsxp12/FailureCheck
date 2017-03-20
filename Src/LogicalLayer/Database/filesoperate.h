#ifndef FILESOPERATE_H
#define FILESOPERATE_H

#include <QFile>
#include <QStringList>
#include <QTextCodec>
#include <QString>


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
#include "fileinfoclass.h"

class FilesOperate
{
public:
    FilesOperate(const QString &goalPath);
    virtual ~FilesOperate();

    void initFiles();
    FileInfoClassList initDatabases();

    FileInfoClass initLadderDiagramClass();
    FileInfoClass initSchematicClass();
    FileInfoClassList initLogicTxtData();
    FileInfoClassList initOtherTxtData();

private:
    QTextCodec* code;
    QString m_goalPath;
    BusInfoList busInfoList;
    FailureInfoList failureInfoList;
    FailureTriggerConditionInfoList failureTriggerConditionInfoList;
    FailureProcessInfoList failureProcessInfoList;
    BaseInfoList baseInfoList;
    SchematicInfoList schematicInfoList;
    DelayInfoList delayInfoList;
    DelayTriggerConditionInfoList delayTriggerConditionInfoList;
    LadderDiagramInfoList ladderDiagramClassList;
    SelfHoldInfoList selfHoldInfoList;
    SelfHoldTriggerConditionInfoList selfHoldTriggerConditionInfoList;
    DeviceStateInfoList deviceStateInfoList;
    DeviceStateTriggerConditionInfoList deviceStateTriggerConditionInfoList;

    QFile *checkTxtFile;
    QFile *dcuinforTxtFile;
    QFile *hostinforTxtFile;
    QFile *k1inforTxtFile;
    QFile *k2inforTxtFile;
    QFile *lcuinforTxtFile;
    QFile *lginforTxtFile;
    QFile *logicTxtFile;
    QFile *pointTxtFile;
    QFile *schemTxtFile;
};

#endif // FILESOPERATE_H
