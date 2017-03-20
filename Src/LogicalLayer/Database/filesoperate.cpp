#include "filesoperate.h"
#include "databasesoperate.h"
#include "enumtypeheader.h"
#include <QDebug>
#include "logfile.h"

#include <QCoreApplication>

FilesOperate::FilesOperate(const QString &goalPath)
{
    m_goalPath = goalPath;
}

FilesOperate::~FilesOperate()
{
    if(checkTxtFile != NULL)
    {
        delete checkTxtFile;
    }

    if(dcuinforTxtFile != NULL)
    {
        delete dcuinforTxtFile;
    }

    if(hostinforTxtFile != NULL)
    {
        delete hostinforTxtFile;
    }

    if(k1inforTxtFile != NULL)
    {
        delete k1inforTxtFile;
    }

    if(k2inforTxtFile != NULL)
    {
        delete k2inforTxtFile;
    }

    if(lcuinforTxtFile != NULL)
    {
        delete lcuinforTxtFile;
    }

    if(lginforTxtFile != NULL)
    {
        delete lginforTxtFile;
    }

    if(logicTxtFile != NULL)
    {
        delete logicTxtFile;
    }

    if(pointTxtFile != NULL)
    {
        delete pointTxtFile;
    }

    if(schemTxtFile != NULL)
    {
        delete schemTxtFile;
    }
}

void FilesOperate::initFiles()
{
    code = QTextCodec::codecForName("GBK");
    checkTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"check.txt"));
    dcuinforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"dcuinfor.txt"));
    hostinforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"hostinfor.txt"));
    k1inforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"k1infor.txt"));
    k2inforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"k2infor.txt"));
    lcuinforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"lcuinfor.txt"));
    lginforTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"lginfor.txt"));
    logicTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"logic.txt"));
    pointTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"point.txt"));
    schemTxtFile = new QFile(QString("%1/%2").arg(m_goalPath,"schem.txt"));
}

FileInfoClassList FilesOperate::initDatabases()
{
    FileInfoClassList fileInfoList;
    fileInfoList.clear();

    initFiles();

    fileInfoList.append(initLogicTxtData());
    fileInfoList.append(initOtherTxtData());
    fileInfoList.append(initSchematicClass());
    fileInfoList.append(initLadderDiagramClass());

    return fileInfoList;
}

FileInfoClass FilesOperate::initLadderDiagramClass()
{
    QString pageNumStr;
    QString x1CoordStr;
    QString x2CoordStr;
    QString yCoordStr;
    QString displayContentStr;
    QString partTypeStr;
    QString meaningStr;

    FileInfoClass *fileInfo = new FileInfoClass();
    fileInfo->seterrorFileName("point.txt");
    fileInfo->setFileFormatOk(true);
    fileInfo->setInitDataBaseOk(true);

    QStringList fileLineStringList;
    int rowNumber = 0;

    if (!pointTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open point txt file!";
#endif
#if LOG
        LogFile::debug("can't open point txt file!");
#endif
        return *fileInfo;
    }

    QString fileLine;
    QTextStream stream(pointTxtFile);
    stream.setCodec(code);       //对输出流的设置
    ladderDiagramClassList.clear();

    while (!stream.atEnd())
    {
        rowNumber++;
        fileLine = stream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != POINT_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("point.txt 文件错误!");
#endif
#if LOG
            LogFile::debug("point.txt 文件错误!");
#endif
            fileInfo->seterrorRowNum(rowNumber);
            fileInfo->setFileFormatOk(false);
            continue;
        }

        pageNumStr = fileLineStringList.at(0).simplified();
        x1CoordStr = fileLineStringList.at(1).simplified();
        yCoordStr = fileLineStringList.at(2).simplified();
        x2CoordStr = fileLineStringList.at(3).simplified();
        displayContentStr = fileLineStringList.at(4).simplified();
        partTypeStr = fileLineStringList.at(5).simplified();
        meaningStr = fileLineStringList.at(6).simplified();

        //代表这条数据为线
        if (displayContentStr == "A")
        {
            //竖线，Y坐标加1
            if (x1CoordStr == x2CoordStr)
            {
                yCoordStr = QString::number(yCoordStr.toInt() + 1);
                partTypeStr = QString::number(VERTICAL_LINE);
            }
            //横线
            else
            {
                partTypeStr = QString::number(HORIZONTAL_LINE);
            }

            displayContentStr.clear();
        }
        if (partTypeStr == ".")
        {
            partTypeStr.clear();
        }
        if (meaningStr == ".")
        {
            meaningStr.clear();
        }
        //输出元件
        if (x1CoordStr == "O")
        {
            x1CoordStr = "0";
        }

        LadderDiagramClass ladderDiagramClass(pageNumStr, x1CoordStr, x2CoordStr, yCoordStr,
                                              displayContentStr, partTypeStr, meaningStr);
        if (!ladderDiagramClassList.contains(ladderDiagramClass))
        {
            ladderDiagramClassList.append(ladderDiagramClass);
        }
        else
        {
#if DEBUG
            qDebug() << QString("There are duplicate data, In the document line %1, PageNnu:%2, X1Coord:%3, X2Coord:%4, YCoord:%5, displayContent:%6, partType:%7, meaning:%8")
                        .arg(QString::number(ladderDiagramClassList.size()),
                             ladderDiagramClass.getPageNumStr(),
                             ladderDiagramClass.getXCoordStr(),
                             ladderDiagramClass.getX2CoordStr(),
                             ladderDiagramClass.getYCoordStr(),
                             ladderDiagramClass.getBusId(),
                             ladderDiagramClass.getPartTypeStr(),
                             ladderDiagramClass.getMeaning());
#endif
#if LOG
            LogFile::debug(QString("There are duplicate data, In the document line %1, PageNnu:%2, X1Coord:%3, X2Coord:%4, YCoord:%5, displayContent:%6, partType:%7, meaning:%8")
                           .arg(QString::number(ladderDiagramClassList.size()),
                                ladderDiagramClass.getPageNumStr(),
                                ladderDiagramClass.getXCoordStr(),
                                ladderDiagramClass.getX2CoordStr(),
                                ladderDiagramClass.getYCoordStr(),
                                ladderDiagramClass.getBusId(),
                                ladderDiagramClass.getPartTypeStr(),
                                ladderDiagramClass.getMeaning()));
#endif
        }
    }
    pointTxtFile->close();

    qSort(ladderDiagramClassList.begin(), ladderDiagramClassList.end(),
          LadderDiagramClass::LadderDiagramClassInfoSort);

    //处理竖线和元件的冲突问题
    for (int i = 0; i < ladderDiagramClassList.size(); i++)
    {
        LadderDiagramClass ladderDiagramClass = ladderDiagramClassList.at(i);
        if (ladderDiagramClass.getPartTypeInt() == VERTICAL_LINE)
        {
            LadderDiagramClass nextLadderDiagramClass = ladderDiagramClassList.at(i + 1);
            //有冲突的元件
            if ( ladderDiagramClass.getPageNumStr() == nextLadderDiagramClass.getPageNumStr()
                 && ladderDiagramClass.getYCoordStr() == nextLadderDiagramClass.getYCoordStr()
                 && ladderDiagramClass.getXCoordStr() == nextLadderDiagramClass.getXCoordStr()
                 && ladderDiagramClass.getX2CoordInt() + 1 == nextLadderDiagramClass.getX2CoordInt() )
            {
                switch (nextLadderDiagramClass.getPartTypeInt())
                {
                case NORMALLY_CLOSED_CONTACT:
                    nextLadderDiagramClass.setPartTypeInt(NORMALLY_CLOSED_CONTACT_ADD_VERTICAL_LINE);
                    break;
                case NORMALLY_OPEN_CONTACT:
                    nextLadderDiagramClass.setPartTypeInt(NORMALLY_OPEN_CONTACT_ADD_VERTICAL_LINE);
                    break;
                case HORIZONTAL_LINE:
                    nextLadderDiagramClass.setPartTypeInt(VERTICAL_LINE_ADD_HORIZONTAL_LINE);
                    break;
                default:
                    break;
                }
                ladderDiagramClassList.replace(i + 1, nextLadderDiagramClass);
                ladderDiagramClassList.removeAt(i);
            }
        }
    }

    fileInfo->setInitDataBaseOk(DatabasesOperate::initDatabasesLadderDiagram(ladderDiagramClassList));
    return *fileInfo;
}

