#include "databasesoperate.h"
#include "enumtypeheader.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlError>
#include "logfile.h"
#include "QCoreApplication"

QSqlDatabase DatabasesOperate::db = QSqlDatabase();

bool DatabasesOperate::connectDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../FailureCheck/Databases/FailureCheck.db");

    if (db.open())
    {
#if DEBUG
        qDebug() << QString::fromUtf8("数据库连接成功!");
#endif
#if LOG
        LogFile::debug(QString::fromUtf8("数据库连接成功!"));
#endif
        //建表
        if(createDataBaseAllTables())
        {
#if DEBUG
            qDebug() << "createDataBaseAllTables success!";
#endif
#if LOG
            LogFile::debug("createDataBaseAllTables success!");
#endif
        }else
        {
#if DEBUG
            qDebug() << "createDataBaseAllTables error!";
#endif
#if LOG
            LogFile::debug("createDataBaseAllTables error!");
#endif
        }
        return true;
    }else
    {
#if DEBUG
        qDebug() << QString::fromUtf8("数据库连接失败!");
#endif
#if LOG
        LogFile::debug(QString::fromUtf8("数据库连接失败!"));
#endif
        return false;
    }
}

bool DatabasesOperate::clearDataBaseAllTables()
{
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    { //先判断该数据库是否支持事务操作
        if(QSqlDatabase::database().transaction()) //启动事务操作
        {
            //下面执行各种数据库操作
            QSqlQuery query;
            query.exec("DELETE FROM `bus_info`;");
            query.exec("DELETE FROM `failure_info`;");
            query.exec("DELETE FROM `failure_trigger_condition`;");
            query.exec("DELETE FROM `failure_process`;");
            query.exec("DELETE FROM `base_info`;");
            query.exec("DELETE FROM `schematic`;");
            query.exec("DELETE FROM `delay_info`;");
            query.exec("DELETE FROM `delay_trigger_condition`;");
            query.exec("DELETE FROM `ladder_diagram`;");
            query.exec("DELETE FROM `self_hold_info`;");
            query.exec("DELETE FROM `self_hold_trigger_condition`;");
            query.exec("DELETE FROM `device_state_info`;");
            query.exec("DELETE FROM `device_state_trigger_condition`;");
            //query.exec("DELETE FROM `failure_history_record`;");
            query.exec("DELETE FROM `verhicle_info`;");

            if(!QSqlDatabase::database().commit())
            {
#if DEBUG
                qDebug() <<"clearDataBaseAllTables error:"<< QSqlDatabase::database().lastError(); //提交
#endif
#if LOG
                LogFile::debug("clearDataBaseAllTables commit error");
#endif
                if(!QSqlDatabase::database().rollback())
                {
#if DEBUG
                    qDebug() <<"clearDataBaseAllTables error:" <<QSqlDatabase::database().lastError(); //回滚
#endif
#if LOG
                    LogFile::debug("clearDataBaseAllTables callback error");
#endif
                }
            }else
            {
                return true;
            }
        }
    }else
    {
        return false;
    }
}

