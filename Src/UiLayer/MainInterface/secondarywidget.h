#ifndef SECONDARYWIDGET_H
#define SECONDARYWIDGET_H

#include <QWidget>
#include <QListWidgetItem>

#include "baseinfoclass.h"
#include "realtimedataclass.h"
#include "failureinfoclass.h"
#include "secondarylistwidgetpage.h"
#include "secondwidgettableitemmodelclass.h"

class RequestDataThread;

namespace Ui {
class SecondaryWidget;
}

class SecondaryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SecondaryWidget(RequestDataThread *requestDataThread, QWidget *parent = 0);
    ~SecondaryWidget();

    void initTableWidget();         //初始化tableWidget
    void initWidget();              //初始化界面上的显示内容
    static FailureInfoList failureInfoList;
signals:
    void signal_setGuiContainerCurrentWidget(int);        //切换界面的信号
    void signal_sendPageData();              //发送故障信息到页面的信号

private slots:
    void on_pushButton_clicked();
    void slot_pbt_pageUpClicked();
    void slot_pbt_pageDownClicked();
    void slot_receiveSecondaryWidgetData();

private:
    Ui::SecondaryWidget *ui;

    RequestDataThread *m_requestDataThread;
    BaseInfoList m_interfaceBaseInfoList;
    SecondaryListWidgetPage *m_secondaryListWidgetPage;

    int m_totalPage;       //总页数
    int m_currentPage;     //当前页数
    int m_createPage;      //创建页数

    QMap<QString, SecondWidgetTableItemModelClass*> m_mapCoordAndTableWidgetItemPtr;    //Map<坐标,QTableWidgetItem指针>
    QMap<QString, int> m_mapCoordAndmachineType;                                        //Map<坐标,机器类型>

    RealtimeDataInfoList m_currentRealtimeDataInfoList;
    RealtimeDataInfoList m_historyRealtimeDataInfoList;
};

#endif // SECONDARYWIDGET_H
