#ifndef EXPORTFUNCTION_H
#define EXPORTFUNCTION_H

#include <QWidget>
#include "requestdatathread.h"
#include <QString>
#include <QStringList>
#include "custommsgbox.h"
#include "cbaseoperate.h"
#include "exportthread.h"
#include <QTimer>

namespace Ui {
class ExportFunction;
}

class ExportFunction : public QWidget
{
    Q_OBJECT
    
public:
    explicit ExportFunction(RequestDataThread *requestDataThread,QWidget *parent = 0);
    ~ExportFunction();
    void getUsbPathAndDir(const QString &path,const QStringList &dir);
    
private slots:
    void on_pbt_commit_clicked();
    void slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag);
    void slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag);
    void slot_exportThreadFinished();
    void slot_timerTimeOut();
private:
    Ui::ExportFunction *ui;
    RequestDataThread *m_requestDataThread;
    ExportThread *m_exportThread;
    QMap<QString,QString> m_mapVerhicleInfoKeyAndValue;
    QString m_usbPath;
    QStringList m_usbDir;
    CustomMsgBox *m_customMsgBox;
    QString m_outputPath;
    QTimer *m_msgLabelTimer;
};

#endif // EXPORTFUNCTION_H
