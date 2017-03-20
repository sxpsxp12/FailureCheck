#include "failurerecord.h"
#include "ui_failurerecord.h"
#include "enumtypeheader.h"
#include "dataanalysis.h"

FailureRecord::FailureRecord(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FailureRecord)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;
    initTableWidget();
    DatabasesOperate::initFailureHistoryList(m_failureHistoryRecordInfoList);
    m_currentPage = 0;
    m_TotalPageNum = m_failureHistoryRecordInfoList.size()/ONEPAGE_ITEM_ROWNUM;
    initLabel();
}
void FailureRecord::initTableWidget()
{
    ui->tableWidget->setColumnWidth(0,51);
    ui->tableWidget->setColumnWidth(1,50);
    ui->tableWidget->setColumnWidth(2,103);
    ui->tableWidget->setColumnWidth(3,83);
    ui->tableWidget->setColumnWidth(4,103);
    ui->tableWidget->setColumnWidth(5,84);
    ui->tableWidget->setColumnWidth(6,318);
    ui->tableWidget->horizontalHeader()->setVisible(true);

    //初始化键值对
    //17行7列
    for(int i = 0; i < ONEPAGE_ITEM_ROWNUM;i++)
    {
        for(int j = 0;j < 7;j++)
        {
            QString address = QString::number(i).append("~").append(QString::number(j));
            m_mapAddressAndItemPtr[address] = new  QTableWidgetItem("");
            m_mapAddressAndItemPtr[address]->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i,j,m_mapAddressAndItemPtr[address]);
        }
    }
}

void FailureRecord::initLabel()
{
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_TotalPageNum+1));
    //将界面更新到当前所在页
    updateTableWidgetContent(m_currentPage);
}

void FailureRecord::updateTableWidgetContent(int &page)
{
    //将所有内容置空
    foreach (QTableWidgetItem * item, m_mapAddressAndItemPtr) {
        item->setText("");
    }

    int historyRecordlistSize = m_failureHistoryRecordInfoList.size();
    if(historyRecordlistSize != 0)
    {
        // 根据坐标将Item 对象取出赋值
        for(int i = 0;i < ONEPAGE_ITEM_ROWNUM;i++)
        {
            int currentHistoryListNum = historyRecordlistSize-(page*ONEPAGE_ITEM_ROWNUM+i)-1;
            //当前的列表索引号小于0则退出
            if(currentHistoryListNum < 0)
            {
                break;
            }

            QString address0 = QString::number(i).append("~").append(QString::number(0));
            m_mapAddressAndItemPtr[address0]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getFailureId());

            QString address1 = QString::number(i).append("~").append(QString::number(1));
            m_mapAddressAndItemPtr[address1]->setText(QString::number(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getFailureLv()));

            QString address2 = QString::number(i).append("~").append(QString::number(2));
            m_mapAddressAndItemPtr[address2]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getStartTime().split(" ",QString::SkipEmptyParts).at(0));

            QString address3 = QString::number(i).append("~").append(QString::number(3));
            m_mapAddressAndItemPtr[address3]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getStartTime().split(" ",QString::SkipEmptyParts).at(1));

            if(!m_failureHistoryRecordInfoList.at(currentHistoryListNum).getEndTime().isEmpty())
            {
                QString address4 = QString::number(i).append("~").append(QString::number(4));
                m_mapAddressAndItemPtr[address4]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getEndTime().split(" ",QString::SkipEmptyParts).at(0));

                QString address5 = QString::number(i).append("~").append(QString::number(5));
                m_mapAddressAndItemPtr[address5]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getEndTime().split(" ",QString::SkipEmptyParts).at(1));
            }
            QString address6 = QString::number(i).append("~").append(QString::number(6));
            m_mapAddressAndItemPtr[address6]->setText(m_failureHistoryRecordInfoList.at(currentHistoryListNum).getFailureName());
        }
    }
}

FailureRecord::~FailureRecord()
{
    delete ui;
}

void FailureRecord::on_pbt_lastPage_clicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
        updateTableWidgetContent(m_currentPage);
    }
}

void FailureRecord::on_pbt_nextPage_clicked()
{
    if(m_currentPage < m_TotalPageNum)
    {
        m_currentPage += 1;
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
        updateTableWidgetContent(m_currentPage);
    }
}

void FailureRecord::on_pbt_headPage_clicked()
{
    m_currentPage = 0;
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    updateTableWidgetContent(m_currentPage);
}

void FailureRecord::on_pbt_endPage_clicked()
{
    m_currentPage = m_TotalPageNum;
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    updateTableWidgetContent(m_currentPage);
}

void FailureRecord::slot_ReceiveFailureRecordWidgetData()
{
    //将传进来的故障列表分情况进行记录
    foreach (FailureHistoryRecordClass failureHistoryInfo, DataAnalysis::m_failureHistoryRecordInfoList)
    {
        bool isReplace = false;
        bool isExists = false;

        //将内存中的故障列表进行大小判断，大于10000则删除旧的记录
        if(m_failureHistoryRecordInfoList.size() >= FAILURE_HISTORY_RECORD_MAXNUM)
        {
            m_failureHistoryRecordInfoList.removeFirst();
        }

        //m_failureHistoryRecordInfoList该故障列表存放所有的故障，包括很久以前的，其中可能会有ID一样的故障，所以要区分最近的故障是否已经解决，只能通过Id和发生时间来唯一区分故障
        for(int i=0;i < m_failureHistoryRecordInfoList.size();i++)
        {
            //用id和startTime唯一区分记录
            if( (!failureHistoryInfo.getEndTime().isEmpty()) && (m_failureHistoryRecordInfoList.at(i).getFailureId() == failureHistoryInfo.getFailureId()) && (m_failureHistoryRecordInfoList.at(i).getStartTime() == failureHistoryInfo.getStartTime()) )
            {
                //将已经解决的故障替换
                m_failureHistoryRecordInfoList.replace(i,failureHistoryInfo);
                isReplace = true;
                break;
            }
        }

        if(!isReplace)      //该故障是未解决的故障或者是个新的故障
        {
            for(int i=0;i < m_failureHistoryRecordInfoList.size();i++)
            {
                //已经包含了未解决的故障，不进行追加
                if((m_failureHistoryRecordInfoList.at(i).getFailureId() == failureHistoryInfo.getFailureId()) && (m_failureHistoryRecordInfoList.at(i).getStartTime() == failureHistoryInfo.getStartTime()))
                {
                    isExists = true;
                    break;
                }
            }

            if(!isExists)   //列表中不存在该故障，进行追加
            {
#if 0
                //播报一级界面新的故障
                switch(failureHistoryInfo.getFailureLv())
                {
                case SERIOUS:
                case MEDIUM:
                case SLIGHT:
                    m_requestDataThread->setIsVoicePlay(true);
                    m_requestDataThread->setVoicePlayContent(failureHistoryInfo.getFailureName());
                    break;
                case PROMPT:
                    break;
                }
#endif
                //将新产生的故障进行记录
                m_failureHistoryRecordInfoList.append(failureHistoryInfo);
            }
        }
    }
    //重新计算总页数
    m_TotalPageNum = m_failureHistoryRecordInfoList.size()/ONEPAGE_ITEM_ROWNUM;
    initLabel();
}
