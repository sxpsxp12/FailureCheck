#include "dcuwidget.h"
#include "ui_dcuwidget.h"

#include "databasesoperate.h"
#include "enumtypeheader.h"
#include "dcuwidgetpage.h"
#include "dataanalysis.h"

DCUWidget::DCUWidget(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DCUWidget)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    m_realtimeDataInfoList.clear();
    ui->stackedWidget->setCurrentIndex(0);

    m_totalPage = 0;
    m_currentPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList, DCU);
    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList2, DCU2);
    initWidget();

    connect(ui->pbt_pageUp, SIGNAL(clicked()), SLOT(slot_pbt_pageUpClicked()));
    connect(ui->pbt_pageDown, SIGNAL(clicked()), SLOT(slot_pbt_pageDownClicked()));
}

DCUWidget::~DCUWidget()
{
    delete ui;
}

void DCUWidget::initWidget()
{
    //遍历列表，找到总页数
    for (int i = 0; i < m_interfaceBaseInfoList.size(); i++)
    {
        if (m_totalPage < m_interfaceBaseInfoList.at(i).getPageNum())
        {
            m_totalPage = m_interfaceBaseInfoList.at(i).getPageNum();
        }
    }
    for (int i = 0; i < m_interfaceBaseInfoList2.size(); i++)
    {
        if (m_totalPage < m_interfaceBaseInfoList2.at(i).getPageNum())
        {
            m_totalPage = m_interfaceBaseInfoList2.at(i).getPageNum();
        }
    }

    //显示页数
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_totalPage));

    m_interfaceBaseInfoList.append(m_interfaceBaseInfoList2);        //微机柜一架和二架数据合并

    for (int i = 1; i <= m_totalPage; i++)
    {
        m_DCUWidgetPage = new DCUWidgetPage(m_interfaceBaseInfoList, i);
        m_mapIntAndDCUWidgetPagePtr[i-1] = m_DCUWidgetPage;
        ui->stackedWidget->addWidget(m_DCUWidgetPage);
    }
}

void DCUWidget::slot_pbt_pageUpClicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndDCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void DCUWidget::slot_pbt_pageDownClicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndDCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void DCUWidget::slot_receiveDcuWidgetData()
{
    m_realtimeDataInfoList.clear();
    m_realtimeDataInfoList = DataAnalysis::m_dcuWidgetRealtimeDataList;
    m_mapIntAndDCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList);
}
