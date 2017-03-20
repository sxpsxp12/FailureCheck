#include "secondarylistwidgetpage.h"
#include "ui_secondarylistwidgetpage.h"

#include "enumtypeheader.h"
#include "secondarywidget.h"

SecondaryListWidgetPage::SecondaryListWidgetPage(int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondaryListWidgetPage)
{
    ui->setupUi(this);

    m_pageNum = pageNum;
    m_createItemNum = 0;
}

SecondaryListWidgetPage::~SecondaryListWidgetPage()
{
    delete ui;
}

void SecondaryListWidgetPage::slot_receivePageData()
{
    QString coordStr;
    FailureInfoClass failureInfo;

    m_failureInfoList = SecondaryWidget::failureInfoList;

    for (int i = 0; i < SECONDARY_FAILURE_LIST_ROW_NUM; i++)                   //每次刷新数据前先清空旧数据
    {
        coordStr = QString::number(m_pageNum) + QString::number(i);
        if (m_mapCoordAndListWidgetItemPtr[coordStr] != NULL)
        {
            m_mapCoordAndListWidgetItemPtr[coordStr]->setText("");
            m_mapCoordAndListWidgetItemPtr[coordStr]->setHidden(true);
        }
        coordStr = QString::number(m_pageNum) + QString::number(i);
        if (m_mapCoordAndListWidgetItemPtr[coordStr] != NULL)
        {
            m_mapCoordAndListWidgetItemPtr[coordStr]->setText("");
            m_mapCoordAndListWidgetItemPtr[coordStr]->setHidden(true);
        }
        coordStr = QString::number(m_pageNum) + QString::number(i);
        if (m_mapCoordAndListWidgetItemPtr[coordStr] != NULL)
        {
            m_mapCoordAndListWidgetItemPtr[coordStr]->setText("");
            m_mapCoordAndListWidgetItemPtr[coordStr]->setHidden(true);
        }
    }

    int total = (m_failureInfoList.size()/SECONDARY_FAILURE_LIST_ROW_NUM) <= (m_pageNum-1) ?
                m_failureInfoList.size()%SECONDARY_FAILURE_LIST_ROW_NUM : SECONDARY_FAILURE_LIST_ROW_NUM;   //本页的数据总数
    int pageIncrement = (m_pageNum-1) * SECONDARY_FAILURE_LIST_ROW_NUM;                    //每页故障个数增量

    if (m_pageNum <= (m_failureInfoList.size()/SECONDARY_FAILURE_LIST_ROW_NUM+1))        //判断该页面是否有数据，有则进行下面的更新数据操作，否则跳过
    {
        for (int i = pageIncrement; i < total + pageIncrement; i++)                      //刷新本页数据
        {
            failureInfo = m_failureInfoList.at(i);

            if (i - pageIncrement < m_createItemNum)               //如果本页数据号小于已创建的ListWidgetItem项，则不用创建，直接用Map刷新数据
            {
                coordStr = QString::number(m_pageNum) + QString::number(i - pageIncrement);
                m_mapCoordAndListWidgetItemPtr[coordStr]->setHidden(false);
                m_mapCoordAndListWidgetItemPtr[coordStr]->setText(failureInfo.getFailureName());
            }
            else
            {
                m_createItemNum++;

                coordStr = QString::number(m_pageNum) + QString::number(i - pageIncrement);
                m_mapCoordAndListWidgetItemPtr[coordStr] = new QListWidgetItem(failureInfo.getFailureName());
                ui->ListWidget_OrderHint->addItem(m_mapCoordAndListWidgetItemPtr[coordStr]);
            }
        }
    }
}
