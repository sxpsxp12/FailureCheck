#ifndef BUSIDWIDGETPAGE_H
#define BUSIDWIDGETPAGE_H

#include <QWidget>
#include <QMap>

#include "baseinfoclass.h"
#include "realtimedataclass.h"

namespace Ui {
class BusIdWidgetPage;
}

class QTableWidgetItem;
class InterfaceLampHole;

class BusIdWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit BusIdWidgetPage(BaseInfoList baseInfoList, int pageNum, QWidget *parent = 0);
    ~BusIdWidgetPage();

    void initTableWidget(BaseInfoList baseInfoList, int pageNum);
    void refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList, int curPageNum);

signals:
    void signal_tableWidgetItemClicked(QTableWidgetItem*);

private slots:
    void slot_tableWidgetItemClicked(QTableWidgetItem* tableWidgetItem);

private:
    Ui::BusIdWidgetPage *ui;

    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;        //Map<坐标,QTableWidgetItem指针>

    RealtimeDataInfoList m_currentRealtimeDataInfoList;
    RealtimeDataInfoList m_historyRealtimeDataInfoList;
};

#endif // BUSIDWIDGETPAGE_H
