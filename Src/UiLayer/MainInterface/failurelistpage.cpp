#include "failurelistpage.h"
#include "ui_failurelistpage.h"

#include <QTableWidgetItem>

#include "enumtypeheader.h"
#include <QDebug>
#include "failurelist.h"

FailureListPage::FailureListPage(int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FailureListPage)
{
    ui->setupUi(this);

    m_pageNum = pageNum;
    m_createItemNum = 0;

    initTableWidgetUI();
}

FailureListPage::~FailureListPage()
{
    delete ui;
}

void FailureListPage::initTableWidgetUI()
{
    ui->tableWidget_failureList->setColumnWidth(0,93);
    ui->tableWidget_failureList->setColumnWidth(1,634);
    ui->tableWidget_failureList->setColumnWidth(2,37);

    ui->tableWidget_failureList->horizontalHeader()->setMinimumHeight(30);
    ui->tableWidget_failureList->verticalHeader()->setMinimumWidth(25);
    ui->tableWidget_failureList->horizontalHeader()->setEnabled(false);
    ui->tableWidget_failureList->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_failureList->verticalHeader()->setEnabled(false);

    connect(ui->tableWidget_failureList,SIGNAL(cellClicked(int,int)),this,SLOT(slot_setClickedTableWidgetLastItem(int,int)));
}

//接收tablewidget的最后一行的item点击
void FailureListPage::slot_setClickedTableWidgetLastItem(int row, int column)
{
    QString coordStr;
    coordStr = QString::number(m_pageNum) + QString::number(row) + QString::number(2);

    if(column == 2)
    {
        if (m_mapRowNumberAndimageName[coordStr] != NULL)
        {
            emit signal_sendImageName(m_mapRowNumberAndimageName[coordStr]);
            emit signal_setGuiContainerCurrentWidget(FAILURE_DETAILS);
        }
    }
}

void FailureListPage::slot_receivePageData()
{
    FailureProcessClass failureProcessInfo;
    QString coordStr;
    failureProcessInfoList.clear();
    failureProcessInfoList = FailureList::m_failureProcessInfoList;

    for (int i = 0; i < FAILURE_LIST_ROW_NUM; i++)
    {
        coordStr = QString::number(m_pageNum) + QString::number(i) + QString::number(0);
        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setText("");
        coordStr = QString::number(m_pageNum) + QString::number(i) + QString::number(1);
        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setText("");
        coordStr = QString::number(m_pageNum) + QString::number(i) + QString::number(2);
        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setText("");
        if (m_mapRowNumberAndimageName[coordStr] != NULL)
            m_mapRowNumberAndimageName[coordStr].clear();
    }

    int total = (failureProcessInfoList.size()/FAILURE_LIST_ROW_NUM) <= (m_pageNum-1) ?
                failureProcessInfoList.size()%FAILURE_LIST_ROW_NUM : FAILURE_LIST_ROW_NUM;
    int pageIncrement = (m_pageNum-1) * FAILURE_LIST_ROW_NUM;                   //每页故障处理方法个数增量

    if (m_pageNum <= (failureProcessInfoList.size()/FAILURE_LIST_ROW_NUM+1))        //判断该页面是否有数据，有则进行下面的更新数据操作，否则跳过
    {
        for (int i = pageIncrement; i < total + pageIncrement; i++)
        {
            failureProcessInfo = failureProcessInfoList.at(i);

            if (i - pageIncrement < m_createItemNum)
            {
                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(0);
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(failureProcessInfo.getProcessPoint());

                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(1);
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(failureProcessInfo.getProcessMethod());

                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(2);
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(tr("详情"));
                m_mapRowNumberAndimageName[coordStr] = failureProcessInfo.getImageName();
            }
            else
            {
                m_createItemNum++;

                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(0);
                QTableWidgetItem *processPoint = new QTableWidgetItem(failureProcessInfo.getProcessPoint());
                processPoint->setFlags(Qt::NoItemFlags);
                ui->tableWidget_failureList->setItem(i-pageIncrement, 0, processPoint);
                m_mapCoordAndTableWidgetItemPtr[coordStr] = processPoint;

                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(1);
                QTableWidgetItem *processMethod = new QTableWidgetItem(failureProcessInfo.getProcessMethod());
                processMethod->setFlags(Qt::NoItemFlags);
                ui->tableWidget_failureList->setItem(i-pageIncrement, 1, processMethod);
                m_mapCoordAndTableWidgetItemPtr[coordStr] = processMethod;

                coordStr = QString::number(m_pageNum) + QString::number(i-pageIncrement) + QString::number(2);
                QTableWidgetItem *failureDetail = new QTableWidgetItem(tr("详情"));
                failureDetail->setFlags(Qt::NoItemFlags);
                ui->tableWidget_failureList->setItem(i-pageIncrement, 2, failureDetail);
                m_mapCoordAndTableWidgetItemPtr[coordStr] = failureDetail;
                m_mapRowNumberAndimageName[coordStr] = failureProcessInfo.getImageName();
            }
        }
    }
}
