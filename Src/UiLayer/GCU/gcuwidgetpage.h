#ifndef GCUWIDGETPAGE_H
#define GCUWIDGETPAGE_H

#include <QWidget>
#include <QMap>
#include <QTableWidgetItem>

#include "enumtypeheader.h"
#include "baseinfoclass.h"
#include "realtimedataclass.h"

namespace Ui {
class GCUWidgetPage;
}

class GCUWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit GCUWidgetPage(BaseInfoList baseInfoList, int pageNum,QWidget *parent = 0);
    ~GCUWidgetPage();

    void initTableWidgetUI();
    void initTableWidget(BaseInfoList baseInfoList, int pageNum);
    void refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList);

private slots:

private:
    Ui::GCUWidgetPage *ui;

    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;
};

#endif // GCUWIDGETPAGE_H
