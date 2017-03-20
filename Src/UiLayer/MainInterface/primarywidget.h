#ifndef PRIMARYWIDGET_H
#define PRIMARYWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>
#include "baseinfoclass.h"
#include "realtimedataclass.h"
#include "failureinfoclass.h"
#include "primarylistwidgetpage.h"
#include "primarywidgettableitemmodel.h"    //tablewidget的模板

class primaryWidgetTableItemModel;
class RequestDataThread;

namespace Ui {
class PrimaryWidget;
}

class PrimaryWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrimaryWidget(RequestDataThread *, QWidget *parent = 0);
    ~PrimaryWidget();

    void initTableWidget();        //初始化TableWidget
    void initWidget();             //初始化界面上的图片和显示内容
    static FailureInfoList failureInfoList;

signals:
    void signal_setGuiContainerCurrentWidget(int);            //切换界面的信号
    void signal_setClickedListItem(int index);         //通过信号将被点击的listItem的故障信息类传到故障列表中
    void signal_sendPageData();                //发送故障信息到页面的信号

private slots:
    void on_Btn_secondScreen_clicked();
    void slot_receiveListWidgetItemClickedData(int index);       //接收点击ListWidget项的槽函数
    void slot_pbt_pageUpClicked();
    void slot_pbt_pageDownClicked();
    void slot_receivePrimaryWidgetData();
    void slot_receiveMasterFlag(bool flag);

private:
    Ui::PrimaryWidget *ui;

    RequestDataThread *m_requestDataThread;
    BaseInfoList m_interfaceBaseInfoList;
    PrimaryListWidgetPage *m_primaryListWidgetPage;

    RealtimeDataInfoList realtimeDataInfoList;

    int m_totalPage;       //总页数
    int m_currentPage;     //当前页数
    int m_createPage;      //创建页数
    QString verhicleNum;   //车型,车号
    QMap<QString,QString> m_verhicleInfoMapKeyAndValue;
    QMap<QString, primaryWidgetTableItemModel*> m_mapCoordAndTableWidgetItemPtr;        //Map<坐标,QTableWidgetItem指针>
};

#endif // PRIMARYWIDGET_H
