#ifndef LGWIDGETPAGE_H
#define LGWIDGETPAGE_H

#include <QWidget>
#include <QMap>
#include <QTableWidgetItem>

#include "realtimedataclass.h"
#include "baseinfoclass.h"
#include "enumtypeheader.h"

namespace Ui {
class LGWidgetPage;
}

class LGWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit LGWidgetPage(BaseInfoList baseInfoList, int pageNum,QWidget *parent = 0);
    ~LGWidgetPage();

    void initTableWidgetUI();
    void initTableWidget(BaseInfoList baseInfoList, int pageNum);

    void refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList);
private slots:

private:
    Ui::LGWidgetPage *ui;

    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;
};

#endif // LGWIDGETPAGE_H
