#ifndef INTERFACELAMPHOLE_H
#define INTERFACELAMPHOLE_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QMap>
#include "baseinfoclass.h"
#include "realtimedataclass.h"

namespace Ui {
class InterfaceLampHole;
}

class RequestDataThread;
class InterfaceLampHolePage;

class InterfaceLampHole : public QWidget
{
    Q_OBJECT
    
public:
    explicit InterfaceLampHole(RequestDataThread *, QWidget *parent = 0);
    ~InterfaceLampHole();

    void initWidget();

    void updateLabel(int currentPage, int totalPage);   //刷新label
    void searchLcuInfoRow(int listNum);

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_searchId(int,int,int);
    void signal_setFocusToBusIdWidget();                  //设置按钮焦点信号

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pbt_pageDown_clicked();
    void on_pbt_pageUp_clicked();
    void on_pbt_homePage_clicked();
    void on_pbt_endPage_clicked();

    void slot_tableWidgetItemClicked(QTableWidgetItem* tableWidgetItem);
    void slot_receiveInterfaceLampHoleWidgetData();

    void on_pbt_clear_clicked();
    void slot_pbt_searchClicked();
    void on_cbx_enumSelection_activated(const QString &arg1);

    void slot_setFocus();        //设置按钮焦点

private:
    Ui::InterfaceLampHole *ui;

    RequestDataThread *m_requestDataThread;

    BaseInfoList m_interfaceBaseInfoList;
    InterfaceLampHolePage *m_interfaceLampHolePage;
    QStringList m_fuzzySearchList;

    int m_totalPage;                         //总页数
    int m_currentPage;                       //当前页数

    int m_pageNum;                        //当前个数
    int m_totalNum;                          //总个数

    bool m_searchFlag;                      //搜索标志位
    int m_totalSearchNum;                   //总搜索数
    int m_currentSearchNum;                 //当前搜索数

    QString m_searchId;
    BaseInfoList m_baseInfoList;

    RealtimeDataInfoList m_realtimeDataInfoList;
    QMap<int,InterfaceLampHolePage*> m_mapIntAndInterfaceLampHolePagePtr;
};

#endif // INTERFACELAMPHOLE_H
