#include "gcuwidget.h"
#include "ui_gcuwidget.h"

#include "databasesoperate.h"
#include "dataanalysis.h"

GCUWidget::GCUWidget(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCUWidget)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    ui->stackedWidget->setCurrentIndex(0);
    m_realTimeDataInfoList.clear();
    m_totalPage = 0;
    m_currentPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_gcuBaseInfoList, GCU);
    initTableWidget();
}

GCUWidget::~GCUWidget()
{
    delete ui;
}

void GCUWidget::initTableWidget()
{
    for (int i = 0; i < m_gcuBaseInfoList.size(); i++)
    {
        if (m_totalPage < m_gcuBaseInfoList.at(i).getPageNum())
        {
            m_totalPage = m_gcuBaseInfoList.at(i).getPageNum();
        }
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_totalPage));

    for (int i = 1; i <= m_totalPage; i++)
    {
        m_gcuWidgetPage = new GCUWidgetPage(m_gcuBaseInfoList, i);
        m_mapIntAndGCUWidgetPagePtr[i-1] = m_gcuWidgetPage;
        ui->stackedWidget->addWidget(m_gcuWidgetPage);
    }
}

void GCUWidget::on_pushButton_clicked()
{
    emit signal_setGuiContainerCurrentWidget(GCU);
}

void GCUWidget::on_pushButton_2_clicked()
{
    emit signal_setGuiContainerCurrentWidget(LG);
    emit signal_setFocusToLGWidget();
}

void GCUWidget::slot_receiveGcuWidgetData()
{
    m_realTimeDataInfoList.clear();
    m_realTimeDataInfoList = DataAnalysis::m_gcuWidgetRealtimeDataList;

    m_mapIntAndGCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
}

void GCUWidget::slot_setFocus()
{
    ui->pushButton->setFocus();
}

void GCUWidget::on_pbt_pageUp_clicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndGCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void GCUWidget::on_pbt_pageDown_clicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndGCUWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}
