#include "interfacelamphole.h"
#include "ui_interfacelamphole.h"

#include "interfacelampholepage.h"
#include "databasesoperate.h"
#include "enumtypeheader.h"
#include "dataanalysis.h"

InterfaceLampHole::InterfaceLampHole(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InterfaceLampHole)
{
    ui->setupUi(this);

    m_requestDataThread = requestDataThread;
    m_realtimeDataInfoList.clear();
    ui->stackedWidget->setCurrentIndex(0);

    m_totalPage = 0;
    m_currentPage = 0;

    m_totalSearchNum = 0;
    m_currentSearchNum = 0;

    m_searchFlag = false;

    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList, INTERFACE_LAMP_HOLE);
    initWidget();

    connect(ui->pbt_search, SIGNAL(clicked()), this, SLOT(slot_pbt_searchClicked()));
}

InterfaceLampHole::~InterfaceLampHole()
{
    delete ui;

    if(m_interfaceLampHolePage != NULL)
    {
        delete m_interfaceLampHolePage;
    }
}

//刷新label
void InterfaceLampHole::updateLabel(int currentPage, int totalPage)
{
    ui->label_pageNum->setText(tr("第%1页").arg(currentPage));
    ui->label_totalPage->setText(tr("共%1页").arg(totalPage));
}

//线号槽
void InterfaceLampHole::on_pushButton_clicked()
{
    emit signal_setGuiContainerCurrentWidget(BUS_ID_INTERFACE);
    emit signal_setFocusToBusIdWidget();
}

//接口灯孔槽
void InterfaceLampHole::on_pushButton_2_clicked()
{
    emit signal_setGuiContainerCurrentWidget(INTERFACE_LAMP_HOLE);
}

void InterfaceLampHole::initWidget()
{
    for (int i = 0; i < m_interfaceBaseInfoList.size(); i++)
    {
        if (m_totalPage < m_interfaceBaseInfoList.at(i).getPageNum())
        {
            m_totalPage = m_interfaceBaseInfoList.at(i).getPageNum();
        }
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_totalPage->setText(tr("共%1页").arg(m_totalPage));
    ui->pbt_pageDown->setText(tr("下一页"));
    ui->pbt_pageUp->setText(tr("上一页"));

    for (int i = 1; i <= m_totalPage; i++)
    {
        m_interfaceLampHolePage = new InterfaceLampHolePage(m_interfaceBaseInfoList, i);
        m_mapIntAndInterfaceLampHolePagePtr[i-1] = m_interfaceLampHolePage;
        ui->stackedWidget->addWidget(m_interfaceLampHolePage);
        connect(this,SIGNAL(signal_searchId(int,int,int)),m_interfaceLampHolePage,SLOT(slot_searchId(int,int,int)));
    }
}

void InterfaceLampHole::on_pbt_pageDown_clicked()
{
    int pageNum;
    int xCoord;
    int yCoord;
    QStringList fuzzySearchList;

    if (m_searchFlag)
    {
        if (m_currentSearchNum < m_totalSearchNum-1)
        {
            m_currentSearchNum += 1;

            fuzzySearchList = m_fuzzySearchList.at(m_currentSearchNum).split("|", QString::SkipEmptyParts);
            pageNum = fuzzySearchList.at(0).toInt();
            xCoord = fuzzySearchList.at(1).toInt();
            yCoord = fuzzySearchList.at(2).toInt();

            m_currentPage = pageNum-1;
            ui->stackedWidget->setCurrentIndex(m_currentPage);
            m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
            ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));

            emit signal_searchId(pageNum, xCoord, yCoord);
        }
    }
    else
    {
        if(m_currentPage < m_totalPage-1)
        {
            m_currentPage += 1;
            ui->stackedWidget->setCurrentIndex(m_currentPage);
            m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
            ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
        }
    }
}

