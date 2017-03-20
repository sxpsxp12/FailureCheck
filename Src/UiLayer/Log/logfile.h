#ifndef LOGFILE_H
#define LOGFILE_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include "enumtypeheader.h"

class LogFile
{
public:
    LogFile();

    static void setFilePath(const QString &filePathStr);

    //static void initLogFile();
   // static void error(const QString &errorStr);
   // static void warning(const QString &warningStr);
    static bool debug(const QString &debugStr);
   // static void infor(const QString &inforStr);

    static QString currentTime();

private:
    static QFile *m_file;
    static QString m_filePathStr;
};

#endif // LOGFILE_H
