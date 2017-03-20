#ifndef PRIMARYLISTWIDGETPAGE_H
#define PRIMARYLISTWIDGETPAGE_H

#include <QWidget>
#include <QModelIndex>
#include <QListWidgetItem>
#include <QMap>

#include "failureinfoclass.h"

namespace Ui {
class PrimaryListWidgetPage;
}

class PrimaryListWidgetPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit PrimaryListWidgetPage(int pageNum, QWidget *parent = 0);
    ~PrimaryListWidgetPage();

signals:
    void signal_sendListWidgetItemClickedData(int);

private slots:
    void slot_receivePageData();               //接收上层传来的数据
    void slot_listWidgetItemClicked(QModelIndex modelIndex);                  //点击ListWidget项的槽函数

private:
    Ui::PrimaryListWidgetPage *ui;

    int m_pageNum;                          //本页面的页数
    int m_createItemNum;                    //QListWidget已创建的行数

    //FailureInfoList m_failureInfoList;

    QMap<QString, QListWidgetItem*> m_mapCoordAndListWidgetItemPtr;
};

#endif // PRIMARYLISTWIDGETPAGE_H
