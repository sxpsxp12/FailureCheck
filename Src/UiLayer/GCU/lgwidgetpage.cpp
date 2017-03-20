#include "lgwidgetpage.h"
#include "ui_lgwidgetpage.h"

LGWidgetPage::LGWidgetPage(BaseInfoList baseInfoList, int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LGWidgetPage)
{
    ui->setupUi(this);

    initTableWidgetUI();
    initTableWidget(baseInfoList,pageNum);
}

LGWidgetPage::~LGWidgetPage()
{
    delete ui;
}

void LGWidgetPage::initTableWidgetUI()
{
    ui->tableWidget->setColumnWidth(0,292);
    ui->tableWidget->setColumnWidth(1,482);
}

void LGWidgetPage::initTableWidget(BaseInfoList baseInfoList, int pageNum)
{
    QString coordStr;
    BaseInfoClass baseInfo;

    for (int i = 0; i < baseInfoList.size(); i++)
    {
        baseInfo = baseInfoList.at(i);

        if (baseInfo.getPageNum() == pageNum)
        {
            coordStr =QString::number(pageNum) + QString::number(baseInfo.getXCoord()+1) + QString::number(baseInfo.getYCoord());
            //显示部分
            QTableWidgetItem *meaningItem = new QTableWidgetItem(tr("  %1").arg(baseInfo.getMeaning()));
            meaningItem->setTextAlignment(Qt::AlignLeft);
            meaningItem->setTextColor(QColor(254, 254, 0));
            ui->tableWidget->setItem(baseInfo.getYCoord(), baseInfo.getXCoord(), meaningItem);

            //数据部分
            m_mapCoordAndTableWidgetItemPtr[coordStr] = new QTableWidgetItem(tr(""));
            m_mapCoordAndTableWidgetItemPtr[coordStr]-> setTextColor(QColor(254, 254, 0));
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(baseInfo.getYCoord(),baseInfo.getXCoord()+1, m_mapCoordAndTableWidgetItemPtr[coordStr]);
        }
    }
}

void LGWidgetPage::refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList)
{
    QString coordStr;

    for (int i = 0; i < realtimeDataInfoList.size(); i++)
    {
        coordStr = QString::number(realtimeDataInfoList.at(i).getPageNum()) +
                QString::number(realtimeDataInfoList.at(i).getXCoord()+1) +
                QString::number(realtimeDataInfoList.at(i).getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordStr] != NULL)
        {
            switch (realtimeDataInfoList.at(i).getDataDisplayMethod())
            {
            case NO_DISPLAY:
                break;
            case ONLY_DISPLAY_IMAGE:
                break;
            case DISPLAY_NUMERICAL_VALUE_AND_COLOR:          //主界面三个展示
                break;
            case ONLY_DISPLAY_COLOR:                      //LCU、接口灯孔
                break;
            case ONLY_DISPLAY_DEVICE_STATE_DESC:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getDeviceStateDesc());
                break;
            case ONLY_DISPLAY_DECIMAL_NUMERICAL_VALUE:
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setText(realtimeDataInfoList.at(i).getStrNumericalValue());
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

