#include "interfacelampholepage.h"
#include "ui_interfacelampholepage.h"

#include "enumtypeheader.h"
#include "databasesoperate.h"

InterfaceLampHolePage::InterfaceLampHolePage(BaseInfoList baseInfoList, int pageNum, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterfaceLampHolePage)
{
    ui->setupUi(this);

    ui->tableWidget_InterfaceLampHoleleft->setEnabled(false);

    initTableWidgetUI();
    initTableWidget(baseInfoList, pageNum);
}

InterfaceLampHolePage::~InterfaceLampHolePage()
{
    delete ui;
}

void InterfaceLampHolePage::initTableWidgetUI()
{
    //左侧的列表
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(0,40);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(1,60);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(2,60);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(3,230);

    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(4,40);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(5,60);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(6,60);
    ui->tableWidget_InterfaceLampHoleleft->setColumnWidth(7,230);
    ui->tableWidget_InterfaceLampHoleleft->horizontalHeader()->setMinimumHeight(39);
    ui->tableWidget_InterfaceLampHoleleft->horizontalHeader()->setEnabled(false);
}

void InterfaceLampHolePage::initTableWidget(BaseInfoList baseInfoList, int pageNum)
{
    QString coordStr;
    BaseInfoClass baseInfo;

    QTableWidgetItem *busIdItem;
    QTableWidgetItem *interfacesItem;
    QTableWidgetItem *indicatorLightItem;
    QTableWidgetItem *meaningItem;

    for (int i = 0;i < baseInfoList.size();i++)
    {
        baseInfo = baseInfoList.at(i);

        if (baseInfo.getPageNum() == pageNum)
        {
            if (baseInfo.getBusId().left(2) != "BY")
            {
                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()) + "|" + QString::number(baseInfo.getYCoord());
                busIdItem = new QTableWidgetItem(tr("%1").arg(baseInfo.getBusId()));
                m_mapCoordAndTableWidgetItemPtr[coordStr] = busIdItem;
                \
                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+1) + "|" + QString::number(baseInfo.getYCoord());
                interfacesItem = new QTableWidgetItem(tr("%1").arg(baseInfo.getInterfaces()));
                m_mapCoordAndTableWidgetItemPtr[coordStr] = interfacesItem;

                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+2) + "|" + QString::number(baseInfo.getYCoord());
                indicatorLightItem = new QTableWidgetItem(tr("%1").arg(baseInfo.getIndicatorLight()));
                m_mapCoordAndTableWidgetItemPtr[coordStr] = indicatorLightItem;

                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+3) + "|" + QString::number(baseInfo.getYCoord());
                meaningItem = new QTableWidgetItem(tr("%1").arg(baseInfo.getMeaning()));
                m_mapCoordAndTableWidgetItemPtr[coordStr] = meaningItem;
            }
            else
            {
                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()) + "|" + QString::number(baseInfo.getYCoord());
                busIdItem = new QTableWidgetItem("---");
                m_mapCoordAndTableWidgetItemPtr[coordStr] = busIdItem;

                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+1) + "|" + QString::number(baseInfo.getYCoord());
                interfacesItem = new QTableWidgetItem();
                m_mapCoordAndTableWidgetItemPtr[coordStr] = interfacesItem;

                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+2) + "|" + QString::number(baseInfo.getYCoord());
                indicatorLightItem = new QTableWidgetItem();
                m_mapCoordAndTableWidgetItemPtr[coordStr] = indicatorLightItem;

                coordStr = QString::number(pageNum) + "|" + QString::number(baseInfo.getXCoord()+3) + "|" + QString::number(baseInfo.getYCoord());
                meaningItem = new QTableWidgetItem();
                m_mapCoordAndTableWidgetItemPtr[coordStr] = meaningItem;
            }

            if(baseInfo.getXCoord() == INTERFACE_1_X_COORD)
            {
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 0, busIdItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 1, interfacesItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 2, indicatorLightItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 3, meaningItem);
            }
            if(baseInfo.getXCoord() == INTERFACE_2_X_COORD)
            {
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 4, busIdItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 5, interfacesItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 6, indicatorLightItem);
                ui->tableWidget_InterfaceLampHoleleft->setItem(baseInfo.getYCoord(), 7, meaningItem);
            }
        }
    }
}

