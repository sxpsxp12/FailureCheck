#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H

#include <QWidget>
#include <QStringList>
#include <QString>
#include "importfunction.h"
#include "exportfunction.h"
#include "requestdatathread.h"
#include "custommsgbox.h"
#include "fileinfoclass.h"
#include "cbaseoperate.h"

namespace Ui {
class ImportExport;
}

class ImportExport : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImportExport(RequestDataThread *requestDataThread,QWidget *parent = 0);
    ~ImportExport();
    
    void initImportExportWidget();

private slots:
    void on_pbt_import_clicked();

    void on_pbt_export_clicked();

    void slot_customMsgBoxOkFunctionFlag(const int okFunctionFlag);
    void slot_customMsgBoxCancleFunctionFlag(const int cancleFunctionFlag);

private:
    Ui::ImportExport *ui;
    QString m_usbPath;

    RequestDataThread *m_requestDataThread;

    ImportFunction *m_importFunction;
    ExportFunction *m_exportFunction;
    CustomMsgBox *m_customMsgBox;

    void initWidget();
    //检测USB
    bool detectUSB();
};

#endif // IMPORTEXPORT_H
