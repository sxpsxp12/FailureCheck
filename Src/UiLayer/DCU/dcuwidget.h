#ifndef MICROCOMPUTERARK_H
#define MICROCOMPUTERARK_H

#include <QWidget>
#include <QMap>
#include "realtimedataclass.h"
#include "baseinfoclass.h"

class RequestDataThread;
class DCUWidgetPage;

namespace Ui {
class DCUWidget;
}

class DCUWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DCUWidget(RequestDataThread *, QWidget *parent = 0);
    ~DCUWidget();
    void initWidget();

signals:

private slots:
    void slot_pbt_pageUpClicked();
    void slot_pbt_pageDownClicked();
    void slot_receiveDcuWidgetData();          //从GuiContainer接收实时数据信号槽

private:
    Ui::DCUWidget *ui;
    RequestDataThread *m_requestDataThread;

    BaseInfoList m_interfaceBaseInfoList;              //存放微机柜一架基本信息
    BaseInfoList m_interfaceBaseInfoList2;             //存放微机柜二架基本信息
    DCUWidgetPage *m_DCUWidgetPage;

    int m_totalPage;         //总页数
    int m_currentPage;       //当前页数
    RealtimeDataInfoList m_realtimeDataInfoList;
    QMap<int,DCUWidgetPage *> m_mapIntAndDCUWidgetPagePtr;
};

#endif // MICROCOMPUTERARK_H
