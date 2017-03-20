#include "logfile.h"

#include <QDebug>
#include <QTime>
#include <QStringList>
#include <QDate>
#include "cbaseoperate.h"

QFile *LogFile::m_file = new QFile;
QString LogFile::m_filePathStr = QString();

LogFile::LogFile()
{
}

void LogFile::setFilePath(const QString &filePathStr)
{
    m_filePathStr = filePathStr;
}



bool LogFile::debug(const QString &debugStr)
{
    bool ret=false;
    QString message="";
    QString filePath = m_filePathStr + "LOG.txt";
    QFile *RealFile=new QFile;
     if(RealFile!=NULL)
     {
        do
         {

             RealFile->setFileName(filePath);
             if(RealFile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
             {
                 QTextStream stream(RealFile);
                 message = currentTime() + " : " + "debug   >> " + debugStr + "\r\n";
                 stream << message;
                 RealFile->close();
                 ret=true;
             }
          }while(false);
      }
     delete RealFile;
     return ret;
}
#if 0
void LogFile::initLogFile()
{
    if(CBaseOperate::getLogFileNumber())
    {
        //判断是否大于文件个数的限制7个
        if(CBaseOperate::logFileNumber > LOGFILE_SIZE)
        {
            //删除旧的文件
            if(CBaseOperate::limitLogFileNumber())
            {
                CBaseOperate::logFileNumber--;
            }
        }
    }
}

void LogFile::error(const QString &errorStr)
{
    QString message;
    QString filePath;

    filePath = m_filePathStr + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";

    //判断是否大于文件个数的限制7个
    if(CBaseOperate::logFileNumber > LOGFILE_SIZE)
    {
        //删除旧的文件
        if( CBaseOperate::limitLogFileNumber() )
        {
            CBaseOperate::logFileNumber--;
        }
    }

    //满足条件，不大于7
    if(CBaseOperate::oldLogFileName.compare(filePath) != 0)
    {
        CBaseOperate::logFileNumber++;
        if(CBaseOperate::oldLogFileName != NULL)
        {
            m_file->close();
        }

        CBaseOperate::oldLogFileName = filePath;
        m_file->setFileName(filePath);
        if(!m_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
#if DEBUG
            qDebug() << "error openLogFile error !";
#endif
            return ;
        }
    }

    QTextStream stream(m_file);
    message = currentTime() + " : " + "error   >> " + errorStr;
    stream << message << "\n";
}

void LogFile::warning(const QString &warningStr)
{
    QString message;
    QString filePath;
    filePath = m_filePathStr + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";

    //判断是否大于文件个数的限制7个
    if(CBaseOperate::logFileNumber > LOGFILE_SIZE)
    {
        //删除旧的文件
        if( CBaseOperate::limitLogFileNumber() )
        {
            CBaseOperate::logFileNumber--;
        }
    }

    //满足条件，不大于7
    if(CBaseOperate::oldLogFileName.compare(filePath) != 0)
    {
        CBaseOperate::logFileNumber++;
        if(CBaseOperate::oldLogFileName != NULL)
        {
            m_file->close();
        }

        CBaseOperate::oldLogFileName = filePath;
        m_file->setFileName(filePath);
        if(!m_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
#if DEBUG
            qDebug() << "warning openLogFile error !";
#endif
            return ;
        }
    }

    QTextStream stream(m_file);
    message = currentTime() + " : " + "warning >> " + warningStr;
    stream << message << "\n";
}



void LogFile::infor(const QString &inforStr)
{
    QString message;
    QString filePath;
    filePath = m_filePathStr + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";

    //判断是否大于文件个数的限制7个
    if(CBaseOperate::logFileNumber > LOGFILE_SIZE)
    {
        //删除旧的文件
        if( CBaseOperate::limitLogFileNumber() )
        {
            CBaseOperate::logFileNumber--;
        }
    }

    //满足条件，不大于7
    if(CBaseOperate::oldLogFileName.compare(filePath) != 0)
    {
        CBaseOperate::logFileNumber++;
        if(CBaseOperate::oldLogFileName != NULL)
        {
            m_file->close();
        }

        CBaseOperate::oldLogFileName = filePath;
        m_file->setFileName(filePath);
        if(!m_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
#if DEBUG
            qDebug() << "infor openLogFile error !";
#endif
            return ;
        }
    }

    QTextStream stream(m_file);
    message = currentTime() + " : " + "infor   >> " + inforStr;
    stream << message << "\n";
}
#endif

QString LogFile::currentTime()
{
    QString time;

    time = QTime::currentTime().toString("hh") + QString::fromUtf8("时") +
            QTime::currentTime().toString("mm") + QString::fromUtf8("分") +
            QTime::currentTime().toString("ss") + QString::fromUtf8("秒") +
            QTime::currentTime().toString("zzz") + QString::fromUtf8("毫秒");

    return time;
}
