#include "ladderdiagram.h"
#include "ui_ladderdiagram.h"

#include "databasesoperate.h"
#include "inputpartitem.h"
#include "outputpartitem.h"
#include "enumtypeheader.h"
#include "dataanalysis.h"

LadderDiagram::LadderDiagram(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LadderDiagram)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    m_realtimeDataList.clear();

    m_flagSearch = false;
    m_mutexSearch = false;
    m_flagSearchInput = true;
    m_searchCurNum = 0;
    m_curPageNum = 0;

    ui->stackedWidget->setCurrentIndex(0);

    //获取所有的列表信息，方便搜索时使用
    DatabasesOperate::getLadderDiagramAllListInfo(m_AllLadderDiagramInfo);
    DatabasesOperate::getLadderDiagramMaxPageNum(m_maxPageNum);

    initTableWidget();
}

LadderDiagram::~LadderDiagram()
{
    delete ui;

    if(m_ladderDiagramPage != NULL)
    {
        delete m_ladderDiagramPage;
    }
}

void LadderDiagram::resizeEvent(QResizeEvent *)
{
    //初始化widget的背景色
    setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    setPalette(palette);
}

void LadderDiagram::initTableWidget()
{
    ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
    ui->lbl_totalPage->setText(tr("共%1页").arg(m_maxPageNum+1));

    for (int i = 0; i <= m_maxPageNum; i++)
    {
        DatabasesOperate::getLadderDiagramInfo(i, m_ladderDiagramInputInfoList, m_ladderDiagramOutPutInfoList);
        m_ladderDiagramPage = new LadderDiagramPage(i, m_ladderDiagramInputInfoList, m_ladderDiagramOutPutInfoList);

        m_mapIntAndLadderDiagramPagePtr[i] = m_ladderDiagramPage;

        connect(this, SIGNAL(signal_searchInput(QString)), m_ladderDiagramPage, SLOT(slot_searchInput(QString)));
        connect(this, SIGNAL(signal_searchOutput(QString)), m_ladderDiagramPage, SLOT(slot_searchOutput(QString)));

        ui->stackedWidget->addWidget(m_ladderDiagramPage);
        m_ladderDiagramInputInfoList.clear();
        m_ladderDiagramOutPutInfoList.clear();
    }
}

void LadderDiagram::on_pbt_prePage_clicked()
{
    if(m_flagSearch == true)
    {
        if(m_searchCurNum > 0)
        {
            m_searchCurNum -= 1;
            if (m_flagSearchInput)
                emit signal_searchInput(m_searchIdCoordList.at(m_searchCurNum));
            else
                emit signal_searchOutput(m_searchIdCoordList.at(m_searchCurNum));

            if( m_searchPageNumList.at(m_searchCurNum) != m_curPageNum)
            {
                m_curPageNum = m_searchPageNumList.at(m_searchCurNum);
                ui->stackedWidget->setCurrentIndex(m_curPageNum);
                m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
                ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
            }
        }
        else
        {
            return;
        }

    }else
    {
        if(m_curPageNum > 0)
        {
            m_curPageNum-=1;
            ui->stackedWidget->setCurrentIndex(m_curPageNum);
            m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
            ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
        }else
        {
            return;
        }

    }
}

void LadderDiagram::on_pbt_nextPage_clicked()
{
    if(m_flagSearch == true)               //如果此时为搜索状态，为“下一个”
    {
        if(m_searchCurNum < m_searchIdCoordList.size()-1)
        {
            m_searchCurNum += 1;
            if (m_flagSearchInput)
                emit signal_searchInput(m_searchIdCoordList.at(m_searchCurNum));
            else
                emit signal_searchOutput(m_searchIdCoordList.at(m_searchCurNum));

            if( m_searchPageNumList.at(m_searchCurNum) != m_curPageNum)
            {
                m_curPageNum = m_searchPageNumList.at(m_searchCurNum);
                ui->stackedWidget->setCurrentIndex(m_curPageNum);
                m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
                ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
            }

        }else
        {
            return;
        }

    }else
    {
        if(m_curPageNum < m_maxPageNum)
        {
            m_curPageNum += 1;
            ui->stackedWidget->setCurrentIndex(m_curPageNum);
            m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
            ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
        }else
        {
            return;
        }
    }
}

void LadderDiagram::on_pbt_firstPage_clicked()
{
    if (m_curPageNum == 0)
        return;

    m_curPageNum = 0;
    ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
    ui->stackedWidget->setCurrentIndex(m_curPageNum);
    m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
}

