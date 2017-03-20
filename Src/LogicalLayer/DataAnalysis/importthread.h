#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H

#include "fileinfoclass.h"
#include "filesoperate.h"
#include "databasesoperate.h"

#include <QThread>
#include <QString>
#include <QList>

class ImportThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportThread(QObject *parent = 0);

    void setGoalPath(QString goalPath);
    FileInfoClassList getErrorFileInfoList();

protected:
    void run();

private:
    FileInfoClassList m_fileInfoList;
    FileInfoClassList m_errorFileInfoList;
    FilesOperate *m_filesOperate;
    QString m_goalPath;
};

#endif // IMPORTTHREAD_H
