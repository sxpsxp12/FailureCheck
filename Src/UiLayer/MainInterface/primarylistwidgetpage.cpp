#include "primarylistwidgetpage.h"
#include "ui_primarylistwidgetpage.h"

#include "enumtypeheader.h"
#include <QDebug>
#include "primarywidget.h"
PrimaryListWidgetPage::PrimaryListWidgetPage(int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrimaryListWidgetPage)
{
    ui->setupUi(this);

    m_pageNum = pageNum;
    m_createItemNum = 0;

    connect(ui->ListWidget_ShowFaultInfoItem, SIGNAL(clicked(QModelIndex)),
            this, SLOT(slot_listWidgetItemClicked(QModelIndex)));
}

PrimaryListWidgetPage::~PrimaryListWidgetPage()
{
    delete ui;
}

void PrimaryListWidgetPage::slot_receivePageData()
{
    QString coordStr;
    FailureInfoClass failureInfo;

    for (int i = 0; i < PRIMARY_FAILURE_LIST_ROW_NUM; i++)                           //每次刷新数据前先清空旧数据
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

    int total = (PrimaryWidget::failureInfoList.size()/PRIMARY_FAILURE_LIST_ROW_NUM) <= (m_pageNum-1) ?
                PrimaryWidget::failureInfoList.size()%PRIMARY_FAILURE_LIST_ROW_NUM : PRIMARY_FAILURE_LIST_ROW_NUM;   //本页的数据总数
    int pageIncrement = (m_pageNum-1) * PRIMARY_FAILURE_LIST_ROW_NUM;                    //每页故障个数增量

    if (m_pageNum <= (PrimaryWidget::failureInfoList.size()/PRIMARY_FAILURE_LIST_ROW_NUM+1))        //判断该页面是否有数据，有则进行下面的更新数据操作，否则跳过
    {
        for (int i = pageIncrement; i < total + pageIncrement; i++)           //刷新本页数据
        {
            failureInfo = PrimaryWidget::failureInfoList.at(i);

            if (i - pageIncrement < m_createItemNum)                //如果本页数据号小于已创建的ListWidgetItem项，则不用创建，直接用Map刷新数据
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
                ui->ListWidget_ShowFaultInfoItem->addItem(m_mapCoordAndListWidgetItemPtr[coordStr]);
            }
        }
    }
}

void PrimaryListWidgetPage::slot_listWidgetItemClicked(QModelIndex modelIndex)
{
    int rowNum;
    int serialNumber;

    rowNum = modelIndex.row();
    serialNumber = (m_pageNum-1) * PRIMARY_FAILURE_LIST_ROW_NUM + rowNum;      //获取列表中的数据序号

    if (serialNumber < PrimaryWidget::failureInfoList.size())               //判断点击的列表项是否在当前列表范围内
    {
        //failureInfo = PrimaryWidget::failureInfoList.at();
        emit signal_sendListWidgetItemClickedData(serialNumber);
    }
}
