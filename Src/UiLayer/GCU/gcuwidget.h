#ifndef GCUWIDGET_H
#define GCUWIDGET_H

#include <QWidget>
#include <QMap>
#include "enumtypeheader.h"
#include "gcuwidgetpage.h"
#include "baseinfoclass.h"
#include "realtimedataclass.h"

class RequestDataThread;

namespace Ui {
class GCUWidget;
}

class GCUWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GCUWidget(RequestDataThread *, QWidget *parent = 0);
    ~GCUWidget();

    void initTableWidget();

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_setFocusToLGWidget();                  //设置按钮焦点信号

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pbt_pageUp_clicked();
    void on_pbt_pageDown_clicked();

    void slot_receiveGcuWidgetData();

    void slot_setFocus();           //设置按钮焦点

private:
    Ui::GCUWidget *ui;
    BaseInfoList m_gcuBaseInfoList;

    RequestDataThread *m_requestDataThread;
    GCUWidgetPage *m_gcuWidgetPage;

    int m_currentPage;
    int m_totalPage;

    RealtimeDataInfoList m_realTimeDataInfoList;
    QMap<int,GCUWidgetPage *> m_mapIntAndGCUWidgetPagePtr;
};

#endif // GCUWIDGET_H