void LadderDiagram::on_pbt_trailerPage_clicked()
{
    if (m_curPageNum == m_maxPageNum)
        return;

    m_curPageNum = m_maxPageNum;
    ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
    ui->stackedWidget->setCurrentIndex(m_maxPageNum);
    m_mapIntAndLadderDiagramPagePtr[m_maxPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
}

//接受数据槽
void LadderDiagram::slot_receiveLadderDiagramWidgetData()
{
    m_realtimeDataList.clear();
    m_realtimeDataList = DataAnalysis::m_ladderDiagramPartStateInfoList;
    //将数据传给当前界面
    m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
}

void LadderDiagram::on_pbt_clearSearch_clicked()
{
    m_searchId.clear();
    ui->lbl_searchId->clear();
    m_flagSearch = false;
    m_mutexSearch = false;
    m_searchIdCoordList.clear();
    m_searchPageNumList.clear();
    m_searchCurNum = 0;
    ui->pbt_nextPage->setText(tr("下一页"));
    ui->pbt_prePage->setText(tr("上一页"));
    emit signal_searchInput("");
    emit signal_searchOutput("");
}

void LadderDiagram::on_pbt_searchInput_clicked()
{
    QString coordStr;
    LadderDiagramClass ladderDiagramInfo;

    if (m_mutexSearch)            //如果为真，则说明已经在查找输出，两者不能同时进行，所以退出
        return;

    m_mutexSearch = true;
    m_flagSearch = true;
    m_flagSearchInput = true;

    for (int i = 0; i < m_AllLadderDiagramInfo.size(); i++)
    {
        ladderDiagramInfo = m_AllLadderDiagramInfo.at(i);
        if(ladderDiagramInfo.getPartTypeInt() != OUTPUT && ladderDiagramInfo.getPartTypeInt() != REPEATER)
            if(!m_searchId.isEmpty() && (ladderDiagramInfo.getBusId() == m_searchId))
            {
                coordStr = ladderDiagramInfo.getPageNumStr() + ladderDiagramInfo.getXCoordStr() + ladderDiagramInfo.getYCoordStr();
                m_searchPageNumList.append(ladderDiagramInfo.getPageNumInt());
                m_searchIdCoordList.append(coordStr);
            }
    }

    if (m_searchPageNumList.size() == 0)               //如果没有匹配的线号，则退出
    {
        m_flagSearch = false;
        m_mutexSearch = false;
        return;
    }

    if (m_searchPageNumList.at(0) != m_curPageNum)
    {
        m_curPageNum = m_searchPageNumList.at(0);
        ui->stackedWidget->setCurrentIndex(m_curPageNum);
        m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
        ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
    }

    ui->pbt_nextPage->setText(tr("下一个"));
    ui->pbt_prePage->setText(tr("上一个"));

    if (!m_searchIdCoordList.isEmpty())
    {
        emit signal_searchInput(m_searchIdCoordList.at(0));
    }
}

void LadderDiagram::on_pbt_searchOutput_clicked()
{
    QString coordStr;
    LadderDiagramClass ladderDiagramInfo;

    if (m_mutexSearch)
        return;

    m_mutexSearch = true;
    m_flagSearch = true;
    m_flagSearchInput = false;

    for(int i = 0; i < m_AllLadderDiagramInfo.size(); i++)
    {
        ladderDiagramInfo = m_AllLadderDiagramInfo.at(i);

        if(ladderDiagramInfo.getPartTypeInt() == OUTPUT|| ladderDiagramInfo.getPartTypeInt() == REPEATER)
            if(!m_searchId.isEmpty() && (ladderDiagramInfo.getBusId() == m_searchId))
            {
                coordStr = ladderDiagramInfo.getPageNumStr() + ladderDiagramInfo.getXCoordStr() + ladderDiagramInfo.getYCoordStr();
                m_searchPageNumList.append(ladderDiagramInfo.getPageNumInt());
                m_searchIdCoordList.append(coordStr);
            }
    }

    if (m_searchPageNumList.size() == 0)               //如果没有匹配的线号，则退出
    {
        m_flagSearch = false;
        m_mutexSearch = false;
        return;
    }

    if(!m_searchPageNumList.isEmpty() && (m_searchPageNumList.at(0) != m_curPageNum))
    {
        m_curPageNum = m_searchPageNumList.at(0);
        ui->stackedWidget->setCurrentIndex(m_curPageNum);
        m_mapIntAndLadderDiagramPagePtr[m_curPageNum]->refreshWidget(m_realtimeDataList, m_curPageNum);
        ui->lbl_curPage->setText(tr("第%1页").arg(m_curPageNum+1));
    }

    ui->pbt_nextPage->setText(tr("下一个"));
    ui->pbt_prePage->setText(tr("上一个"));

    if(!m_searchIdCoordList.isEmpty())
    {
        emit signal_searchOutput(m_searchIdCoordList.at(0));
    }
}

void LadderDiagram::on_comboBox_activated(const QString &arg1)
{
    m_searchId += arg1;
    ui->lbl_searchId->setText(m_searchId);
}

void LadderDiagram::slot_receiveOutputBusId(QString outputBusId)
{
    on_pbt_clearSearch_clicked();

    m_searchId = outputBusId;
    ui->lbl_searchId->setText(m_searchId);
    on_pbt_searchOutput_clicked();
}