FileInfoClass FilesOperate::initSchematicClass()
{
    QString schematicCatalogStr;
    QString schematicNameStr;

    QStringList fileLineStringList;
    int rowNumber = 0;

    FileInfoClass *fileInfo = new FileInfoClass;
    fileInfo->setFileFormatOk(true);
    fileInfo->setInitDataBaseOk(true);
    fileInfo->seterrorFileName("schem.txt");

    if (!schemTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open schemTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open schemTxtFile!");
#endif
        return *fileInfo;
    }

    QString fileLine;
    QTextStream stream(schemTxtFile);
    stream.setCodec(code);       //对输出流的设置
    schematicInfoList.clear();

    while (!stream.atEnd())
    {
        rowNumber++;
        fileLine = stream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != SCHEM_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("schem.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("schem.txt 文件错误!"));
#endif
            fileInfo->seterrorRowNum(rowNumber);
            fileInfo->setFileFormatOk(false);
        }

        schematicCatalogStr = fileLineStringList.at(0).simplified();
        schematicNameStr = fileLineStringList.at(1).simplified();

        if ((schematicCatalogStr == ".") && (schematicNameStr == "."))
        {
            break;
        }
        else
        {
            if (schematicCatalogStr == ".") schematicCatalogStr.clear();
            if (schematicNameStr == ".") schematicNameStr.clear();
            schematicInfoList.append(SchematicClass(schematicCatalogStr, schematicNameStr));
        }
    }
    schemTxtFile->close();
    fileInfo->setInitDataBaseOk(DatabasesOperate::initDatabasesSchematic(schematicInfoList));
    return *fileInfo;
}

