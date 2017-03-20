#ifndef FAILURELIST_H
#define FAILURELIST_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QMap>

#include "failureprocessclass.h"
#include "failureinfoclass.h"
#include "failurelistpage.h"

namespace Ui {
class FailureList;
}

class FailureList : public QWidget
{
    Q_OBJECT
    
public:
    explicit FailureList(QWidget *parent = 0);
    ~FailureList();
    static FailureProcessInfoList m_failureProcessInfoList;

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_sendPageData();
    void signal_sendImageName(QString);
    void signal_sendOutputBusId(QString);                   //发送输出线号到故障列表界面

private slots:
    void slot_setClickedListItem(int index);                //接收传过来的Item
    void slot_setGuiContainerCurrentWidget(int currentWidget);                 //接收从Page界面传来的切换界面的信号
    void slot_receiveImageName(QString imageName);                             //接收图片名称
    void on_pbt_ladderDiagram_clicked();
    void slot_pbt_pageUpClicked();
    void slot_pbt_pageDownClicked();

private:
    Ui::FailureList *ui;

    FailureListPage *m_failureListPage;

    int m_totalPage;       //总页数
    int m_currentPage;     //当前页数
    int m_createPage;      //创建页数

    QString m_outputBusId;      //输出线号
};

#endif // FAILURELIST_H
