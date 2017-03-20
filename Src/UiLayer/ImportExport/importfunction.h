#ifndef IMPORTFUNCTION_H
#define IMPORTFUNCTION_H

#include <QWidget>
#include "requestdatathread.h"
#include <QString>
#include <QStringList>
#include "custommsgbox.h"
#include "cbaseoperate.h"
#include "importthread.h".h
#include <QTimer>

namespace Ui {
class ImportFunction;
}

class ImportFunction : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImportFunction(RequestDataThread *requestDataThread,QWidget *parent = 0);
    ~ImportFunction();

    void getUsbPathAndDir(const QString &path,const QStringList &dir);
    
private slots:
    void on_pbt_commit_clicked();

    void slot_InitDataBaseIsSuccess(FileInfoClassList &fileInfoList);
    void slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag);
    void slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag);

    void slot_importThreadFinished();

    void slot_timerTimeOut();
signals:

private:
    Ui::ImportFunction *ui;
    CustomMsgBox *m_customMsgBox;
    RequestDataThread *m_requestDataThread;
    ImportThread *m_importThread;
    QString m_usbPath;
    QStringList m_usbDir;
    QTimer *m_msgLabelTimer;
};

#endif // IMPORTFUNCTION_H
