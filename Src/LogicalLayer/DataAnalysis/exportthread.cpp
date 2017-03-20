#include "exportthread.h"
#include "enumtypeheader.h"
#include "cbaseoperate.h"

ExportThread::ExportThread(QObject *parent) :
    QThread(parent)
{
}

void ExportThread::setOutputPath(QString outputPath)
{
    m_outputPath = outputPath;
}

void ExportThread::isExportNewData(bool isNewData)
{
    if (isNewData)
        m_flagNewData = true;
    else
        m_flagNewData = false;
}

void ExportThread::run()
{
    if (m_flagNewData)
    {
        //导入最新的数据，删除旧的数据,然后拷贝到旧书据文件夹中，将最新的数据文件夹清空，
        CBaseOperate::exportDataToUSB(NEWDATA,m_outputPath);
        CBaseOperate::clearHistoryData();
        CBaseOperate::copyNewDataToHistoryData();
        CBaseOperate::clearNewData();

        //初始化故障记录
        CBaseOperate::exportAfterInit();
    }
    else
    {
        //导出上一次的数据,然后删除该路径下所有文件，将最新的数据复制到该文件夹下
        CBaseOperate::exportDataToUSB(HISTORYDATA,m_outputPath);
        CBaseOperate::clearHistoryData();
    }
}
