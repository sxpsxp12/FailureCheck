#include "busidwidget.h"
#include "ui_busidwidget.h"

#include "busidwidgetpage.h"
#include "databasesoperate.h"
#include "enumtypeheader.h"
#include "dataanalysis.h"

BusIdWidget::BusIdWidget(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BusIdWidget)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    m_realtimeDataInfoList.clear();
    ui->stackedWidget->setCurrentIndex(0);

    m_totalPage = 0;
    m_currentPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList, BUS_ID_INTERFACE);
    initWidget();
}

BusIdWidget::~BusIdWidget()
{
    delete ui;

    if(m_busIdWidgetPage != NULL)
    {
        delete m_busIdWidgetPage;
    }
}

void BusIdWidget::initWidget()
{
    for (int i = 0; i < m_interfaceBaseInfoList.size(); i++)
    {
        if (m_totalPage < m_interfaceBaseInfoList.at(i).getPageNum())
        {
            m_totalPage = m_interfaceBaseInfoList.at(i).getPageNum();
        }
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_totalPage));

    for (int i = 1; i <= m_totalPage; i++)
    {
         m_busIdWidgetPage = new BusIdWidgetPage(m_interfaceBaseInfoList, i);
         m_mapIntAndBusIdWidgetPagePtr[i-1] = m_busIdWidgetPage;
         ui->stackedWidget->addWidget(m_busIdWidgetPage);
         connect(m_busIdWidgetPage, SIGNAL(signal_tableWidgetItemClicked(QTableWidgetItem*)),
                 this, SLOT(slot_tableWidgetItemClicked(QTableWidgetItem*)));
    }
}

//线号的槽
void BusIdWidget::on_pushButton_clicked()
{
    emit signal_setGuiContainerCurrentWidget(BUS_ID_INTERFACE);
}

//接口灯孔灯
void BusIdWidget::on_pushButton_2_clicked()
{
    emit signal_setGuiContainerCurrentWidget(INTERFACE_LAMP_HOLE);
    emit signal_setFocusToInterfaceLampHole();
}

void BusIdWidget::on_pbt_pageDown_clicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndBusIdWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void BusIdWidget::on_pbt_pageUp_clicked()
{
    if(m_currentPage > 0)
    {
        m_currentPage-=1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndBusIdWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void BusIdWidget::slot_receiveBusIdWidgetData()
{
    m_realtimeDataInfoList.clear();
    m_realtimeDataInfoList = DataAnalysis::m_busIdWidgetRealtimeDataList;

    m_mapIntAndBusIdWidgetPagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
}

void BusIdWidget::slot_tableWidgetItemClicked(QTableWidgetItem *tableWidgetItem)
{
    emit signal_tableWidgetItemClicked(tableWidgetItem);
    emit signal_setGuiContainerCurrentWidget(INTERFACE_LAMP_HOLE);
}

void BusIdWidget::slot_setFocus()
{
    ui->pushButton->setFocus();
}

