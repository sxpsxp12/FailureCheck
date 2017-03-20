#ifndef DCUWIDGETPAGE_H
#define DCUWIDGETPAGE_H

#include <QWidget>
#include <QMap>

#include "baseinfoclass.h"
#include "realtimedataclass.h"

namespace Ui {
class DCUWidgetPage;
}

class QTableWidgetItem;

class DCUWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit DCUWidgetPage(BaseInfoList baseInfoList, int pagNum = 0, QWidget *parent = 0);
    ~DCUWidgetPage();

    void initTableWidgetUI();
    void initTableWidget(BaseInfoList baseInfoList, int pageNum);

    void refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList);
private slots:

private:
    Ui::DCUWidgetPage *ui;

    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;        //Map<坐标,QTableWidgetItem指针>
};

#endif // DCUWIDGETPAGE_H
