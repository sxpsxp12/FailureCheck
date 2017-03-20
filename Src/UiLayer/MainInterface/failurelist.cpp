#include "failurelist.h"
#include "ui_failurelist.h"

#include "enumtypeheader.h"
#include "databasesoperate.h"
#include <QDebug>
#include "primarywidget.h"
FailureProcessInfoList FailureList::m_failureProcessInfoList;

FailureList::FailureList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FailureList)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    m_totalPage = 0;
    m_currentPage = 0;
    m_createPage = 0;

    connect(ui->pbt_pageUp, SIGNAL(clicked()), SLOT(slot_pbt_pageUpClicked()));
    connect(ui->pbt_pageDown, SIGNAL(clicked()), SLOT(slot_pbt_pageDownClicked()));
}

FailureList::~FailureList()
{
    delete ui;
}

//处理传过来的ListItem
void FailureList::slot_setClickedListItem(int index)
{
    QString failureName;
    QString failureLv;

    m_outputBusId = PrimaryWidget::failureInfoList.at(index).getOutputBusId();
    failureName = PrimaryWidget::failureInfoList.at(index).getFailureName();

    switch (PrimaryWidget::failureInfoList.at(index).getFailureLv())
    {
    case SERIOUS:
        failureLv =  tr("严重");
        break;
    case MEDIUM:
        failureLv =  tr("中等");
        break;
    case SLIGHT:
        failureLv =  tr("轻微");
        break;
    case PROMPT:
        failureLv =  tr("提示");
        break;
    default:
        break;
    }

    ui->label_outputBusId->setText(tr("%1").arg(m_outputBusId));
    ui->label_failureName->setText(tr("%1").arg(failureName));
    ui->label_failureLv->setText(tr("级别：%1").arg(failureLv));

    DatabasesOperate::initFailureProcessInfoList(m_failureProcessInfoList, PrimaryWidget::failureInfoList.at(index).getFailureId());

    m_totalPage = (m_failureProcessInfoList.size()-1) / FAILURE_LIST_ROW_NUM + 1;

    for ( ; m_createPage < m_totalPage; ++m_createPage)
    {
         m_failureListPage = new FailureListPage(m_createPage+1);
         ui->stackedWidget->addWidget(m_failureListPage);
         connect(m_failureListPage, SIGNAL(signal_setGuiContainerCurrentWidget(int)),
                 this, SLOT(slot_setGuiContainerCurrentWidget(int)));
         connect(m_failureListPage, SIGNAL(signal_sendImageName(QString)),
                 this, SLOT(slot_receiveImageName(QString)));
         connect(this, SIGNAL(signal_sendPageData()),
                 m_failureListPage, SLOT(slot_receivePageData()));
    }

    emit signal_sendPageData();

    if (ui->stackedWidget->currentIndex() > m_totalPage-1)
    {
        ui->stackedWidget->setCurrentIndex(m_totalPage-1);
        m_currentPage = m_totalPage-1;
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_TotalPage->setText(tr("共%1页").arg(m_totalPage));
}

void FailureList::slot_setGuiContainerCurrentWidget(int currentWidget)
{
    emit signal_setGuiContainerCurrentWidget(currentWidget);
}

void FailureList::slot_receiveImageName(QString imageName)
{
    emit signal_sendImageName(imageName);
}

//梯形图
void FailureList::on_pbt_ladderDiagram_clicked()
{
    if(m_outputBusId != ".")
    {
        emit signal_sendOutputBusId(m_outputBusId);
        emit signal_setGuiContainerCurrentWidget(LADDER_DIAGRAM);
    }
}

void FailureList::slot_pbt_pageUpClicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void FailureList::slot_pbt_pageDownClicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}