void InterfaceLampHolePage::refreshWidget(const RealtimeDataInfoList &realtimeDataInfoList, int curPageNum)
{
    QString coordBusIdStr;
    QString coordInterfacesStr;
    QString coordIndicatorLightStr;
    QString coordMeaningStr;

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

        coordBusIdStr = QString::number(realtimeDataInfo.getPageNum()) + "|" +
                QString::number(realtimeDataInfo.getXCoord()) + "|" +
                QString::number(realtimeDataInfo.getYCoord());

        if (m_mapCoordAndTableWidgetItemPtr[coordBusIdStr] != NULL)
        {
            coordInterfacesStr = QString::number(realtimeDataInfo.getPageNum()) + "|" +
                    QString::number(realtimeDataInfo.getXCoord()+1) + "|" +
                    QString::number(realtimeDataInfo.getYCoord());
            coordIndicatorLightStr = QString::number(realtimeDataInfo.getPageNum()) + "|" +
                    QString::number(realtimeDataInfo.getXCoord()+2) + "|" +
                    QString::number(realtimeDataInfo.getYCoord());
            coordMeaningStr = QString::number(realtimeDataInfo.getPageNum()) + "|" +
                    QString::number(realtimeDataInfo.getXCoord()+3) + "|" +
                    QString::number(realtimeDataInfo.getYCoord());

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
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setTextColor(QColor(254, 254, 254));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setTextColor(QColor(254, 254, 254));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setTextColor(QColor(254, 254, 254));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setTextColor(QColor(254, 254, 254));
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setBackgroundColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setBackgroundColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setBackgroundColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setBackgroundColor(QColor(0, 0, 0));
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setBackgroundColor(QColor(0, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setBackgroundColor(QColor(0, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setBackgroundColor(QColor(0, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setBackgroundColor(QColor(0, 254, 0));
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setBackgroundColor(QColor(254, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setBackgroundColor(QColor(254, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setBackgroundColor(QColor(254, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setBackgroundColor(QColor(254, 0, 0));
                    break;
                case YELLOW:
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setTextColor(QColor(0, 0, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setBackgroundColor(QColor(254, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setBackgroundColor(QColor(254, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setBackgroundColor(QColor(254, 254, 0));
                    m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setBackgroundColor(QColor(254, 254, 0));
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

void InterfaceLampHolePage::slot_searchId(int pageNum, int xCoord, int yCoord)
{
    QString coordBusIdStr = QString::number(pageNum) + "|" + QString::number(xCoord) + "|" + QString::number(yCoord);

    if(m_mapCoordAndTableWidgetItemPtr[m_preCoordBusIdStr] != NULL)
    {
        m_mapCoordAndTableWidgetItemPtr[m_preCoordBusIdStr]->setSelected(false);
        m_mapCoordAndTableWidgetItemPtr[m_preCoordInterfacesStr]->setSelected(false);
        m_mapCoordAndTableWidgetItemPtr[m_preCoordIndicatorLightStr]->setSelected(false);
        m_mapCoordAndTableWidgetItemPtr[m_preCoordMeaningStr]->setSelected(false);
    }

    if(m_mapCoordAndTableWidgetItemPtr[coordBusIdStr] != NULL)
    {
        QString coordInterfacesStr = QString::number(pageNum) + "|" + QString::number(xCoord+1) + "|" + QString::number(yCoord);
        QString coordIndicatorLightStr = QString::number(pageNum) + "|" + QString::number(xCoord+2) + "|" + QString::number(yCoord);
        QString coordMeaningStr = QString::number(pageNum) + "|" + QString::number(xCoord+3) + "|" + QString::number(yCoord);
        m_preCoordBusIdStr = coordBusIdStr;
        m_preCoordInterfacesStr = coordInterfacesStr;
        m_preCoordIndicatorLightStr = coordIndicatorLightStr;
        m_preCoordMeaningStr = coordMeaningStr;
        m_mapCoordAndTableWidgetItemPtr[coordBusIdStr]->setSelected(true);
        m_mapCoordAndTableWidgetItemPtr[coordInterfacesStr]->setSelected(true);
        m_mapCoordAndTableWidgetItemPtr[coordIndicatorLightStr]->setSelected(true);
        m_mapCoordAndTableWidgetItemPtr[coordMeaningStr]->setSelected(true);
    }
}
