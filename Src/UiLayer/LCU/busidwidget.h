#ifndef BUSIDWIDGET_H
#define BUSIDWIDGET_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMap>
#include "baseinfoclass.h"
#include "realtimedataclass.h"

class RequestDataThread;
class BusIdWidgetPage;

namespace Ui {
class BusIdWidget;
}

class BusIdWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit BusIdWidget(RequestDataThread *, QWidget *parent = 0);
    ~BusIdWidget();

    void initWidget();

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_tableWidgetItemClicked(QTableWidgetItem*);
    void signal_setFocusToInterfaceLampHole();                  //设置按钮焦点信号

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pbt_pageDown_clicked();
    void on_pbt_pageUp_clicked();

    void slot_receiveBusIdWidgetData();          //从GuiContainer接收实时数据信号槽
    void slot_tableWidgetItemClicked(QTableWidgetItem* tableWidgetItem);

    void slot_setFocus();         //设置按钮焦点

private:
    Ui::BusIdWidget *ui;

    RequestDataThread *m_requestDataThread;
    BaseInfoList m_interfaceBaseInfoList;
    BusIdWidgetPage *m_busIdWidgetPage;

    int m_totalPage;       //总页数
    int m_currentPage;     //当前页数

    RealtimeDataInfoList m_realtimeDataInfoList;
    QMap<int,BusIdWidgetPage *> m_mapIntAndBusIdWidgetPagePtr;
};

#endif // BUSIDWIDGET_H