FileInfoClassList FilesOperate::initLogicTxtData()
{
    QString failureIdStr;
    QString failureNameStr;
    QString failureTriggerConditionNumStr;
    QString failureProcessMethodConditionNumStr;
    QString failureLvStr;
    QString outputBusIdStr;
    QString failureBusIdStr;
    QString failureLogicalRelationshipStr;
    QString failureTriggerValueStr;
    QString processPointStr;
    QString processMethodStr;
    QString processImageNameStr;

    QString delayIdStr;
    QString delayLogicalRelationshipStr;
    QString delayTimeStr;
    QString delayValueStr;
    QString delayConditionNumStr;
    QString busIdStr;
    QString busLogicalRelationshipStr;
    QString triggerValueStr;

    QString selfFlagIdStr;
    QString selfHoldIdStr;
    QString selfHoldValueStr;
    QString selfHoldConditionNumStr;
    QString selfHoldBusIdStr;
    QString logicalRelationshipStr;
    QString selfHoldTriggerValueStr;

    QString deviceStateIdStr;
    QString deviceStateBusIdStr;
    QString colorStr;
    QString imageNameStr;
    QString deviceStateDescStr;
    QString conditionNumStr;
    QString triggerBusIdStr;
    QString deviceStateLogicalRelationshipStr;
    QString deviceStateTriggerValueStr;

    QString categoryStr;                 //类型分类，如“G”,"Y","Z","K1","K2","W1","W2","LG"
    int rowNumber = 0;

    FileInfoClass *LogicFileInfo = new FileInfoClass;
    LogicFileInfo->setFileFormatOk(true);
    LogicFileInfo->setInitDataBaseOk(true);
    LogicFileInfo->seterrorFileName("logic.txt");

    FileInfoClass *checkFileInfo = new FileInfoClass;
    checkFileInfo->setFileFormatOk(true);
    checkFileInfo->setInitDataBaseOk(true);
    checkFileInfo->seterrorFileName("check.txt");

    FileInfoClassList fileInfoList;
    fileInfoList.clear();

    if (!logicTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open logicTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open logicTxtFile!");
#endif
        return fileInfoList;
    }
    if (!checkTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open checkTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open checkTxtFile!");
#endif
        return fileInfoList;
    }

    QString fileLine;
    QTextStream stream(logicTxtFile);
    stream.setCodec(code);       //对输出流的设置
    QStringList fileLineStringList;

    QString checkFileLine;
    QTextStream checkStream(checkTxtFile);
    checkStream.setCodec(code);       //对输出流的设置
    QStringList checkFileLineStringList;

    delayInfoList.clear();
    delayTriggerConditionInfoList.clear();
    selfHoldInfoList.clear();
    selfHoldTriggerConditionInfoList.clear();
    deviceStateInfoList.clear();
    deviceStateTriggerConditionInfoList.clear();
    failureInfoList.clear();
    failureProcessInfoList.clear();
    failureTriggerConditionInfoList.clear();

    int processMethodNumInt = 0;
    int realProcessMethodNumInt = 0;

    while (!stream.atEnd())
    {
        ////0511: GUI And Core
        QCoreApplication::processEvents();

        rowNumber++;
        fileLine = stream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != LOGIC_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("logic.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("logic.txt 文件错误!"));
#endif
            LogicFileInfo->seterrorRowNum(rowNumber);
            LogicFileInfo->setFileFormatOk(false);
        }

        categoryStr = fileLineStringList.at(4).simplified();

        if (categoryStr == "G")
        {
            checkFileLine = checkStream.readLine();
            checkFileLineStringList = checkFileLine.split(' ', QString::SkipEmptyParts);

            if (checkFileLineStringList.size() != CHECK_TXT_COLUMN_NUM)
            {
#if DEBUG
                qDebug() << QString::fromUtf8("check.txt 文件错误!");
#endif
#if LOG
                LogFile::debug(QString::fromUtf8("check.txt 文件错误!"));
#endif
                checkFileInfo->setFileFormatOk(false);
                checkFileInfo->seterrorRowNum(rowNumber);
            }

            failureIdStr = checkFileLineStringList.at(0).simplified();
            failureNameStr = checkFileLineStringList.at(2).simplified();
            failureTriggerConditionNumStr = fileLineStringList.at(5).simplified();
            failureLvStr = checkFileLineStringList.at(3).simplified();
            outputBusIdStr = checkFileLineStringList.at(1).simplified();

            processMethodNumInt = (CHECK_TXT_COLUMN_NUM - 4) / 3;
            realProcessMethodNumInt = 0;

            for (int i = 0; i < processMethodNumInt; i++)
            {
                realProcessMethodNumInt++;
                processPointStr = checkFileLineStringList.at(4+3*i).simplified();
                processMethodStr = checkFileLineStringList.at(5+3*i).simplified();
                processImageNameStr = checkFileLineStringList.at(6+3*i).simplified();

                if ((processPointStr == ".") && (processMethodStr == ".") && (processImageNameStr == "."))
                {
                    realProcessMethodNumInt--;
                    break;
                }
                failureProcessInfoList.append(FailureProcessClass(failureIdStr, processPointStr,
                                                                  processMethodStr, processImageNameStr));
            }

            failureProcessMethodConditionNumStr = QString::number(realProcessMethodNumInt);

            for (int i = 0; i < failureTriggerConditionNumStr.toInt(); i++)
            {
                failureBusIdStr = fileLineStringList.at(6+6*i).simplified();
                failureLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                failureTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (failureLogicalRelationshipStr == ".")
                    failureLogicalRelationshipStr = "5";            //等于 ==

                failureTriggerConditionInfoList.append(FailureTriggerConditionClass(failureIdStr, failureBusIdStr,
                                                                                    failureLogicalRelationshipStr, failureTriggerValueStr));
            }
            failureInfoList.append(FailureInfoClass(failureIdStr, failureNameStr, failureTriggerConditionNumStr.toInt(),
                                                    failureProcessMethodConditionNumStr.toInt(), failureLvStr.toInt(),
                                                    outputBusIdStr));
        }
        else if (categoryStr == "Y")
        {
            delayIdStr = fileLineStringList.at(1).simplified();
            delayLogicalRelationshipStr = fileLineStringList.at(3).simplified();
            delayTimeStr = fileLineStringList.at(2).simplified();
            delayConditionNumStr = fileLineStringList.at(5).simplified();

            delayValueStr = "1";

            for (int i = 0; i < delayConditionNumStr.toInt(); i++)
            {
                busIdStr = fileLineStringList.at(6+6*i).simplified();
                busLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                triggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (busLogicalRelationshipStr == ".")
                    busLogicalRelationshipStr = "5";            //等于 ==

                delayTriggerConditionInfoList.append(DelayTriggerConditionClass(delayIdStr, busIdStr,
                                                                                busLogicalRelationshipStr, triggerValueStr));
            }
            delayInfoList.append(DelayInfoClass(delayIdStr, delayLogicalRelationshipStr.toInt(), delayTimeStr.toInt(),
                                                delayValueStr.toInt(), delayConditionNumStr.toInt()));
        }
        else if (categoryStr == "Z")
        {
            selfFlagIdStr = fileLineStringList.at(0).simplified();
            selfHoldIdStr = fileLineStringList.at(1).simplified();
            selfHoldValueStr = fileLineStringList.at(2).simplified();
            selfHoldConditionNumStr = fileLineStringList.at(5).simplified();

            for (int i = 0; i < selfHoldConditionNumStr.toInt(); i++)
            {
                selfHoldBusIdStr = fileLineStringList.at(6+6*i).simplified();
                logicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                selfHoldTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (logicalRelationshipStr == ".")
                    logicalRelationshipStr = "5";            //等于 ==

                selfHoldTriggerConditionInfoList.append(SelfHoldTriggerConditionClass(selfFlagIdStr, selfHoldBusIdStr,
                                                                                      logicalRelationshipStr, selfHoldTriggerValueStr));
            }
            selfHoldInfoList.append(SelfHoldInfoClass(selfFlagIdStr, selfHoldIdStr, selfHoldValueStr.toInt(),
                                                      selfHoldConditionNumStr.toInt()));
        }
        else if (categoryStr == "K2")
        {
            deviceStateIdStr = fileLineStringList.at(0).simplified();
            deviceStateBusIdStr = fileLineStringList.at(1).simplified();
            conditionNumStr = fileLineStringList.at(5).simplified();
            colorStr = fileLineStringList.at(2).simplified();

            imageNameStr.clear();
            deviceStateDescStr.clear();

            for (int i = 0; i < conditionNumStr.toInt(); i++)
            {
                triggerBusIdStr = fileLineStringList.at(6+6*i).simplified();
                deviceStateLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                deviceStateTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (deviceStateLogicalRelationshipStr == ".")
                    deviceStateLogicalRelationshipStr = "5";            //等于 ==

                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(deviceStateIdStr, triggerBusIdStr,
                                                                                            deviceStateLogicalRelationshipStr.toInt(), deviceStateTriggerValueStr.toDouble()));
            }
            deviceStateInfoList.append(DeviceStateInfoClass(deviceStateIdStr, deviceStateBusIdStr,
                                                            colorStr.toInt(), imageNameStr,
                                                            deviceStateDescStr, conditionNumStr.toInt()));
        }
        else if (categoryStr == "K1")
        {
            deviceStateIdStr = fileLineStringList.at(0).simplified();
            deviceStateBusIdStr = fileLineStringList.at(1).simplified();
            conditionNumStr = fileLineStringList.at(5).simplified();
            colorStr = fileLineStringList.at(2).simplified();

            if (deviceStateBusIdStr == "train")
            {
                imageNameStr = "train" + fileLineStringList.at(3).simplified();
            }
            else
            {
                imageNameStr = fileLineStringList.at(3).simplified();
            }

            deviceStateDescStr.clear();

            for (int i = 0; i < conditionNumStr.toInt(); i++)
            {
                triggerBusIdStr = fileLineStringList.at(6+6*i).simplified();
                deviceStateLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                deviceStateTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (deviceStateLogicalRelationshipStr == ".")
                    deviceStateLogicalRelationshipStr = "5";            //等于 ==

                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(deviceStateIdStr, triggerBusIdStr,
                                                                                            deviceStateLogicalRelationshipStr.toInt(), deviceStateTriggerValueStr.toDouble()));
            }
            deviceStateInfoList.append(DeviceStateInfoClass(deviceStateIdStr, deviceStateBusIdStr,
                                                            colorStr.toInt(), imageNameStr,
                                                            deviceStateDescStr, conditionNumStr.toInt()));
        }
        else if (categoryStr == "W1")
        {
            deviceStateIdStr = fileLineStringList.at(0).simplified();
            deviceStateBusIdStr = fileLineStringList.at(6).simplified();
            conditionNumStr = fileLineStringList.at(5).simplified();
            colorStr = fileLineStringList.at(2).simplified();
            deviceStateDescStr = fileLineStringList.at(1).simplified();

            imageNameStr.clear();

            for (int i = 0; i < conditionNumStr.toInt(); i++)
            {
                triggerBusIdStr = fileLineStringList.at(6+6*i).simplified();
                deviceStateLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                deviceStateTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (deviceStateLogicalRelationshipStr == ".")
                    deviceStateLogicalRelationshipStr = "5";            //等于 ==

                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(deviceStateIdStr, triggerBusIdStr,
                                                                                            deviceStateLogicalRelationshipStr.toInt(), deviceStateTriggerValueStr.toDouble()));
            }
            deviceStateInfoList.append(DeviceStateInfoClass(deviceStateIdStr, deviceStateBusIdStr,
                                                            colorStr.toInt(), imageNameStr,
                                                            deviceStateDescStr, conditionNumStr.toInt()));
        }
        else if (categoryStr == "W2")
        {
            deviceStateIdStr = fileLineStringList.at(0).simplified();
            deviceStateBusIdStr = fileLineStringList.at(6).simplified();
            conditionNumStr = fileLineStringList.at(5).simplified();
            colorStr = fileLineStringList.at(2).simplified();
            deviceStateDescStr = fileLineStringList.at(1).simplified();

            imageNameStr.clear();

            for (int i = 0; i < conditionNumStr.toInt(); i++)
            {
                triggerBusIdStr = fileLineStringList.at(6+6*i).simplified();
                deviceStateLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                deviceStateTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (deviceStateLogicalRelationshipStr == ".")
                    deviceStateLogicalRelationshipStr = "5";            //等于 ==

                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(deviceStateIdStr, triggerBusIdStr,
                                                                                            deviceStateLogicalRelationshipStr.toInt(), deviceStateTriggerValueStr.toDouble()));
            }
            deviceStateInfoList.append(DeviceStateInfoClass(deviceStateIdStr, deviceStateBusIdStr,
                                                            colorStr.toInt(), imageNameStr,
                                                            deviceStateDescStr, conditionNumStr.toInt()));
        }
        else if (categoryStr == "LG")
        {
            deviceStateIdStr = fileLineStringList.at(0).simplified();
            deviceStateBusIdStr = fileLineStringList.at(6).simplified();
            conditionNumStr = fileLineStringList.at(5).simplified();
            deviceStateDescStr = fileLineStringList.at(1).simplified();
            colorStr = fileLineStringList.at(2).simplified();

            imageNameStr.clear();

            for (int i = 0; i < conditionNumStr.toInt(); i++)
            {
                triggerBusIdStr = fileLineStringList.at(6+6*i).simplified();
                deviceStateLogicalRelationshipStr = fileLineStringList.at(8+6*i).simplified();
                deviceStateTriggerValueStr = fileLineStringList.at(9+6*i).simplified();

                if (deviceStateLogicalRelationshipStr == ".")
                    deviceStateLogicalRelationshipStr = "5";            //等于 ==

                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(deviceStateIdStr, triggerBusIdStr,
                                                                                            deviceStateLogicalRelationshipStr.toInt(), deviceStateTriggerValueStr.toDouble()));
            }
            deviceStateInfoList.append(DeviceStateInfoClass(deviceStateIdStr, deviceStateBusIdStr,
                                                            colorStr.toInt(), imageNameStr,
                                                            deviceStateDescStr, conditionNumStr.toInt()));
        }
    }

    logicTxtFile->close();
    checkTxtFile->close();

    if(!DatabasesOperate::initDatabasesDelayInfo(delayInfoList) || !DatabasesOperate::initDatabasesDelayTriggerCondition(delayTriggerConditionInfoList) ||
            !DatabasesOperate::initDatabasesSelfHoldInfo(selfHoldInfoList) || !DatabasesOperate::initDatabasesSelfHoldTriggerCondition(selfHoldTriggerConditionInfoList) ||
            !DatabasesOperate::initDatabasesDeviceStateInfo(deviceStateInfoList) || !DatabasesOperate::initDatabasesDeviceStateTriggerCondition(deviceStateTriggerConditionInfoList) ||
            !DatabasesOperate::initDatabasesFailureInfo(failureInfoList) || !DatabasesOperate::initDatabasesFailureTriggerCondition(failureTriggerConditionInfoList) )
    {
        LogicFileInfo->setInitDataBaseOk(false);
    }

    if(!DatabasesOperate::initDatabasesFailureProcess(failureProcessInfoList))
    {
        checkFileInfo->setInitDataBaseOk(false);
    }

    fileInfoList.append(*LogicFileInfo);
    fileInfoList.append(*checkFileInfo);
    return fileInfoList;
}