void InterfaceLampHole::on_pbt_pageUp_clicked()
{
    int pageNum;
    int xCoord;
    int yCoord;
    QStringList fuzzySearchList;

    if (m_searchFlag)
    {
        if (m_currentSearchNum > 0)
        {
            m_currentSearchNum -= 1;

            fuzzySearchList = m_fuzzySearchList.at(m_currentSearchNum).split("|", QString::SkipEmptyParts);
            pageNum = fuzzySearchList.at(0).toInt();
            xCoord = fuzzySearchList.at(1).toInt();
            yCoord = fuzzySearchList.at(2).toInt();

            m_currentPage = pageNum-1;
            ui->stackedWidget->setCurrentIndex(m_currentPage);
            m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
            ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));

            emit signal_searchId(pageNum, xCoord, yCoord);
        }
    }else
    {
        if(m_currentPage > 0)
        {
            m_currentPage-=1;
            ui->stackedWidget->setCurrentIndex(m_currentPage);
            m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
            ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
        }
    }
}

void InterfaceLampHole::on_pbt_homePage_clicked()
{
    m_currentPage = 0;
    ui->stackedWidget->setCurrentIndex(m_currentPage);
    m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
}

void InterfaceLampHole::on_pbt_endPage_clicked()
{
    m_currentPage = m_totalPage-1;
    ui->stackedWidget->setCurrentIndex(m_currentPage);
    m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
}

void InterfaceLampHole::slot_receiveInterfaceLampHoleWidgetData()
{
    m_realtimeDataInfoList.clear();
    m_realtimeDataInfoList = DataAnalysis::m_interfaceLampHoleWidgetRealtimeDataList;

    m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
}

void InterfaceLampHole::on_pbt_clear_clicked()
{
    m_searchId.clear();
    ui->label->clear();
    m_searchFlag = false;

    ui->pbt_pageDown->setText(tr("下一页"));
    ui->pbt_pageUp->setText(tr("上一页"));

    emit signal_searchId(-1, -1, -1);
}

void InterfaceLampHole::slot_pbt_searchClicked()
{
    int pageNum;
    int xCoord;
    int yCoord;
    QString coordStr;
    bool flagExist = false;

    BaseInfoClass interFaceLampHoleInfo;
    int length = m_searchId.length();

    m_fuzzySearchList.clear();
    m_totalSearchNum = 0;
    m_currentSearchNum = 0;

    for (int i = 0; i < m_interfaceBaseInfoList.size(); i++)
    {
        interFaceLampHoleInfo = m_interfaceBaseInfoList.at(i);
        for (int j = 0; j < interFaceLampHoleInfo.getBusId().length()-(length-1); j++)
        {
            if (interFaceLampHoleInfo.getBusId().mid(j, length) == m_searchId)
            {
                pageNum = interFaceLampHoleInfo.getPageNum();
                xCoord = interFaceLampHoleInfo.getXCoord();
                yCoord = interFaceLampHoleInfo.getYCoord();
                coordStr = QString::number(pageNum) + "|" +
                        QString::number(xCoord) + "|" +
                        QString::number(yCoord);
                m_fuzzySearchList.append(coordStr);
                flagExist = true;
                break;
            }
        }
    }

    QStringList fuzzySearchList;

    if (!m_fuzzySearchList.isEmpty())
    {
        fuzzySearchList = m_fuzzySearchList.at(0).split("|", QString::SkipEmptyParts);

        pageNum = fuzzySearchList.at(0).toInt();
        xCoord = fuzzySearchList.at(1).toInt();
        yCoord = fuzzySearchList.at(2).toInt();

        m_totalSearchNum = m_fuzzySearchList.size();
        m_currentSearchNum = 0;
    }

    if (flagExist && ((pageNum-1) != m_currentPage))
    {
        m_currentPage = pageNum-1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        m_mapIntAndInterfaceLampHolePagePtr[m_currentPage]->refreshWidget(m_realtimeDataInfoList, m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }

    m_searchFlag = true;

    ui->pbt_pageDown->setText(tr("下一个"));
    ui->pbt_pageUp->setText(tr("上一个"));

    emit signal_searchId(pageNum, xCoord, yCoord);
}

void InterfaceLampHole::slot_tableWidgetItemClicked(QTableWidgetItem *tableWidgetItem)
{
    m_searchId = tableWidgetItem->text();
    ui->label->setText(m_searchId);
    slot_pbt_searchClicked();
}

void InterfaceLampHole::on_cbx_enumSelection_activated(const QString &arg1)
{
    m_searchId += arg1;
    ui->label->setText(m_searchId);
}

void InterfaceLampHole::slot_setFocus()
{
    ui->pushButton_2->setFocus();
}
