#ifndef INTERFACELAMPHOLEPAGE_H
#define INTERFACELAMPHOLEPAGE_H

#include <QWidget>
#include <QList>
#include <QMap>

#include "baseinfoclass.h"
#include "realtimedataclass.h"

namespace Ui {
class InterfaceLampHolePage;
}

class QTableWidgetItem;

class InterfaceLampHolePage : public QWidget
{
    Q_OBJECT
    
public:
    explicit InterfaceLampHolePage(BaseInfoList baseInfoList, int pageNum, QWidget *parent = 0);
    ~InterfaceLampHolePage();

    void initTableWidgetUI();
    void initTableWidget(BaseInfoList baseInfoList, int pageNum);
    void refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList, int curPageNum);
private slots:
    void slot_searchId(int pageNum,int xCoord,int yCoord);
    
private:
    Ui::InterfaceLampHolePage *ui;

    QString m_preCoordBusIdStr;
    QString m_preCoordInterfacesStr;
    QString m_preCoordIndicatorLightStr;
    QString m_preCoordMeaningStr;

    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;        //Map<坐标,QTableWidgetItem指针>

    RealtimeDataInfoList m_currentRealtimeDataInfoList;
    RealtimeDataInfoList m_historyRealtimeDataInfoList;
};

#endif // INTERFACELAMPHOLEPAGE_H
