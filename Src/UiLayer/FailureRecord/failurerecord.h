#ifndef FAILURERECORD_H
#define FAILURERECORD_H

#include <QWidget>
#include "failurehistoryrecordclass.h"
#include "databasesoperate.h"
#include <QMap>
#include <QTableWidgetItem>
#include <QDebug>
#include "requestdatathread.h"

#define ONEPAGE_ITEM_ROWNUM 17

namespace Ui {
class FailureRecord;
}

class FailureRecord : public QWidget
{
    Q_OBJECT
    
public:
    explicit FailureRecord(RequestDataThread *requestDataThread,QWidget *parent = 0);
    ~FailureRecord();
    void initTableWidget();
    void initLabel();
    void updateTableWidgetContent(int &page);
private slots:
    void on_pbt_lastPage_clicked();

    void on_pbt_nextPage_clicked();

    void on_pbt_headPage_clicked();

    void on_pbt_endPage_clicked();

    void slot_ReceiveFailureRecordWidgetData();

private:
    Ui::FailureRecord *ui;
    RequestDataThread *m_requestDataThread;
    FailureHistoryRecordInfoList m_failureHistoryRecordInfoList;
    QMap<QString,QTableWidgetItem *> m_mapAddressAndItemPtr;

    int m_currentPage;
    int m_TotalPageNum;
};

#endif // FAILURERECORD_H
