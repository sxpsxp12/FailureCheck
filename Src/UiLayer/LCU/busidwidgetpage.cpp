#include "busidwidgetpage.h"
#include "ui_busidwidgetpage.h"

#include "enumtypeheader.h"

BusIdWidgetPage::BusIdWidgetPage(BaseInfoList baseInfoList, int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BusIdWidgetPage)
{
    ui->setupUi(this);

    initTableWidget(baseInfoList, pageNum);

    connect(ui->tableWidget_busID, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(slot_tableWidgetItemClicked(QTableWidgetItem*)));
}

BusIdWidgetPage::~BusIdWidgetPage()
{
    delete ui;
}

void BusIdWidgetPage::initTableWidget(BaseInfoList baseInfoList, int pageNum)
{
    QString coordStr;
    BaseInfoClass baseInfo;

    for (int i = 0; i < baseInfoList.size(); i++)
    {
        baseInfo = baseInfoList.at(i);

        if (baseInfo.getPageNum() == pageNum)
        {
            coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()) + "|" + QString::number(baseInfo.getYCoord());

            m_mapCoordAndTableWidgetItemPtr[coordStr] = new QTableWidgetItem(tr("%1").arg(baseInfo.getMeaning()));
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextAlignment(Qt::AlignCenter);
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 254, 254));
            ui->tableWidget_busID->setItem(baseInfo.getYCoord(), baseInfo.getXCoord(), m_mapCoordAndTableWidgetItemPtr[coordStr]);
        }
    }
}

void BusIdWidgetPage::refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList, int curPageNum)
{
    QString coordStr;
    RealtimeDataClass realtimeDataInfo;

    m_currentRealtimeDataInfoList.clear();
    for (int i = 0; i < realtimeDataInfoList.size(); i++)
    {
        if (realtimeDataInfoList.at(i).getPageNum()-1 == curPageNum)
        {
            m_currentRealtimeDataInfoList.append(realtimeDataInfoList.at(i));
        }
    }

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

        coordStr = QString::number(realtimeDataInfo.getPageNum()) + "|" +
                QString::number(realtimeDataInfo.getXCoord()) + "|" +
                QString::number(realtimeDataInfo.getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
        {
            switch (realtimeDataInfo.getDataDisplayMethod())
            {
            case NO_DISPLAY:
                break;
            case ONLY_DISPLAY_IMAGE:
                break;
            case DISPLAY_NUMERICAL_VALUE_AND_COLOR:
                break;
            case ONLY_DISPLAY_COLOR:
                switch(realtimeDataInfo.getColor())
                {
                case WHITE:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 254, 254));
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setBackgroundColor(QColor(0, 0, 0));
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setBackgroundColor(QColor(0, 254, 0));
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setBackgroundColor(QColor(254, 0, 0));
                    break;
                case YELLOW:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setBackgroundColor(QColor(254, 254, 0));
                    break;
                default:
                    break;
                }
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
}

void BusIdWidgetPage::slot_tableWidgetItemClicked(QTableWidgetItem * tableWidgetItem)
{
    emit signal_tableWidgetItemClicked(tableWidgetItem);
    tableWidgetItem->setSelected(false);
}
