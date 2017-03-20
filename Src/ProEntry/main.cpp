#include <QtGui/QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QMetaType>
#include <QSqlDatabase>
#include <QDebug>
#include <QDateTime>

#include "databasesoperate.h"
#include "filesoperate.h"
#include "realtimedataclass.h"
#include "failureinfoclass.h"
#include "ladderdiagramclass.h"
#include "canoperate.h"
#include "enumtypeheader.h"
#include "logfile.h"
#include "splashscreen.h"
#include "cbaseoperate.h"

int main(int argc, char *argv[])
{
    int res;
    //设置编码格式
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QApplication a(argc, argv);

    qRegisterMetaType<RealtimeDataInfoList>("RealtimeDataInfoList");
    qRegisterMetaType<FailureInfoList>("FailureInfoList");
    qRegisterMetaType<LadderDiagramInfoList>("LadderDiagramInfoList");
    qRegisterMetaType<FailureHistoryRecordInfoList>("FailureHistoryRecordInfoList");
    qRegisterMetaType<FileInfoClassList>("FileInfoClassList");

#if LOGW
    LogFile::setFilePath(QString("%1").arg(LOGPATH));
    //LogFile::initLogFile();
#endif

    if (!DatabasesOperate::connectDatabase())
    {
#if DEBUG
        qDebug() << "connectDataBase error!";
#endif
#if LOG
        LogFile::debug("connectDataBase error!");
#endif
        return 1;
    }

    //语音播报初始化
/*    if(CBaseOperate::voicePlayInit() < 0)
    {
        return 1;
    }
*/
    SplashScreen *splashScreen = new SplashScreen;
    splashScreen->move((QApplication::desktop()->width() - splashScreen->width())/2,
                       (QApplication::desktop()->height() - splashScreen->height())/2);
    splashScreen->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    splashScreen->show();

#if LOGW
    QString s1 = "the software at " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " start";
    LogFile::debug(s1);
    //LogFile::initLogFile();
#endif



    res = a.exec();
    DatabasesOperate::db.close();
    CBaseOperate::clostVoiceFd();
    return res;
}