FileInfoClassList FilesOperate::initOtherTxtData()
{
    //BaseInfoClass
    QString busIdStr;
    QString categoryIdStr;
    QString interfacesStr;
    QString indicatorLightStr;
    QString meaningStr;
    QString pageNumStr;
    QString xCoordStr;
    QString yCoordStr;
    QString dataDisplayMethodStr;

    //BusInfoClass
    QString byteAddrHStr;
    QString byteAddrLStr;
    QString bitAddrStr;
    QString formulaTemplateIdStr;
    QString formulaParameterStr;
    QString addrTypeStr;

    baseInfoList.clear();
    busInfoList.clear();

    QString fileLine;
    QStringList fileLineStringList;
    int rowNumber = 0;

    FileInfoClass *DcuFileInfo = new FileInfoClass();
    DcuFileInfo->setFileFormatOk(true);
    DcuFileInfo->setInitDataBaseOk(true);
    DcuFileInfo->seterrorFileName("dcuinfor.txt");

    FileInfoClass *hostFileInfo = new FileInfoClass;
    hostFileInfo->setFileFormatOk(true);
    hostFileInfo->setInitDataBaseOk(true);
    hostFileInfo->seterrorFileName("hostinfor.txt");

    FileInfoClass *k1inforFileInfo = new FileInfoClass;
    k1inforFileInfo->setFileFormatOk(true);
    k1inforFileInfo->setInitDataBaseOk(true);
    k1inforFileInfo->seterrorFileName("k1infor.txt");

    FileInfoClass *k2inforFileInfo = new FileInfoClass;
    k2inforFileInfo->setFileFormatOk(true);
    k2inforFileInfo->setInitDataBaseOk(true);
    k2inforFileInfo->seterrorFileName("k2infor.txt");

    FileInfoClass *lcuinforFileInfo = new FileInfoClass;
    lcuinforFileInfo->setFileFormatOk(true);
    lcuinforFileInfo->setInitDataBaseOk(true);
    lcuinforFileInfo->seterrorFileName("lcuinfor.txt");

    FileInfoClass *lginforFileInfo = new FileInfoClass;
    lginforFileInfo->setFileFormatOk(true);
    lginforFileInfo->setInitDataBaseOk(true);
    lginforFileInfo->seterrorFileName("lginfor.txt");

    FileInfoClassList fileInfoList;
    fileInfoList.clear();

    //dcuinfor.txt 文档的解析

    if (!dcuinforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open dcuinforTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open dcuinforTxtFile!");
#endif
        return fileInfoList;
    }

    QTextStream dcuinforStream(dcuinforTxtFile);
    dcuinforStream.setCodec(code);       //对输出流的设置

    QString coordStr;

    while (!dcuinforStream.atEnd())
    {
        rowNumber++;
        fileLine = dcuinforStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != DCUINFOR_TXT_COLUMN)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("dcuinfor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("dcuinfor.txt 文件错误!"));
#endif
            DcuFileInfo->setFileFormatOk(false);
            DcuFileInfo->seterrorRowNum(rowNumber);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        categoryIdStr = fileLineStringList.at(1).simplified();
        coordStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        formulaTemplateIdStr = fileLineStringList.at(7).simplified();
        formulaParameterStr = fileLineStringList.at(6).simplified();
        addrTypeStr = fileLineStringList.at(9).simplified();

        interfacesStr.clear();
        indicatorLightStr.clear();

        switch (addrTypeStr.toInt())
        {
        case ONLY_BYTE_ADDR_L:
        case ONLY_BYTE_ADDR_L_FOUR_BIT_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_L_ADD_BIT_ADDR:
            byteAddrLStr = fileLineStringList.at(4).left(3).simplified();
            byteAddrHStr.clear();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        case BYTE_ADDR_H_ADD_BYTE_ADDR_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr = QString::number(byteAddrLStr.toInt()+1);
            bitAddrStr.clear();
            break;
        case ONLY_BYTE_ADDR_H:
            byteAddrHStr=fileLineStringList.at(4).simplified();
            byteAddrLStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_H_ADD_BIT_ADDR:
            byteAddrLStr.clear();
            byteAddrHStr=fileLineStringList.at(4).left(3).simplified();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        default:
            break;
        }

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            if (categoryIdStr == "W1")
            {
                categoryIdStr = QString::number(DCU);
                pageNumStr = QString::number((coordStr.toInt()-1) / DCU_ROW_NUM + 1);
                xCoordStr = QString::number(DCU1_X_COORD);
                yCoordStr = QString::number((coordStr.toInt() % DCU_ROW_NUM == 0) ? DCU_ROW_NUM-1 : coordStr.toInt() % DCU_ROW_NUM - 1);
            }
            else if (categoryIdStr == "W2")
            {
                categoryIdStr = QString::number(DCU2);
                pageNumStr = QString::number((coordStr.toInt()-1) / DCU_ROW_NUM + 1);
                xCoordStr = QString::number(DCU2_X_COORD);
                yCoordStr = QString::number((coordStr.toInt() % DCU_ROW_NUM == 0) ? DCU_ROW_NUM-1 : coordStr.toInt() % DCU_ROW_NUM - 1);
            }

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
        busInfoList.append(BusInfoClass(busIdStr, byteAddrHStr.toInt(), byteAddrLStr.toInt(),
                                        bitAddrStr.toInt(), formulaTemplateIdStr.toInt(),
                                        formulaParameterStr, addrTypeStr.toInt()));
    }
    dcuinforTxtFile->close();

    //hostinfor.txt文档解析

    rowNumber = 0;

    if (!hostinforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open hostinforTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open hostinforTxtFile!");
#endif
        return fileInfoList;
    }

    QTextStream hostinforStream(hostinforTxtFile);
    hostinforStream.setCodec(code);

    while (!hostinforStream.atEnd())
    {
        rowNumber++;
        fileLine = hostinforStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != HOSTINFOR_TXT_COLUMN)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("hostinfor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("hostinfor.txt 文件错误!"));
#endif
            hostFileInfo->seterrorRowNum(rowNumber);
            hostFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        categoryIdStr = fileLineStringList.at(1).simplified();
        coordStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        formulaTemplateIdStr = fileLineStringList.at(7).simplified();
        formulaParameterStr = fileLineStringList.at(6).simplified();
        addrTypeStr = fileLineStringList.at(9).simplified();

        interfacesStr.clear();
        indicatorLightStr.clear();

        switch (addrTypeStr.toInt())
        {
        case ONLY_BYTE_ADDR_L:
        case ONLY_BYTE_ADDR_L_FOUR_BIT_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_L_ADD_BIT_ADDR:
            byteAddrLStr = fileLineStringList.at(4).left(3).simplified();
            byteAddrHStr.clear();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        case BYTE_ADDR_H_ADD_BYTE_ADDR_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr = QString::number(byteAddrLStr.toInt()+1);
            bitAddrStr.clear();
            break;
        case ONLY_BYTE_ADDR_H:
            byteAddrHStr=fileLineStringList.at(4).simplified();
            byteAddrLStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_H_ADD_BIT_ADDR:
            byteAddrLStr.clear();
            byteAddrHStr=fileLineStringList.at(4).left(3).simplified();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        default:
            break;
        }

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(GCU);
            pageNumStr = QString::number((coordStr.toInt()-1) / HOST_PAGE_NUM + 1);
            xCoordStr = QString::number(((coordStr.toInt()-1) % HOST_PAGE_NUM / HOST_ROW_NUM) == 0 ? HOST_X1_COORD : HOST_X2_COORD);
            yCoordStr = QString::number((coordStr.toInt() % HOST_PAGE_NUM % HOST_ROW_NUM) == 0 ? HOST_ROW_NUM-1 : coordStr.toInt() % HOST_ROW_NUM - 1);

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
        busInfoList.append(BusInfoClass(busIdStr, byteAddrHStr.toInt(), byteAddrLStr.toInt(),
                                        bitAddrStr.toInt(), formulaTemplateIdStr.toInt(),
                                        formulaParameterStr, addrTypeStr.toInt()));
    }
    hostinforTxtFile->close();

    //K1.txt文档解析

    rowNumber = 0;

    if (!k1inforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open k1TxtFile!";
#endif
#if LOG
        LogFile::debug("can't open k1TxtFile!");
#endif
        return fileInfoList;
    }

    QTextStream k1inforStream(k1inforTxtFile);
    k1inforStream.setCodec(code);

    while (!k1inforStream.atEnd())
    {
        rowNumber++;
        fileLine = k1inforStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != K1INFOR_TXT_COLUMN)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("k1infor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("k1infor.txt 文件错误!"));
#endif
            k1inforFileInfo->seterrorRowNum(rowNumber);
            k1inforFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        categoryIdStr = fileLineStringList.at(1).simplified();
        coordStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        interfacesStr.clear();
        indicatorLightStr.clear();

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(PRIMARY_INTERFACE);
            pageNumStr.clear();
            xCoordStr = QString::number((coordStr.toInt()-1) % K1_COLUMN_NUM);
            yCoordStr = QString::number((coordStr.toInt()-1) / K1_COLUMN_NUM);

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
    }
    k1inforTxtFile->close();

    //K2.txt文档解析

    QString machineType;
    QString displayDetails;
    rowNumber = 0;

    if (!k2inforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open k2TxtFile!";
#endif
#if LOG
        LogFile::debug("can't open k2TxtFile!");
#endif

        return fileInfoList;
    }

    QTextStream k2inforStream(k2inforTxtFile);
    k2inforStream.setCodec(code);

    while (!k2inforStream.atEnd())
    {
        rowNumber++;
        fileLine = k2inforStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != K2INFOR_TXT_COLUMN)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("k2infor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug( QString::fromUtf8("k2infor.txt 文件错误!"));
#endif
            k2inforFileInfo->seterrorRowNum(rowNumber);
            k2inforFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        machineType = fileLineStringList.at(1).simplified();
        displayDetails = fileLineStringList.at(4).simplified();
        coordStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        interfacesStr.clear();
        indicatorLightStr.clear();

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(SECONDARY_INTERFACE);
            pageNumStr.clear();
            xCoordStr = QString::number((coordStr.toInt()-1) % K2_COLUMN_NUM);
            yCoordStr = QString::number((coordStr.toInt()-1) / K2_COLUMN_NUM);

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();

            meaningStr.append("|");
            meaningStr.append(machineType);
            meaningStr.append("|");
            meaningStr.append(displayDetails);
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
    }
    k2inforTxtFile->close();

    //lcuinfor.txt文档解析（线号界面）

    rowNumber = 0;

    if (!lcuinforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open lcuinforTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open lcuinforTxtFile!");
#endif
        return fileInfoList;
    }

    QTextStream lcuinforBusIdStream(lcuinforTxtFile);
    lcuinforBusIdStream.setCodec(code);

    while (!lcuinforBusIdStream.atEnd())
    {
        rowNumber++;
        fileLine = lcuinforBusIdStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != LCUINFOR_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("lcuinfor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("lcuinfor.txt 文件错误!"));
#endif
            lcuinforFileInfo->seterrorRowNum(rowNumber);
            lcuinforFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        interfacesStr = fileLineStringList.at(1).simplified();
        indicatorLightStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();
        meaningStr.clear();

        formulaTemplateIdStr = fileLineStringList.at(7).simplified();
        formulaParameterStr = fileLineStringList.at(6).simplified();
        addrTypeStr = fileLineStringList.at(9).simplified();

        switch (addrTypeStr.toInt())
        {
        case ONLY_BYTE_ADDR_L:
        case ONLY_BYTE_ADDR_L_FOUR_BIT_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_L_ADD_BIT_ADDR:
            byteAddrLStr = fileLineStringList.at(4).left(3).simplified();
            byteAddrHStr.clear();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        case BYTE_ADDR_H_ADD_BYTE_ADDR_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr = QString::number(byteAddrLStr.toInt()+1);
            bitAddrStr.clear();
            break;
        case ONLY_BYTE_ADDR_H:
            byteAddrHStr=fileLineStringList.at(4).simplified();
            byteAddrLStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_H_ADD_BIT_ADDR:
            byteAddrLStr.clear();
            byteAddrHStr=fileLineStringList.at(4).left(3).simplified();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        default:
            break;
        }

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(BUS_ID_INTERFACE);
            pageNumStr = QString::number((rowNumber-1) / BUS_ID_PAGE_NUM + 1);
            xCoordStr = QString::number((rowNumber-1) % BUS_ID_PAGE_NUM % BUS_ID_COLUMN_NUM);
            yCoordStr = QString::number((rowNumber-1) % BUS_ID_PAGE_NUM / BUS_ID_COLUMN_NUM);

            if (busIdStr.left(2) == "BY")
                meaningStr = "---";
            else
                meaningStr = busIdStr;

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
        busInfoList.append(BusInfoClass(busIdStr, byteAddrHStr.toInt(), byteAddrLStr.toInt(),
                                        bitAddrStr.toInt(), formulaTemplateIdStr.toInt(),
                                        formulaParameterStr, addrTypeStr.toInt()));
    }
    lcuinforTxtFile->close();

    //lcu.txt文档解析（接口灯孔）

    rowNumber = 0;

    if (!lcuinforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open lcuinforTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open lcuinforTxtFile!");
#endif
        return fileInfoList;
    }

    QTextStream lcuinforInterfaceStream(lcuinforTxtFile);
    lcuinforInterfaceStream.setCodec(code);

    while (!lcuinforInterfaceStream.atEnd())
    {
        rowNumber++;
        fileLine = lcuinforInterfaceStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != LCUINFOR_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("lcuinfor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("lcuinfor.txt 文件错误!"));
#endif
            lcuinforFileInfo->seterrorRowNum(rowNumber);
            lcuinforFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        interfacesStr = fileLineStringList.at(1).simplified();
        indicatorLightStr = fileLineStringList.at(2).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(INTERFACE_LAMP_HOLE);
            pageNumStr = QString::number((rowNumber-1) / INTERFACE_PAGE_NUM + 1);
            xCoordStr = QString::number(((rowNumber-1) % INTERFACE_PAGE_NUM / INTERFACE_ROW_NUM) == 0 ? INTERFACE_1_X_COORD : INTERFACE_2_X_COORD);
            yCoordStr = QString::number((rowNumber-1) % INTERFACE_PAGE_NUM % INTERFACE_ROW_NUM);

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
    }
    lcuinforTxtFile->close();

    //lg.txt文档解析

    rowNumber = 0;

    if (!lginforTxtFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
#if DEBUG
        qDebug() << "can't open lginforTxtFile!";
#endif
#if LOG
        LogFile::debug("can't open lginforTxtFile!");
#endif

        return fileInfoList;
    }

    QTextStream lginforStream(lginforTxtFile);
    lginforStream.setCodec(code);

    while (!lginforStream.atEnd())
    {
        rowNumber++;
        fileLine = lginforStream.readLine();
        fileLineStringList = fileLine.split(' ', QString::SkipEmptyParts);

        if (fileLineStringList.size() != LGINFOR_TXT_COLUMN_NUM)
        {
#if DEBUG
            qDebug() << QString::fromUtf8("lginfor.txt 文件错误!");
#endif
#if LOG
            LogFile::debug(QString::fromUtf8("lginfor.txt 文件错误!"));
#endif

            lginforFileInfo->seterrorRowNum(rowNumber);
            lginforFileInfo->setFileFormatOk(false);
        }

        busIdStr = fileLineStringList.at(0).simplified();
        meaningStr = fileLineStringList.at(3).simplified();
        coordStr = fileLineStringList.at(2).simplified();
        dataDisplayMethodStr = fileLineStringList.at(8).simplified();

        formulaTemplateIdStr = fileLineStringList.at(7).simplified();
        formulaParameterStr = fileLineStringList.at(6).simplified();
        addrTypeStr = fileLineStringList.at(9).simplified();

        interfacesStr.clear();
        indicatorLightStr.clear();

        switch (addrTypeStr.toInt())
        {
        case ONLY_BYTE_ADDR_L:
        case ONLY_BYTE_ADDR_L_FOUR_BIT_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_L_ADD_BIT_ADDR:
            byteAddrLStr = fileLineStringList.at(4).left(3).simplified();
            byteAddrHStr.clear();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        case BYTE_ADDR_H_ADD_BYTE_ADDR_L:
            byteAddrLStr = fileLineStringList.at(4).simplified();
            byteAddrHStr = QString::number(byteAddrLStr.toInt()+1);
            bitAddrStr.clear();
            break;
        case ONLY_BYTE_ADDR_H:
            byteAddrHStr=fileLineStringList.at(4).simplified();
            byteAddrLStr.clear();
            bitAddrStr.clear();
            break;
        case BYTE_ADDR_H_ADD_BIT_ADDR:
            byteAddrLStr.clear();
            byteAddrHStr=fileLineStringList.at(4).left(3).simplified();
            bitAddrStr = fileLineStringList.at(4).right(1).simplified();
            break;
        default:
            break;
        }

        if (dataDisplayMethodStr == "0")
        {
            continue;
        }
        else
        {
            categoryIdStr = QString::number(LG);
            pageNumStr = QString::number((coordStr.toInt()-1) / LG_PAGE_NUM + 1);
            xCoordStr = QString::number(LG_X_COORD);
            yCoordStr = QString::number((coordStr.toInt() % LG_PAGE_NUM % LG_ROW_NUM) == 0 ? LG_ROW_NUM-1 : coordStr.toInt() % LG_ROW_NUM - 1);

            if (formulaTemplateIdStr == ".")
                formulaTemplateIdStr.clear();
            if (formulaParameterStr == ".")
                formulaParameterStr.clear();
        }

        baseInfoList.append(BaseInfoClass(busIdStr, categoryIdStr.toInt(), interfacesStr,
                                          indicatorLightStr, meaningStr, pageNumStr.toInt(),
                                          xCoordStr.toInt(), yCoordStr.toInt(),
                                          dataDisplayMethodStr.toInt()));
        busInfoList.append(BusInfoClass(busIdStr, byteAddrHStr.toInt(), byteAddrLStr.toInt(),
                                        bitAddrStr.toInt(), formulaTemplateIdStr.toInt(),
                                        formulaParameterStr, addrTypeStr.toInt()));
    }
    lginforTxtFile->close();

    if(!DatabasesOperate::initDatabasesBaseInfo(baseInfoList))
    {
        lcuinforFileInfo->setInitDataBaseOk(false);
        DcuFileInfo->setInitDataBaseOk(false);
        hostFileInfo->setInitDataBaseOk(false);
        k1inforFileInfo->setInitDataBaseOk(false);
        k2inforFileInfo->setInitDataBaseOk(false);
        lginforFileInfo->setInitDataBaseOk(false);
    }
    if(!DatabasesOperate::initDatabasesBusInfo(busInfoList))
    {
        lginforFileInfo->setInitDataBaseOk(false);
        DcuFileInfo->setInitDataBaseOk(false);
        hostFileInfo->setInitDataBaseOk(false);
    }

    fileInfoList.append(*DcuFileInfo);
    fileInfoList.append(*hostFileInfo);
    fileInfoList.append(*k1inforFileInfo);
    fileInfoList.append(*k2inforFileInfo);
    fileInfoList.append(*lcuinforFileInfo);
    fileInfoList.append(*lginforFileInfo);

    return fileInfoList;
}
