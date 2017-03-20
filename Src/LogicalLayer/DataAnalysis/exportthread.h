#ifndef EXPORTTHREAD_H
#define EXPORTTHREAD_H

#include <QThread>
#include <QString>

class ExportThread : public QThread
{
    Q_OBJECT
public:
    explicit ExportThread(QObject *parent = 0);
    
    void setOutputPath(QString outputPath);
    void isExportNewData(bool isNewData);

protected:
    void run();

private:
    QString m_outputPath;
    bool m_flagNewData;
};

#endif // EXPORTTHREAD_H