bool DatabasesOperate::createDataBaseAllTables()
{
    if(QSqlDatabase::database().driver()->hasFeature(QSqlDriver::Transactions))
    { //先判断该数据库是否支持事务操作
        if(QSqlDatabase::database().transaction()) //启动事务操作
        {
            //下面执行各种数据库操作
            QSqlQuery query;
            query.exec("\
                       CREATE TABLE IF NOT EXISTS `bus_info` (\
                           `bus_id` varchar(10) NOT NULL,\
                           `byte_addr_h` varchar(5) DEFAULT NULL,\
                           `byte_addr_l` varchar(5) NOT NULL,\
                           `bit_addr` varchar(5) DEFAULT NULL,\
                           `formula_template_id` tinyint(1) DEFAULT NULL,\
                           `formula_parameter` varchar(50) DEFAULT NULL,\
                           `addr_type` tinyint(1) NOT NULL,\
                           PRIMARY KEY (`bus_id`));\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `failure_info` (\
                                   `failure_id` varchar(5) NOT NULL,\
                                   `failure_name` varchar(48) NOT NULL,\
                                   `failure_trigger_condition_num` varchar(5) NOT NULL,\
                                   `failure_process_method_condition_num` varchar(5) NOT NULL,\
                                   `failure_lv` varchar(2) NOT NULL,\
                                   `output_bus_id` varchar(10) DEFAULT NULL,\
                                   PRIMARY KEY (`failure_id`)\
                                   );");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `failure_trigger_condition` (\
                                   `failure_id` varchar(5) NOT NULL,\
                                   `bus_id` varchar(10) NOT NULL,\
                                   `logical_relationship` varchar(2) NOT NULL DEFAULT ('5'),\
                                   `trigger_value` varchar(10) NOT NULL,\
                                   PRIMARY KEY (`failure_id`, `bus_id`),\
                                   FOREIGN KEY (`failure_id`) REFERENCES `failure_info` (`failure_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `failure_process` (\
                                   `failure_id` varchar(5) NOT NULL,\
                                   `process_point` varchar(10) NOT NULL,\
                                   `process_method` varchar(58) NOT NULL,\
                                   `image_name` varchar(10) NOT NULL,\
                                   PRIMARY KEY (`failure_id`, `process_point`, `process_method`),\
                                   FOREIGN KEY (`failure_id`) REFERENCES `failure_info` (`failure_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `base_info` (\
                                   `bus_id` varchar(10) NOT NULL,\
                                   `category_id` tinyint(1) NOT NULL,\
                                   `interfaces` varchar(8),\
                                   `indicator_light` varchar(7),\
                                   `meaning` varchar(20) NOT NULL,\
                                   `page_num` varchar(2) NOT NULL DEFAULT ('0'),\
                                   `x_coord` varchar(3),\
                                   `y_coord` varchar(3),\
                                   `data_display_method` tinyint(1) NOT NULL,\
                                   PRIMARY KEY (`bus_id`, `category_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `schematic` (\
                                   `schematic_catalog` varchar(50) NOT NULL,\
                                   `schematic_name` varchar(20) NOT NULL,\
                                   PRIMARY KEY (`schematic_catalog`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `delay_info` (\
                                   `delay_id` varchar(10) NOT NULL,\
                                   `logical_relationship` varchar(2) NOT NULL DEFAULT ('5'),\
                                   `delay_time` varchar(10) NOT NULL,\
                                   `delay_value` varchar(10) NOT NULL DEFAULT ('1'),\
                                   `delay_condition_num` varchar(2) NOT NULL,\
                                   PRIMARY KEY (`delay_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `delay_trigger_condition` (\
                                   `delay_id` varchar(10) NOT NULL,\
                                   `bus_id` varchar(10) NOT NULL,\
                                   `logical_relationship` varchar(2) NOT NULL DEFAULT ('5'),\
                                   `trigger_value` varchar(10) NOT NULL,\
                                   PRIMARY KEY (`delay_id`, `bus_id`),\
                                   FOREIGN KEY (`delay_id`) REFERENCES `delay_info` (`delay_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `ladder_diagram` (\
                                   `page_num` varchar(2) NOT NULL,\
                                   `x_coord` varchar(3) NOT NULL,\
                                   `y_coord` varchar(3) NOT NULL,\
                                   `bus_id` varchar(10) DEFAULT NULL,\
                                   `part_type` varchar(2) NOT NULL,\
                                   `meaning` varchar(11) DEFAULT NULL,\
                                   PRIMARY KEY (`page_num`, `x_coord`, `y_coord`, `part_type`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `self_hold_info` (\
                                   `self_flag_id` varchar(10) UNIQUE NOT NULL,\
                                   `self_hold_id` varchar(10) NOT NULL,\
                                   `self_hold_value` varchar(10) NOT NULL,\
                                   `self_hold_condition_num` varchar(2) NOT NULL,\
                                   PRIMARY KEY (`self_flag_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `self_hold_trigger_condition` (\
                                   `self_flag_id` varchar(10) NOT NULL,\
                                   `bus_id` varchar(10) NOT NULL,\
                                   `logical_relationship` varchar(2) NOT NULL DEFAULT ('5'),\
                                   `trigger_value` varchar(10) NOT NULL,\
                                   FOREIGN KEY (`self_flag_id`) REFERENCES `self_hold_info` (`self_flag_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `device_state_info` (\
                                   `device_state_id` varchar(32) UNIQUE NOT NULL,\
                                   `bus_id` varchar(10),\
                                   `color` tinyint(1) NOT NULL DEFAULT('0'),\
                                   `image_name` varchar(10) DEFAULT NULL,\
                                   `device_state_desc` varchar(50) DEFAULT NULL,\
                                   `condition_num` varchar(2) NOT NULL,\
                                   PRIMARY KEY (`device_state_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `device_state_trigger_condition` (\
                                   `device_state_id` varchar(32) NOT NULL,\
                                   `bus_id` varchar(10),\
                                   `logical_relationship` varchar(2) NOT NULL DEFAULT ('5'),\
                                   `trigger_value` varchar(10) NOT NULL,\
                                   FOREIGN KEY (`device_state_id`) REFERENCES `device_state_info` (`device_state_id`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `failure_history_record` (\
                                   `failure_id` varchar(5) NOT NULL,\
                                   `failure_lv` varchar(2) NOT NULL,\
                                   `start_time` varchar(20) NOT NULL,\
                                   `end_time` varchar(20),\
                                   `failure_name` varchar(48) NOT NULL,\
                                   PRIMARY KEY (`failure_id`, `start_time`)\
                                   );\
                    ");
                    query.exec("\
                               CREATE TABLE IF NOT EXISTS `verhicle_info` (\
                                   `key` varchar(20) NOT NULL,\
                                   `value` varchar(20) NOT NULL,\
                                   PRIMARY KEY (`key`)\
                                   );\
                    ");

                    if(!QSqlDatabase::database().commit())
            {
#if DEBUG
                qDebug() <<"createDataBaseAllTables error:"<< QSqlDatabase::database().lastError(); //提交
#endif
#if LOG
                LogFile::debug("createDataBaseAllTables error:");
#endif
                if(!QSqlDatabase::database().rollback())
                {
#if DEBUG
                    qDebug() <<"createDataBaseAllTables commit error:" <<QSqlDatabase::database().lastError(); //回滚
#endif
#if LOG
                    LogFile::debug("createDataBaseAllTables callback error:");
#endif
                }
            }else
            {
                return true;
            }
        }else
        {
            return false;
        }
    }else
    {
        return false;
    }
}

bool DatabasesOperate::initDatabasesLadderDiagram(LadderDiagramInfoList &ladderDiagramClassList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from ladder_diagram";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from ladder_diagram error";
#endif
#if LOG
        LogFile::debug("delete from ladder_diagram error");
#endif
        return false;
    }
    //#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 ladder_diagram ... ... ");
    //#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 ladder_diagram ... ... "));
#endif
    for (int i = 0; i < ladderDiagramClassList.size(); i++)
    {
        LadderDiagramClass ladderDiagramClass = ladderDiagramClassList.at(i);
        sqlString = QString("insert into ladder_diagram values('%1', '%2', '%3', '%4', '%5', '%6')")
                .arg(ladderDiagramClass.getPageNumStr(),
                     ladderDiagramClass.getXCoordStr(),
                     ladderDiagramClass.getYCoordStr(),
                     ladderDiagramClass.getBusId(),
                     ladderDiagramClass.getPartTypeStr(),
                     ladderDiagramClass.getMeaning());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesLadderDiagram error";
#endif
#if LOG
            LogFile::debug("initDatabasesLadderDiagram error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 ladder_diagram !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 ladder_diagram !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesSchematic(SchematicInfoList &schematicInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from schematic";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from schematic error";
#endif
#if LOG
        LogFile::debug("delete from schematic error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 schematic ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 schematic ... ... "));
#endif
    for (int i = 0; i < schematicInfoList.size(); i++)
    {
        SchematicClass schematicClass = schematicInfoList.at(i);
        sqlString = QString("insert into schematic values('%1', '%2')")
                .arg(schematicClass.getSchematicCatalog(),
                     schematicClass.getSchematicName());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesSchematic error";
#endif
#if LOG
            LogFile::debug("initDatabasesSchematic error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 schematic !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 schematic !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesDelayInfo(DelayInfoList &delayInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from delay_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from delay_info error";
#endif
#if LOG
        LogFile::debug("delete from delay_info error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 delay_info ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 delay_info ... ... "));
#endif
    for (int i = 0; i < delayInfoList.size(); i++)
    {
        DelayInfoClass delayInfoClass = delayInfoList.at(i);
        sqlString = QString("insert into delay_info values('%1', '%2', '%3', '%4', '%5')")
                .arg(delayInfoClass.getDelayId(),
                     QString::number(delayInfoClass.getLogicalRelationship()),
                     QString::number(delayInfoClass.getDelaySeconds()),
                     QString::number(delayInfoClass.getDelayValue()),
                     QString::number(delayInfoClass.getDelayConditionNum()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesDelayInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesDelayInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 delay_info !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 delay_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesDelayTriggerCondition(DelayTriggerConditionInfoList &delayTriggerConditionInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from delay_trigger_condition";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from delay_trigger_condition error";
#endif
#if LOG
        LogFile::debug("delete from delay_trigger_condition error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 delay_trigger_condition ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 delay_trigger_condition ... ... "));
#endif
    for (int i = 0; i < delayTriggerConditionInfoList.size(); i++)
    {
        DelayTriggerConditionClass delayTriggerConditionClass = delayTriggerConditionInfoList.at(i);
        sqlString = QString("insert into delay_trigger_condition values('%1', '%2', '%3', '%4')")
                .arg(delayTriggerConditionClass.getDelayId(),
                     delayTriggerConditionClass.getBusId(),
                     delayTriggerConditionClass.getLogicalRelationship(),
                     delayTriggerConditionClass.getTriggerValue());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesDelayTriggerCondition error";
#endif
#if LOG
            LogFile::debug("initDatabasesDelayTriggerCondition error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 delay_trigger_condition !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 delay_trigger_condition !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesFailureTriggerCondition(FailureTriggerConditionInfoList &failureTriggerConditionInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from failure_trigger_condition";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from failure_trigger_condition error";
#endif
#if LOG
        LogFile::debug("delete from failure_trigger_condition error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 failure_trigger_condition ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 failure_trigger_condition ... ... "));
#endif
    for (int i = 0; i < failureTriggerConditionInfoList.size(); i++)
    {
        FailureTriggerConditionClass failureTriggerConditionClass = failureTriggerConditionInfoList.at(i);
        sqlString = QString("insert into failure_trigger_condition values('%1', '%2', '%3', '%4')")
                .arg(failureTriggerConditionClass.getFailureId(),
                     failureTriggerConditionClass.getBusId(),
                     failureTriggerConditionClass.getLogicalRelationship(),
                     failureTriggerConditionClass.getTriggerValue());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesFailureTriggerCondition error";
#endif
#if LOG
            LogFile::debug("initDatabasesFailureTriggerCondition error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 failure_trigger_condition !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 failure_trigger_condition !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesFailureProcess(FailureProcessInfoList &failureProcessInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from failure_process";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from failure_process error";
#endif
#if LOG
        LogFile::debug("delete from failure_process error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 failure_process ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 failure_process ... ... "));
#endif
    for (int i = 0; i < failureProcessInfoList.size(); i++)
    {
        FailureProcessClass failureProcessClass = failureProcessInfoList.at(i);
        sqlString = QString("insert into failure_process values('%1', '%2', '%3', '%4')")
                .arg(failureProcessClass.getFailureId(),
                     failureProcessClass.getProcessPoint(),
                     failureProcessClass.getProcessMethod(),
                     failureProcessClass.getImageName());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesFailureProcess error";
#endif
#if LOG
            LogFile::debug("initDatabasesFailureProcess error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 failure_process !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 failure_process !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesFailureInfo(FailureInfoList &failureInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from failure_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from failure_info error";
#endif
#if LOG
        LogFile::debug("delete from failure_info error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 failure_info ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 failure_info ... ... "));
#endif
    for (int i = 0; i < failureInfoList.size(); i++)
    {
        FailureInfoClass failureInfoClass = failureInfoList.at(i);
        sqlString = QString("insert into failure_info values('%1', '%2', '%3', '%4', '%5', '%6')")
                .arg(failureInfoClass.getFailureId(),
                     failureInfoClass.getFailureName(),
                     QString::number(failureInfoClass.getFailureTriggerConditionNum()),
                     QString::number(failureInfoClass.getFailureProcessMethodConditionNum()),
                     QString::number(failureInfoClass.getFailureLv()),
                     failureInfoClass.getOutputBusId());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesFailureInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesFailureInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 failure_info !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 failure_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesBusInfo(BusInfoList &busInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from bus_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from bus_info error";
#endif
#if LOG
        LogFile::debug("delete from bus_info error");
#endif
        return false;
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 bus_info ... ... ");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 bus_info ... ... "));
#endif
    for (int i = 0; i < busInfoList.size(); i++)
    {
        BusInfoClass busInfoClass = busInfoList.at(i);
        sqlString = QString("insert into bus_info values('%1', '%2', '%3', '%4', '%5', '%6', '%7')")
                .arg(busInfoClass.getBusId(),
                     QString::number(busInfoClass.getByteAddrH()),
                     QString::number(busInfoClass.getByteAddrL()),
                     QString::number(busInfoClass.getBitAddr()),
                     QString::number(busInfoClass.getFormulaTemplateId()),
                     busInfoClass.getFormulaParameter(),
                     QString::number(busInfoClass.getAddrType()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesBusInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesBusInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
//#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 bus_info !");
//#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 bus_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesBaseInfo(BaseInfoList &baseInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from base_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from base_info error";
#endif
#if LOG
        LogFile::debug("delete from base_info error");
#endif
        return false;
    }
#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 base_info ... ... ");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 base_info ... ... "));
#endif
    for (int i = 0; i < baseInfoList.size(); i++)
    {
        BaseInfoClass baseInfoClass = baseInfoList.at(i);
        sqlString = QString("insert into base_info values('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9')")
                .arg(baseInfoClass.getBusId(),
                     QString::number(baseInfoClass.getCategoryId()),
                     baseInfoClass.getInterfaces(),
                     baseInfoClass.getIndicatorLight(),
                     baseInfoClass.getMeaning(),
                     QString::number(baseInfoClass.getPageNum()),
                     QString::number(baseInfoClass.getXCoord()),
                     QString::number(baseInfoClass.getYCoord()),
                     QString::number(baseInfoClass.getDataDisplayMethod()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesBaseInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesBaseInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 base_info !");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 base_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesSelfHoldInfo(SelfHoldInfoList &selfHoldInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from self_hold_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from self_hold_info error";
#endif
#if LOG
        LogFile::debug("delete from self_hold_info error");
#endif
        return false;
    }
#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 self_hold_info ... ... ");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 self_hold_info ... ... "));
#endif
    for (int i = 0; i < selfHoldInfoList.size(); i++)
    {
        SelfHoldInfoClass selfHoldInfoClass = selfHoldInfoList.at(i);
        sqlString = QString("insert into self_hold_info values('%1', '%2', '%3', '%4')")
                .arg(selfHoldInfoClass.getSelfFlagId(),
                     selfHoldInfoClass.getSelfHoldId(),
                     QString::number(selfHoldInfoClass.getSelfHoldValue()),
                     QString::number(selfHoldInfoClass.getSelfHoldConditionNum()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesSelfHoldInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesSelfHoldInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 self_hold_info !");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 self_hold_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesSelfHoldTriggerCondition(SelfHoldTriggerConditionInfoList &selfHoldTriggerConditionInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from self_hold_trigger_condition";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from self_hold_trigger_condition error";
#endif
#if LOG
        LogFile::debug("delete from self_hold_trigger_condition error");
#endif
        return false;
    }
#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 self_hold_trigger_condition ... ... ");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 self_hold_trigger_condition ... ... "));
#endif
    for (int i = 0; i < selfHoldTriggerConditionInfoList.size(); i++)
    {
        SelfHoldTriggerConditionClass selfHoldTriggerConditionClass = selfHoldTriggerConditionInfoList.at(i);
        sqlString = QString("insert into self_hold_trigger_condition values('%1', '%2', '%3', '%4')")
                .arg(selfHoldTriggerConditionClass.getSelfFlagId(),
                     selfHoldTriggerConditionClass.getBusId(),
                     selfHoldTriggerConditionClass.getLogicalRelationship(),
                     selfHoldTriggerConditionClass.getTriggerValue());

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesSelfHoldTriggerCondition error";
#endif
#if LOG
            LogFile::debug("initDatabasesSelfHoldTriggerCondition error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 self_hold_trigger_condition !");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 self_hold_trigger_condition !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesDeviceStateInfo(DeviceStateInfoList &deviceStateInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from device_state_info";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from device_state_info error";
#endif
#if LOG
        LogFile::debug("delete from device_state_info error");
#endif
        return false;
    }
#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 device_state_info ... ... ");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 device_state_info ... ... "));
#endif
    for (int i = 0; i < deviceStateInfoList.size(); i++)
    {
        DeviceStateInfoClass deviceStateInfoClass = deviceStateInfoList.at(i);
        sqlString = QString("insert into device_state_info values('%1', '%2', '%3', '%4', '%5', '%6')")
                .arg(deviceStateInfoClass.getDeviceStateId(),
                     deviceStateInfoClass.getBusId(),
                     QString::number(deviceStateInfoClass.getColor()),
                     deviceStateInfoClass.getImageName(),
                     deviceStateInfoClass.getDeviceStateDesc(),
                     QString::number(deviceStateInfoClass.getConditionNum()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesDeviceStateInfo error";
#endif
#if LOG
            LogFile::debug("initDatabasesDeviceStateInfo error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 device_state_info !");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 device_state_info !"));
#endif
    return true;
}

bool DatabasesOperate::initDatabasesDeviceStateTriggerCondition(DeviceStateTriggerConditionInfoList &deviceStateTriggerConditionInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "delete from device_state_trigger_condition";
    if (!sqlQuery.exec(sqlString))
    {
#if DEBUG
        qDebug() << "delete from device_state_trigger_condition error";
#endif
#if LOG
        LogFile::debug("delete from device_state_trigger_condition error");
#endif
        return false;
    }
#if DEBUG
    qDebug() << QString::fromUtf8("正在导入数据库 device_state_trigger_condition ... ... ");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("正在导入数据库 device_state_trigger_condition ... ... "));
#endif
    for (int i = 0; i < deviceStateTriggerConditionInfoList.size(); i++)
    {
        DeviceStateTriggerConditionClass deviceStateTriggerConditionClass = deviceStateTriggerConditionInfoList.at(i);
        sqlString = QString("insert into device_state_trigger_condition values('%1', '%2', '%3', '%4')")
                .arg(deviceStateTriggerConditionClass.getDeviceStateId(),
                     deviceStateTriggerConditionClass.getBusId(),
                     QString::number(deviceStateTriggerConditionClass.getLogicalRelationship()),
                     QString::number(deviceStateTriggerConditionClass.getTriggerValue()));

        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "initDatabasesDeviceStateTriggerCondition error";
#endif
#if LOG
            LogFile::debug("initDatabasesDeviceStateTriggerCondition error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
#if DEBUG
    qDebug() << QString::fromUtf8("成功导入数据库 device_state_trigger_condition !");
#endif
#if LOG
    LogFile::debug(QString::fromUtf8("成功导入数据库 device_state_trigger_condition !"));
#endif
    return true;
}

bool DatabasesOperate::getVerhicleInfo(QMap<QString, QString> &verhicleInfoMapKeyAndValue)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    verhicleInfoMapKeyAndValue.clear();
    sqlString = "select * from verhicle_info;";

    if ( sqlQuery.exec( sqlString ))
    {
        while ( sqlQuery.next() )
        {
            verhicleInfoMapKeyAndValue[sqlQuery.value(0).toString()]=sqlQuery.value(1).toString();
            QCoreApplication::processEvents();
        }
        return true;
    }else
    {
#if DEBUG
        qDebug() << "getVerhicleInfo error";
#endif
#if LOG
        LogFile::debug("getVerhicleInfo error");
#endif
        return false;
    }
}

bool DatabasesOperate::insertVerhicleInfoToDataBase(QMap<QString, QString> &verhicleInfoMapKeyAndValue)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    QMapIterator<QString, QString> mapIterator(verhicleInfoMapKeyAndValue);
    while (mapIterator.hasNext()) {
        mapIterator.next();

        sqlString = QString("insert into verhicle_info values('%1','%2');").arg(mapIterator.key(),mapIterator.value());
        if (!sqlQuery.exec(sqlString))
        {
#if DEBUG
            qDebug() << "insertVerhicleInfoToDataBase error";
#endif
#if LOG
            LogFile::debug("insertVerhicleInfoToDataBase error");
#endif
            return false;
        }
        QCoreApplication::processEvents();
    }
    return true;
}

bool DatabasesOperate::initInterfaceBaseInfoList(BaseInfoList &interfaceBaseInfoList, int interfaceName)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    interfaceBaseInfoList.clear();
    sqlString = "select * from base_info where category_id = '%1'";

    switch (interfaceName)
    {
    case PRIMARY_INTERFACE:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(PRIMARY_INTERFACE);
        break;
    case SECONDARY_INTERFACE:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(SECONDARY_INTERFACE);
        break;
    case BUS_ID_INTERFACE:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(BUS_ID_INTERFACE);
        break;
    case INTERFACE_LAMP_HOLE:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(INTERFACE_LAMP_HOLE);
        break;
    case DCU:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(DCU);
        break;
    case DCU2:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(DCU2);
        break;
    case GCU:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(GCU);
        break;
    case LG:
        sqlString = QString("select * from base_info where category_id = '%1'").arg(LG);
        break;
    default:
        break;
    }

    if ( sqlQuery.exec( sqlString ))
    {
        while ( sqlQuery.next() )
        {
            interfaceBaseInfoList.append(BaseInfoClass(
                                             sqlQuery.value(0).toString(),
                                             sqlQuery.value(1).toInt(),
                                             sqlQuery.value(2).toString(),
                                             sqlQuery.value(3).toString(),
                                             sqlQuery.value(4).toString(),
                                             sqlQuery.value(5).toInt(),
                                             sqlQuery.value(6).toInt(),
                                             sqlQuery.value(7).toInt(),
                                             sqlQuery.value(8).toInt()));
            QCoreApplication::processEvents();
        }
        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initInterfaceBaseInfoList error";
#endif
#if LOG
        LogFile::debug("initInterfaceBaseInfoList error");
#endif
        return false;
    }
}

bool DatabasesOperate::initFailureProcessInfoList(FailureProcessInfoList &failureProcessInfoList, QString failureId)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    failureProcessInfoList.clear();
    sqlString = "select * from failure_process where failure_id = '%1'";

    if ( sqlQuery.exec( sqlString.arg(failureId) ))
    {
        while ( sqlQuery.next() )
        {
            failureProcessInfoList.append(FailureProcessClass(
                                              sqlQuery.value(0).toString(),
                                              sqlQuery.value(1).toString(),
                                              sqlQuery.value(2).toString(),
                                              sqlQuery.value(3).toString()));
            QCoreApplication::processEvents();
        }
        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initFailureProcessInfoList error";
#endif
#if LOG
        LogFile::debug("initFailureProcessInfoList error");
#endif
        return false;
    }
}

bool DatabasesOperate::getFailureInfo(FailureInfoClass &failureInfo, QString failureName)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    sqlString = "select * from failure_info where failure_name = '%1'";

    if (sqlQuery.exec(sqlString.arg(failureName)))
    {
        sqlQuery.next();

        failureInfo = FailureInfoClass(
                    sqlQuery.value(0).toString(),
                    sqlQuery.value(1).toString(),
                    sqlQuery.value(2).toInt(),
                    sqlQuery.value(3).toInt(),
                    sqlQuery.value(4).toInt(),
                    sqlQuery.value(5).toString());

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "getFailureInfo error";
#endif
#if LOG
        LogFile::debug("getFailureInfo error");
#endif
        return false;
    }
}

bool DatabasesOperate::getLadderDiagramInfo(const int &pageNum,
                                            LadderDiagramInfoList &ladderDiagramInputInfoList,
                                            LadderDiagramInfoList &ladderDiagramOutPutInfoList)
{
    QString sqlString = QString("select * from ladder_diagram where page_num = '%1'")
            .arg(QString::number(pageNum).rightJustified(2, '0'));
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            LadderDiagramClass ladderDiagramInfo(
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                        query.value(4).toString(),
                        query.value(5).toString());

            if ( ladderDiagramInfo.getPartTypeInt() == REPEATER || ladderDiagramInfo.getPartTypeInt() == OUTPUT )
            {
                ladderDiagramOutPutInfoList.append(ladderDiagramInfo);
            }
            else
            {
                ladderDiagramInputInfoList.append(ladderDiagramInfo);
            }
            QCoreApplication::processEvents();
        }
        return true;
    }
#if DEBUG
    qDebug() << "getLadderDiagramInfo error";
#endif
#if LOG
    LogFile::debug("getLadderDiagramInfo error");
#endif
    return false;
}

bool DatabasesOperate::getLadderDiagramMaxPageNum(int &maxPageNum)
{
    QString sqlString = QString("select max(page_num) from ladder_diagram");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            maxPageNum = query.value(0).toInt();
            QCoreApplication::processEvents();
        }

        return true;
    }
#if DEBUG
    qDebug() << "get LadderDiagramMaxPageNum error";
#endif
#if LOG
    LogFile::debug("get LadderDiagramMaxPageNum error");
#endif
    return false;
}
//获取梯形图所有的信息
bool DatabasesOperate::getLadderDiagramAllListInfo(LadderDiagramInfoList &allListInfo)
{
    allListInfo.clear();
    QString sqlString = QString("select * from ladder_diagram");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            LadderDiagramClass ladderDiagramInfo(
                        query.value(0).toString(),
                        query.value(1).toString(),
                        query.value(2).toString(),
                        query.value(3).toString(),
                        query.value(4).toString(),
                        query.value(5).toString()
                        );
            allListInfo.append(ladderDiagramInfo);
            QCoreApplication::processEvents();
        }

        return true;
    }
#if DEBUG
    qDebug() << " all getLadderDiagramList error";
#endif
#if LOG
    LogFile::debug(" all getLadderDiagramList error");
#endif
    return false;
}

bool DatabasesOperate::initBusInfoList(BusInfoList &busInfoList)
{
    busInfoList.clear();
    QString sqlString = QString("select * from bus_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            busInfoList.append(BusInfoClass(
                                   query.value(0).toString(),
                                   query.value(1).toInt(),
                                   query.value(2).toInt(),
                                   query.value(3).toInt(),
                                   query.value(4).toInt(),
                                   query.value(5).toString(),
                                   query.value(6).toInt()));
            QCoreApplication::processEvents();
        }

        return true;
    }
#if DEBUG
    qDebug() << "initBusInfoList error";
#endif
#if LOG
    LogFile::debug("initBusInfoList error");
#endif
    return false;
}

bool DatabasesOperate::initDelayInfoList(DelayInfoList &delayInfoList)
{
    delayInfoList.clear();
    QString sqlString = QString("select * from delay_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            delayInfoList.append(DelayInfoClass(
                                     query.value(0).toString(),
                                     query.value(1).toInt(),
                                     query.value(2).toInt(),
                                     query.value(3).toInt(),
                                     query.value(4).toInt()));
            QCoreApplication::processEvents();
        }

        return true;
    }
#if DEBUG
    qDebug() << "initDelayInfoList error";
#endif
#if LOG
    LogFile::debug("initDelayInfoList error");
#endif
    return false;
}

bool DatabasesOperate::initMapDelayIdAndDelayTriggerConditionInfoList(
        QMap<QString, DelayTriggerConditionInfoList> &mapDelayIdAndDelayTriggerConditionInfoList,
        const DelayInfoList &delayInfoList)
{
    QSqlQuery query;
    for (int i = 0; i < delayInfoList.size(); i++)
    {
        QString delayId = delayInfoList.at(i).getDelayId();
        QString sqlString = QString("select * from delay_trigger_condition where delay_id = '%1'").arg(delayId);

        if ( query.exec( sqlString ) )
        {
            DelayTriggerConditionInfoList delayTriggerConditionInfoList;
            while ( query.next() )
            {
                delayTriggerConditionInfoList.append(DelayTriggerConditionClass(
                                                         query.value(0).toString(),
                                                         query.value(1).toString(),
                                                         query.value(2).toString(),
                                                         query.value(3).toString()));
                QCoreApplication::processEvents();
            }
            mapDelayIdAndDelayTriggerConditionInfoList[delayId] = delayTriggerConditionInfoList;
        }
        else
        {
#if DEBUG
            qDebug() << "initMapDelayIdAndDelayTriggerConditionInfoList error";
#endif
#if LOG
            LogFile::debug("initMapDelayIdAndDelayTriggerConditionInfoList error");
#endif
            return false;
        }
    }

    return true;
}

bool DatabasesOperate::initSelfHoldInfoList(SelfHoldInfoList &selfHoldInfoList)
{
    selfHoldInfoList.clear();
    QString sqlString = QString("select * from self_hold_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            selfHoldInfoList.append(SelfHoldInfoClass(
                                        query.value(0).toString(),
                                        query.value(1).toString(),
                                        query.value(2).toInt(),
                                        query.value(3).toInt()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initSelfHoldInfoList error";
#endif
#if LOG
        LogFile::debug("initSelfHoldInfoList error");
#endif
        return false;
    }
}

bool DatabasesOperate::initMapSelfHoldIdAndSelfHoldTriggerConditionInfoList(
        QMap<QString, SelfHoldTriggerConditionInfoList> &mapSelfHoldIdAndSelfHoldTriggerConditionInfoList,
        const SelfHoldInfoList &selfHoldInfoList)
{
    QSqlQuery query;
    for (int i = 0; i < selfHoldInfoList.size(); i++)
    {
        QString selfFlagId = selfHoldInfoList.at(i).getSelfFlagId();
        QString sqlString = QString("select * from self_hold_trigger_condition where self_flag_id = '%1'")
                .arg(selfFlagId);

        if ( query.exec( sqlString ) )
        {
            SelfHoldTriggerConditionInfoList selfHoldTriggerConditionInfoList;
            while ( query.next() )
            {
                selfHoldTriggerConditionInfoList.append(SelfHoldTriggerConditionClass(
                                                            query.value(0).toString(),
                                                            query.value(1).toString(),
                                                            query.value(2).toString(),
                                                            query.value(3).toString()));
                QCoreApplication::processEvents();
            }
            mapSelfHoldIdAndSelfHoldTriggerConditionInfoList[selfFlagId] = selfHoldTriggerConditionInfoList;
        }
        else
        {
#if DEBUG
            qDebug() << "initMapSelfHoldIdAndSelfHoldTriggerConditionInfoList error";
#endif
#if LOG
            LogFile::debug("initMapSelfHoldIdAndSelfHoldTriggerConditionInfoList error");
#endif
            return false;
        }
    }

    return true;
}

bool DatabasesOperate::initFailureInfoList(FailureInfoList &failureInfoList)
{
    failureInfoList.clear();
    QString sqlString = QString("select * from failure_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            failureInfoList.append(FailureInfoClass(
                                       query.value(0).toString(),
                                       query.value(1).toString(),
                                       query.value(2).toInt(),
                                       query.value(3).toInt(),
                                       query.value(4).toInt(),
                                       query.value(5).toString()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initFailureInfoList error";
#endif
#if LOG
        LogFile::debug("initFailureInfoList error");
#endif
        return false;
    }
}

bool DatabasesOperate::initMapFailureIdAndFailureTriggerConditionInfoList(
        QMap<QString, FailureTriggerConditionInfoList> &mapFailureIdAndFailureTriggerConditionInfoList,
        const FailureInfoList &failureInfoList)
{
    QSqlQuery query;
    for (int i = 0; i < failureInfoList.size(); i++)
    {
        QString failureId = failureInfoList.at(i).getFailureId();
        QString sqlString = QString("select * from failure_trigger_condition where failure_id = '%1'")
                .arg(failureId);

        if ( query.exec( sqlString ) )
        {
            FailureTriggerConditionInfoList failureTriggerConditionInfoList;
            while ( query.next() )
            {
                failureTriggerConditionInfoList.append(FailureTriggerConditionClass(
                                                           query.value(0).toString(),
                                                           query.value(1).toString(),
                                                           query.value(2).toString(),
                                                           query.value(3).toString()));
                QCoreApplication::processEvents();
            }
            mapFailureIdAndFailureTriggerConditionInfoList[failureId] = failureTriggerConditionInfoList;
        }
        else
        {
#if DEBUG
            qDebug() << "initMapFailureIdAndFailureTriggerConditionInfoList error";
#endif
#if LOG
            LogFile::debug("initMapFailureIdAndFailureTriggerConditionInfoList error");
#endif
            return false;
        }
    }

    return true;
}

bool DatabasesOperate::initDeviceStateInfoList(DeviceStateInfoList &deviceStateInfoList)
{
    deviceStateInfoList.clear();
    QString sqlString = QString("select * from device_state_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            deviceStateInfoList.append(DeviceStateInfoClass(
                                           query.value(0).toString(),
                                           query.value(1).toString(),
                                           query.value(2).toInt(),
                                           query.value(3).toString(),
                                           query.value(4).toString(),
                                           query.value(5).toInt()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initDeviceStateInfoList error";
#endif
#if LOG
        LogFile::debug("initDeviceStateInfoList error");
#endif
        return false;
    }
}

bool DatabasesOperate::initMapDeviceStateIdAndDeviceStateTriggerConditionInfoList(
        QMap<QString, DeviceStateTriggerConditionInfoList> &mapDeviceStateIdAndDeviceStateTriggerConditionInfoList,
        const DeviceStateInfoList &deviceStateInfoList)
{
    QSqlQuery query;
    for (int i = 0; i < deviceStateInfoList.size(); i++)
    {
        QString deviceStateId = deviceStateInfoList.at(i).getDeviceStateId();
        QString sqlString = QString("select * from device_state_trigger_condition where device_state_id = '%1'")
                .arg(deviceStateId);

        if ( query.exec( sqlString ) )
        {
            DeviceStateTriggerConditionInfoList deviceStateTriggerConditionInfoList;
            while ( query.next() )
            {
                deviceStateTriggerConditionInfoList.append(DeviceStateTriggerConditionClass(
                                                               query.value(0).toString(),
                                                               query.value(1).toString(),
                                                               query.value(2).toInt(),
                                                               query.value(3).toDouble()));
                QCoreApplication::processEvents();
            }
            mapDeviceStateIdAndDeviceStateTriggerConditionInfoList[deviceStateId] = deviceStateTriggerConditionInfoList;
        }
        else
        {
#if DEBUG
            qDebug() << "initMapDeviceStateIdAndDeviceStateTriggerConditionInfoList error";
#endif
#if LOG
            LogFile::debug("initMapDeviceStateIdAndDeviceStateTriggerConditionInfoList error");
#endif
            return false;
        }
    }

    return true;
}

bool DatabasesOperate::initAllBaseInfoList(BaseInfoList &allBaseInfoList)
{
    allBaseInfoList.clear();
    QString sqlString = QString("select * from base_info");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            allBaseInfoList.append(BaseInfoClass(
                                       query.value(0).toString(),
                                       query.value(1).toInt(),
                                       query.value(2).toString(),
                                       query.value(3).toString(),
                                       query.value(4).toString(),
                                       query.value(5).toInt(),
                                       query.value(6).toInt(),
                                       query.value(7).toInt(),
                                       query.value(8).toInt()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initAllBaseInfoList error";
#endif
#if LOG
        LogFile::debug("initAllBaseInfoList error");
#endif
        return false;
    }


    return false;
}

bool DatabasesOperate::initLadderDiagramInfoList(LadderDiagramInfoList &ladderDiagramInfoList)
{
    ladderDiagramInfoList.clear();
    QString sqlString = QString("select * from ladder_diagram where bus_id != ''");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            ladderDiagramInfoList.append(LadderDiagramClass(
                                             query.value(0).toString(),
                                             query.value(1).toString(),
                                             query.value(2).toString(),
                                             query.value(3).toString(),
                                             query.value(4).toString(),
                                             query.value(5).toString()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initLadderDiagramInfoList error";
#endif
#if LOG
        LogFile::debug("initLadderDiagramInfoList error");
#endif
        return false;
    }
}

//获取故障历史列表
bool DatabasesOperate::initFailureHistoryList(FailureHistoryRecordInfoList &failureHistoryRecordInfoList)
{
    failureHistoryRecordInfoList.clear();
    QString sqlString = QString("select * from failure_history_record");
    QSqlQuery query;

    if ( query.exec( sqlString ) )
    {
        while ( query.next() )
        {
            failureHistoryRecordInfoList.append(FailureHistoryRecordClass(query.value(0).toString(),
                                                                          query.value(1).toInt(),
                                                                          query.value(2).toString(),
                                                                          query.value(3).toString(),
                                                                          query.value(4).toString()));
            QCoreApplication::processEvents();
        }

        return true;
    }
    else
    {
#if DEBUG
        qDebug() << "initFailureHistoryList error";
#endif
#if LOG
        LogFile::debug("initFailureHistoryList error");
#endif
        return false;
    }
}

bool DatabasesOperate::insertFailureHistoryListToDB(FailureHistoryRecordInfoList &failureHistoryRecordInfoList)
{
    QSqlQuery sqlQuery;
    QString sqlString;

    foreach (FailureHistoryRecordClass failureHistoryInfo, failureHistoryRecordInfoList)
    {
        bool isExists = false;
        //分情况进行插表
        //首先将发生故障时间一样的故障进行整合
        if(!failureHistoryInfo.getEndTime().isEmpty())
        {
            //结束时间不为空进行数据库更新操作
            //ID和开始时间唯一确定一条故障
            sqlString = QString("update failure_history_record set end_time='%1' where failure_id='%2' and start_time='%3'")
                    .arg(failureHistoryInfo.getEndTime(),failureHistoryInfo.getFailureId(),failureHistoryInfo.getStartTime());

            if (!sqlQuery.exec(sqlString))
            {
#if DEBUG
                qDebug() << "insertFailureHistoryListToDB update error";
#endif
#if LOG
                LogFile::debug("insertFailureHistoryListToDB update error");
#endif
                return false;
            }
        }else
        {
            //有两种情况，一种是一直没解决的故障，一种是新产生的故障
            sqlString = QString("select count(*) from failure_history_record where failure_id='%1' and start_time='%2'").arg(failureHistoryInfo.getFailureId(),failureHistoryInfo.getStartTime());
            if ( sqlQuery.exec( sqlString ) )
            {
                while(sqlQuery.next())
                {
                    //已经存在该故障，且该故障没有解决
                    if(sqlQuery.value(0).toInt() != 0)
                    {
                        isExists = true;
                    }
                }
            }

            if(isExists)
            {
                //本故障处理完毕，处理下一个故障
                continue;
            }

            //其次将新的故障插入表

            //在执行插入操作之前判断表中有多少数据，超过10000条删除旧的
            sqlString = QString("select count(*) from failure_history_record ");
            if ( sqlQuery.exec( sqlString ) )
            {
                while(sqlQuery.next())
                {
                    if(sqlQuery.value(0).toInt() >= FAILURE_HISTORY_RECORD_MAXNUM)
                    {
                        //删除旧的记录
                        sqlString = QString("delete from failure_history_record where start_time = (select min(start_time) from failure_history_record);");
                        if (!sqlQuery.exec(sqlString))
                        {
#if DEBUG
                            qDebug() << "insertFailureHistoryListToDB deleteMin error";
#endif
#if LOG
                            LogFile::debug("insertFailureHistoryListToDB deleteMin error");
#endif
                            return false;
                        }
                        break;
                    }
                }
            }

            sqlString = QString("insert into failure_history_record values('%1','%2','%3','%4','%5')").arg(
                        failureHistoryInfo.getFailureId(),
                        QString::number(failureHistoryInfo.getFailureLv()),
                        failureHistoryInfo.getStartTime(),
                        failureHistoryInfo.getEndTime(),
                        failureHistoryInfo.getFailureName());

            if (!sqlQuery.exec(sqlString))
            {
#if DEBUG
                qDebug() << "insertFailureHistoryListToDB insert error";
#endif
#if LOG
                LogFile::debug("insertFailureHistoryListToDB insert error");
#endif
                return false;
            }
        }
        QCoreApplication::processEvents();
    }
    return true;
}
