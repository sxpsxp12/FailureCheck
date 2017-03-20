#include "importthread.h"

ImportThread::ImportThread(QObject *parent) :
    QThread(parent)
{
}

void ImportThread::setGoalPath(QString goalPath)
{
    m_goalPath = goalPath;
}

FileInfoClassList ImportThread::getErrorFileInfoList()
{
    return m_errorFileInfoList;
}

void ImportThread::run()
{
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
    m_fileInfoList.clear();
    m_errorFileInfoList.clear();

    m_fileInfoList = filesOperate->initDatabases();
    foreach (FileInfoClass fileInfo, m_fileInfoList) {
        //将出错的文档进行记录
        if(!fileInfo.getFileFormatOk() || !fileInfo.getInitDataBaseOk())
        {
            m_errorFileInfoList.append(fileInfo);
        }
    }
}
