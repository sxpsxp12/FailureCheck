#include "ladderdiagrampage.h"
#include "ui_ladderdiagrampage.h"
#include <QDebug>

LadderDiagramPage::LadderDiagramPage(int pageNum, LadderDiagramInfoList ladderDiagramInputInfoList,
                                     LadderDiagramInfoList ladderDiagramOutputInfoList, QWidget *parent):
    QWidget(parent),
    ui(new Ui::LadderDiagramPage)
{
    ui->setupUi(this);

    initTableWidget(pageNum, ladderDiagramInputInfoList, ladderDiagramOutputInfoList);
}

LadderDiagramPage::~LadderDiagramPage()
{
    delete ui;
}

void LadderDiagramPage::initTableWidget(int pageNum, LadderDiagramInfoList ladderDiagramInputInfoList, LadderDiagramInfoList ladderDiagramOutputInfoList)
{
    QString coordStr;
    LadderDiagramClass ladderDiagramInfo;

    for ( int i = 0; i < ladderDiagramInputInfoList.size(); i++ )
    {
        ladderDiagramInfo = ladderDiagramInputInfoList.at(i);
        coordStr = ladderDiagramInfo.getPageNumStr() + ladderDiagramInfo.getXCoordStr() + ladderDiagramInfo.getYCoordStr();
        InputPartItem *inputPartItem = new InputPartItem(ladderDiagramInfo, DISCONNECT);
        m_mapCoordAndInputTableWidgetItemPtr[coordStr] = inputPartItem;
        ui->tableWidget_input->setCellWidget(ladderDiagramInfo.getYCoordInt(),
                                             ladderDiagramInfo.getXCoordInt(),
                                             inputPartItem);
    }

    for (int i = 0; i < ladderDiagramOutputInfoList.size(); i++)
    {
        LadderDiagramClass ladderDiagramInfo = ladderDiagramOutputInfoList.at(i);
        coordStr = ladderDiagramInfo.getPageNumStr() + ladderDiagramInfo.getXCoordStr() + ladderDiagramInfo.getYCoordStr();
        OutputPartItem *outputPartItem = new OutputPartItem(ladderDiagramInfo, DISCONNECT);
        m_mapCoordAndOutputTableWidgetItemPtr[coordStr] = outputPartItem;
        ui->tableWidget_output->setCellWidget(ladderDiagramInfo.getYCoordInt(),
                                              ladderDiagramInfo.getXCoordInt(),
                                              outputPartItem);
    }
}

void LadderDiagramPage::refreshWidget(const LadderDiagramInfoList &realtimeDataList, int curPageNum)
{
    QString coordStr;
    LadderDiagramClass ladderDiagramInfo;

    m_currentLadderDiagramInfoList.clear();
    for (int i = 0; i < realtimeDataList.size(); i++)
    {
        ladderDiagramInfo = realtimeDataList.at(i);

        if (ladderDiagramInfo.getPageNumInt() == curPageNum)
        {
   //         qDebug() << "page: " << ladderDiagramInfo.getPageNumInt() << " , " << curPageNum;
            m_currentLadderDiagramInfoList.append(ladderDiagramInfo);
        }
    }

    for (int i = 0; i < m_currentLadderDiagramInfoList.size(); i++)
    {
        ladderDiagramInfo = m_currentLadderDiagramInfoList.at(i);

        if (!m_historyLadderDiagramInfoList.isEmpty())
        {
            if (m_historyLadderDiagramInfoList.at(m_historyLadderDiagramInfoList.indexOf(ladderDiagramInfo)).getColor() == ladderDiagramInfo.getColor())
            {
   //             qDebug() << "NoChange-his: " << m_historyLadderDiagramInfoList.at(m_historyLadderDiagramInfoList.indexOf(ladderDiagramInfo)).getBusId() << " : " <<
   //                         m_historyLadderDiagramInfoList.at(m_historyLadderDiagramInfoList.indexOf(ladderDiagramInfo)).getColor();
   //             qDebug() << "NoChange-cur: " << ladderDiagramInfo.getBusId() << " : " << ladderDiagramInfo.getColor();
                continue;
            }
        }

   //     qDebug() << "IsChange-his: " << m_historyLadderDiagramInfoList.at(m_historyLadderDiagramInfoList.indexOf(ladderDiagramInfo)).getBusId() << " : " <<
   //                 m_historyLadderDiagramInfoList.at(m_historyLadderDiagramInfoList.indexOf(ladderDiagramInfo)).getColor();
   //     qDebug() << "IsChange-cur: " << ladderDiagramInfo.getBusId() << " : " << ladderDiagramInfo.getColor();

        coordStr = ladderDiagramInfo.getPageNumStr() + ladderDiagramInfo.getXCoordStr() + ladderDiagramInfo.getYCoordStr();

        if ( ladderDiagramInfo.getPartTypeInt() == REPEATER || ladderDiagramInfo.getPartTypeInt() == OUTPUT )
        {
            switch (ladderDiagramInfo.getColor())
            {
            case WHITE:
                if (m_mapCoordAndOutputTableWidgetItemPtr[coordStr] != NULL)
                {
                    m_mapCoordAndOutputTableWidgetItemPtr[coordStr]->setDataUpdateColor(WHITE);
                }
                break;
            case GREEN:
                if (m_mapCoordAndOutputTableWidgetItemPtr[coordStr] != NULL)
                {
                    m_mapCoordAndOutputTableWidgetItemPtr[coordStr]->setDataUpdateColor(GREEN);
                }
                break;
            default:
                break;
            }
        }else
        {
            switch (ladderDiagramInfo.getColor())
            {
            case WHITE:
                if (m_mapCoordAndInputTableWidgetItemPtr[coordStr] != NULL)
                {
                    m_mapCoordAndInputTableWidgetItemPtr[coordStr]->setDataUpdateColor(WHITE);
                }
                break;
            case GREEN:
                if (m_mapCoordAndInputTableWidgetItemPtr[coordStr] != NULL)
                {
                    m_mapCoordAndInputTableWidgetItemPtr[coordStr]->setDataUpdateColor(GREEN);
                }
                break;
            default:
                break;
            }
        }
    }

    m_historyLadderDiagramInfoList.clear();
    m_historyLadderDiagramInfoList = m_currentLadderDiagramInfoList;
}

void LadderDiagramPage::slot_searchInput(QString coordStr)
{
    if(m_mapCoordAndInputTableWidgetItemPtr[m_preCoordStr] != NULL)
    {
        m_mapCoordAndInputTableWidgetItemPtr[m_preCoordStr]->resetSelectedItemBorderColor();
    }
    if(m_mapCoordAndInputTableWidgetItemPtr[coordStr] != NULL)
    {
        m_preCoordStr = coordStr;
        m_mapCoordAndInputTableWidgetItemPtr[coordStr]->setSelectedItemBorderColor();
    }
}

void LadderDiagramPage::slot_searchOutput(QString coordStr)
{
    if(m_mapCoordAndOutputTableWidgetItemPtr[m_preCoordStr] != NULL)
    {
        m_mapCoordAndOutputTableWidgetItemPtr[m_preCoordStr]->resetSelectedItemBorderColor();
    }
    if(m_mapCoordAndOutputTableWidgetItemPtr[coordStr] != NULL)
    {
        m_preCoordStr = coordStr;
        m_mapCoordAndOutputTableWidgetItemPtr[coordStr]->setSelectedItemBorderColor();
    }
}

