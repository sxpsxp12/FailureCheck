#include "dcuwidgetpage.h"
#include "ui_dcuwidgetpage.h"

#include "enumtypeheader.h"

DCUWidgetPage::DCUWidgetPage(BaseInfoList baseInfoList, int pagNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DCUWidgetPage)
{
    ui->setupUi(this);

    initTableWidgetUI();
    initTableWidget(baseInfoList, pagNum);
}

DCUWidgetPage::~DCUWidgetPage()
{
    delete ui;
}

void DCUWidgetPage::initTableWidgetUI()
{
    ui->tableWidget->setColumnWidth(0,288);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,288);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->horizontalHeader()->setVisible(true);
}

void DCUWidgetPage::initTableWidget(BaseInfoList baseInfoList, int pageNum)
{
    QString coordStr;
    BaseInfoClass baseInfo;

    for (int i = 0; i < baseInfoList.size(); i++)
    {
        baseInfo = baseInfoList.at(i);

        if(baseInfo.getPageNum() == pageNum)
        {
            coordStr = QString::number(pageNum) + QString::number(baseInfo.getXCoord()) + QString::number(baseInfo.getYCoord());

            //基本界面显示信息
            QTableWidgetItem *item = new QTableWidgetItem(tr("  %1").arg(baseInfo.getMeaning()));
            item->setTextAlignment(Qt::AlignLeft);
            item->setTextColor(QColor(254, 254, 0));
            ui->tableWidget->setItem(baseInfo.getYCoord(), baseInfo.getXCoord(), item);

            //实时数据显示信息
            m_mapCoordAndTableWidgetItemPtr[coordStr] = new QTableWidgetItem(tr(""));
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextAlignment(Qt::AlignCenter);
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 254, 0));
            ui->tableWidget->setItem(baseInfo.getYCoord(), baseInfo.getXCoord()+1, m_mapCoordAndTableWidgetItemPtr[coordStr]);
        }
    }
}

void DCUWidgetPage::refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList)
{
    QString coordStr = NULL;

    for (int i = 0; i < realtimeDataInfoList.size(); i++)
    {
        coordStr = QString::number(realtimeDataInfoList.at(i).getPageNum()) +
                QString::number(realtimeDataInfoList.at(i).getXCoord()) +
                QString::number(realtimeDataInfoList.at(i).getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
        {
            switch (realtimeDataInfoList.at(i).getDataDisplayMethod())
            {
            case NO_DISPLAY:
                break;
            case ONLY_DISPLAY_IMAGE:
                break;
            case DISPLAY_NUMERICAL_VALUE_AND_COLOR:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
                switch(realtimeDataInfoList.at(i).getColor())
                {
                case WHITE:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 254, 254));
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(0, 254, 0));
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 0, 0));
                    break;
                case YELLOW:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextColor(QColor(254, 254, 0));
                    break;
                default:
                    break;
                }
                break;
            case ONLY_DISPLAY_COLOR:
                break;
            case ONLY_DISPLAY_DEVICE_STATE_DESC:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getDeviceStateDesc());
                break;
            case ONLY_DISPLAY_DECIMAL_NUMERICAL_VALUE:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
                break;
            case NOT_ANY_DATA_ONLY_MEANING:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText("---");
                break;
            case ONLY_DISPLAY_HEX_NUMERICAL_VALUE:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
                break;
            default:
                break;
            }
        }
    }
}
