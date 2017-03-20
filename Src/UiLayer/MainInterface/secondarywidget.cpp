#include "secondarywidget.h"
#include "ui_secondarywidget.h"

#include "databasesoperate.h"
#include "enumtypeheader.h"
#include <QListWidgetItem>
#include <QDebug>
#include "dataanalysis.h"
FailureInfoList SecondaryWidget::failureInfoList;

SecondaryWidget::SecondaryWidget(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondaryWidget)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;

    ui->stackedWidget->setCurrentIndex(0);           //设置堆栈Widget的当前页面

    m_totalPage = 0;
    m_currentPage = 0;
    m_createPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList, SECONDARY_INTERFACE);
    initTableWidget();         //初始化tableWidget
    initWidget();

    connect(ui->pbt_pageUp, SIGNAL(clicked()), this, SLOT(slot_pbt_pageUpClicked()));
    connect(ui->pbt_pageDown, SIGNAL(clicked()), this, SLOT(slot_pbt_pageDownClicked()));
}

SecondaryWidget::~SecondaryWidget()
{
    delete ui;

    if(m_secondaryListWidgetPage != NULL)
    {
        delete m_secondaryListWidgetPage;
    }
}

//初始化tableWidget
void SecondaryWidget::initTableWidget()
{
    QString coordStr;
    QStringList meaningStrList;
    BaseInfoClass baseInfo;

    for (int i = 0 ; i < m_interfaceBaseInfoList.size(); i++)
    {
        baseInfo = m_interfaceBaseInfoList.at(i);

        coordStr = QString::number(baseInfo.getPageNum()) + QString::number(baseInfo.getXCoord()) + QString::number(baseInfo.getYCoord());
        meaningStrList = baseInfo.getMeaning().split("|", QString::SkipEmptyParts);

        m_mapCoordAndmachineType[coordStr] = meaningStrList.at(1).toInt();
        m_mapCoordAndTableWidgetItemPtr[coordStr] = new SecondWidgetTableItemModelClass;
        m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineSymbol(meaningStrList.at(2));
        m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineName(meaningStrList.at(0));
        m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineStatus(m_mapCoordAndmachineType[coordStr], NO_WORK);
        ui->tableWidget_containter->setCellWidget(baseInfo.getYCoord(), baseInfo.getXCoord(), m_mapCoordAndTableWidgetItemPtr[coordStr]);
    }
}

void SecondaryWidget::initWidget()
{
    ui->Label_OrderNum->setText(tr("提示数:%1").arg(0));
    ui->label_Totalpage->setText(tr("共%1页").arg(1));
    ui->label_pageNum->setText(tr("第%1页").arg(1));
}

//返回主界面
void SecondaryWidget::on_pushButton_clicked()
{
    emit signal_setGuiContainerCurrentWidget(PRIMARY_INTERFACE);
}

void SecondaryWidget::slot_pbt_pageUpClicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void SecondaryWidget::slot_pbt_pageDownClicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void SecondaryWidget::slot_receiveSecondaryWidgetData()
{
    QString coordStr;
    RealtimeDataClass realtimeDataInfo;

    m_currentRealtimeDataInfoList.clear();
    m_currentRealtimeDataInfoList = DataAnalysis::m_secondaryWidgetRealtimeDataList;
    failureInfoList = DataAnalysis::m_secondaryWidgetFailureInfoList;

    for (int i = 0; i < m_currentRealtimeDataInfoList.size(); i++)
    {
        realtimeDataInfo = m_currentRealtimeDataInfoList.at(i);

        if (!m_historyRealtimeDataInfoList.isEmpty())
        {
            if (m_historyRealtimeDataInfoList.at(m_historyRealtimeDataInfoList.indexOf(realtimeDataInfo)).getColor() == realtimeDataInfo.getColor())
            {
                continue;
            }
        }

        coordStr = QString::number(realtimeDataInfo.getPageNum()) +
                QString::number(realtimeDataInfo.getXCoord()) +
                QString::number(realtimeDataInfo.getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
        {
            switch (realtimeDataInfo.getDataDisplayMethod())
            {
            case NO_DISPLAY:
                break;
            case ONLY_DISPLAY_IMAGE:
                switch (realtimeDataInfo.getColor())
                {
                case WHITE:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineStatus(m_mapCoordAndmachineType[coordStr], NO_WORK);
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineStatus(m_mapCoordAndmachineType[coordStr], NORMAL_WORK);
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setMachineStatus(m_mapCoordAndmachineType[coordStr], ABNORMAL_WORK);
                    break;
                case YELLOW:
                    break;
                default:
                    break;
                }
                break;
            case DISPLAY_NUMERICAL_VALUE_AND_COLOR:
                break;
            case ONLY_DISPLAY_COLOR:
                break;
            case ONLY_DISPLAY_DEVICE_STATE_DESC:
                break;
            case ONLY_DISPLAY_DECIMAL_NUMERICAL_VALUE:
                break;
            case NOT_ANY_DATA_ONLY_MEANING:
                break;
            case ONLY_DISPLAY_HEX_NUMERICAL_VALUE:
                break;
            default:
                break;
            }
        }
    }

    m_historyRealtimeDataInfoList.clear();
    m_historyRealtimeDataInfoList = m_currentRealtimeDataInfoList;


    m_totalPage = (failureInfoList.size()-1) / PRIMARY_FAILURE_LIST_ROW_NUM + 1;

    ui->Label_OrderNum->setText(tr("提示数：%1").arg(failureInfoList.size()));

    for ( ; m_createPage < m_totalPage; m_createPage++)                     //创建页数小于总页数，则创建新页
    {
         m_secondaryListWidgetPage = new SecondaryListWidgetPage(m_createPage+1);
         ui->stackedWidget->addWidget(m_secondaryListWidgetPage);
         connect(this, SIGNAL(signal_sendPageData()),
                 m_secondaryListWidgetPage, SLOT(slot_receivePageData()));
    }

    emit signal_sendPageData();

    if (ui->stackedWidget->currentIndex() > m_totalPage-1)                 //更新数据后，判断所在页面是否超出总页数
    {
        ui->stackedWidget->setCurrentIndex(m_totalPage-1);                 //如果超出，则切换到最后一页
        m_currentPage = m_totalPage-1;
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_Totalpage->setText(tr("共%1页").arg(m_totalPage));
}
