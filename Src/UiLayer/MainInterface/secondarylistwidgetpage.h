#ifndef SECONDARYLISTWIDGETPAGE_H
#define SECONDARYLISTWIDGETPAGE_H

#include "failureinfoclass.h"

#include <QWidget>
#include <QModelIndex>
#include <QListWidgetItem>
#include <QMap>

namespace Ui {
class SecondaryListWidgetPage;
}

class SecondaryListWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit SecondaryListWidgetPage(int pageNum, QWidget *parent = 0);
    ~SecondaryListWidgetPage();

private slots:
    void slot_receivePageData();        //接收上层传来的数据

private:
    Ui::SecondaryListWidgetPage *ui;

    int m_pageNum;                          //本页面的页数
    int m_createItemNum;                    //QListWidget已创建的行数

    FailureInfoList m_failureInfoList;

    QMap<QString, QListWidgetItem*> m_mapCoordAndListWidgetItemPtr;
};

#endif // SECONDARYLISTWIDGETPAGE_H
