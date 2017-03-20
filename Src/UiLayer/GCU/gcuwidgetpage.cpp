#include "gcuwidgetpage.h"
#include "ui_gcuwidgetpage.h"

GCUWidgetPage::GCUWidgetPage(BaseInfoList baseInfoList, int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCUWidgetPage)
{
    ui->setupUi(this);

    initTableWidgetUI();
    initTableWidget(baseInfoList,pageNum);
}

GCUWidgetPage::~GCUWidgetPage()
{
    delete ui;
}

void GCUWidgetPage::initTableWidgetUI()
{
    ui->tableWidget->setColumnWidth(0,288);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,288);
    ui->tableWidget->setColumnWidth(3,100);
}

void GCUWidgetPage::initTableWidget(BaseInfoList baseInfoList, int pageNum)
{
    QString dataCoordStr;
    BaseInfoClass baseInfo;

    for (int i = 0; i < baseInfoList.size(); i++)
    {
        baseInfo = baseInfoList.at(i);

        if (baseInfo.getPageNum() == pageNum)
        {
            //显示部分
            QTableWidgetItem *meaningItem = new QTableWidgetItem(tr("  %1").arg(baseInfo.getMeaning()));
            meaningItem->setTextAlignment(Qt::AlignLeft);
            meaningItem->setTextColor(QColor(254, 254, 0));
            ui->tableWidget->setItem(baseInfo.getYCoord(), baseInfo.getXCoord(),meaningItem);

            //实时数据部分
            dataCoordStr = QString::number(pageNum) + QString::number(baseInfo.getXCoord()+1) + QString::number(baseInfo.getYCoord());
            m_mapCoordAndTableWidgetItemPtr[dataCoordStr] = new QTableWidgetItem(tr(""));
            m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextAlignment(Qt::AlignCenter);
            m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextColor(QColor(254, 254, 0));
            ui->tableWidget->setItem(baseInfo.getYCoord(),baseInfo.getXCoord()+1,m_mapCoordAndTableWidgetItemPtr[dataCoordStr]);
        }
    }
}

void GCUWidgetPage::refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList)
{
    QString coordStr;
    QString dataCoordStr;

    for (int i = 0; i < realtimeDataInfoList.size(); i++)
    {
        coordStr = QString::number(realtimeDataInfoList.at(i).getPageNum()) +
                QString::number(realtimeDataInfoList.at(i).getXCoord()+1) +
                QString::number(realtimeDataInfoList.at(i).getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
        {
            dataCoordStr =QString::number(realtimeDataInfoList.at(i).getPageNum()) +
                    QString::number(realtimeDataInfoList.at(i).getXCoord()+1) +
                    QString::number(realtimeDataInfoList.at(i).getYCoord());

            switch (realtimeDataInfoList.at(i).getDataDisplayMethod())
            {
            case NO_DISPLAY:
                break;
            case ONLY_DISPLAY_IMAGE:
                break;
            case DISPLAY_NUMERICAL_VALUE_AND_COLOR:
                m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
                switch(realtimeDataInfoList.at(i).getColor())
                {
                case WHITE:
                    m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextColor(QColor(254, 254, 254));
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextColor(QColor(0, 254, 0));
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextColor(QColor(254, 0, 0));
                    break;
                case YELLOW:
                    m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setTextColor(QColor(254, 254, 0));
                    break;
                default:
                    break;
                }
                break;
            case ONLY_DISPLAY_COLOR:
                break;
            case ONLY_DISPLAY_DEVICE_STATE_DESC:
                break;
            case ONLY_DISPLAY_DECIMAL_NUMERICAL_VALUE:
                m_mapCoordAndTableWidgetItemPtr[dataCoordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
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
}

