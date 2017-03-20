#ifndef FAILURELISTPAGE_H
#define FAILURELISTPAGE_H

#include <QWidget>
#include <QMap>

#include "failureprocessclass.h"

class QTableWidgetItem;

namespace Ui {
class FailureListPage;
}

class FailureListPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit FailureListPage(int pageNum, QWidget *parent = 0);
    ~FailureListPage();

    void initTableWidgetUI();     //初始化TableWidget界面部分

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_sendImageName(QString);

private slots:
    void slot_setClickedTableWidgetLastItem(int row, int column);   //接收tablewidget的最后一行的item点击信号
    void slot_receivePageData();
    
private:
    Ui::FailureListPage *ui;

    int m_pageNum;
    int m_createItemNum;                    //QTableWidget已创建的行数
    int m_currentItemNum;                   //当前存在的QTableWidgetItem数量
    FailureProcessInfoList failureProcessInfoList;
    QMap<QString, QString> m_mapRowNumberAndimageName;
    QMap<QString, QTableWidgetItem*> m_mapCoordAndTableWidgetItemPtr;
};

#endif // FAILURELISTPAGE_H
