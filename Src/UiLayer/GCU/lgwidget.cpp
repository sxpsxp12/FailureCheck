#include "lgwidget.h"
#include "ui_lgwidget.h"

#include "databasesoperate.h"
#include "dataanalysis.h"

LGWidget::LGWidget(RequestDataThread *requestDataThread,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LGWidget)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    ui->stackedWidget->setCurrentIndex(0);
    m_realTimeDataInfoList.clear();
    m_totalPage = 0;
    m_currentPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_lgBaseInfoList, LG);
    Inittable();
}

LGWidget::~LGWidget()
{
    delete ui;
}

void LGWidget::Inittable()
{
    for (int i = 0; i < m_lgBaseInfoList.size(); i++)
    {
        if (m_totalPage < m_lgBaseInfoList.at(i).getPageNum())
        {
            m_totalPage = m_lgBaseInfoList.at(i).getPageNum();
        }
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_totalPage));

    for (int i = 1; i <= m_totalPage; i++)
    {
        m_lgWidgetPage = new LGWidgetPage(m_lgBaseInfoList,i);
        m_mapIntAndLGWidgetPagePtr[i-1] = m_lgWidgetPage;
        ui->stackedWidget->addWidget(m_lgWidgetPage);
    }
}


void LGWidget::slot_receiveLgWidgetData()
{
    m_realTimeDataInfoList.clear();
    m_realTimeDataInfoList =  DataAnalysis::m_lgWidgetRealtimeDataList;

    m_mapIntAndLGWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
}

void LGWidget::slot_setFocus()
{
    ui->pushButton_2->setFocus();
}

void LGWidget::on_pushButton_clicked()
{
    emit signal_setGuiContainerCurrentWidget(GCU);
    emit signal_setFocusToGCUWidget();
}

void LGWidget::on_pushButton_2_clicked()
{
    emit signal_setGuiContainerCurrentWidget(LG);
}

void LGWidget::on_pbt_pageUp_clicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndLGWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void LGWidget::on_pbt_pageDown_clicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndLGWidgetPagePtr[m_currentPage]->refreshWidget(m_realTimeDataInfoList);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}
