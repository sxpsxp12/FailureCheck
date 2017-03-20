#include "primarywidget.h"
#include "ui_primarywidget.h"

#include "databasesoperate.h"
#include "enumtypeheader.h"
#include <QDebug>
#include "dataanalysis.h"

FailureInfoList PrimaryWidget::failureInfoList;

PrimaryWidget::PrimaryWidget(RequestDataThread *requestDataThread, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrimaryWidget)
{
    ui->setupUi(this);
    m_requestDataThread = requestDataThread;

    ui->stackedWidget->setCurrentIndex(0);              //设置堆栈Widget的当前页面

    m_totalPage = 0;
    m_currentPage = 0;
    m_createPage = 0;

    DatabasesOperate::initInterfaceBaseInfoList(m_interfaceBaseInfoList, PRIMARY_INTERFACE);
    DatabasesOperate::getVerhicleInfo(m_verhicleInfoMapKeyAndValue);
    initTableWidget();           //初始化TableWidget
    initWidget();                //初始化界面上的图片和显示内容

    connect(ui->pbt_pageUp, SIGNAL(clicked()), this, SLOT(slot_pbt_pageUpClicked()));
    connect(ui->pbt_pageDown, SIGNAL(clicked()), this, SLOT(slot_pbt_pageDownClicked()));
}

PrimaryWidget::~PrimaryWidget()
{
    delete ui;
}

void PrimaryWidget::initTableWidget()
{
    QString coordStr;
    BaseInfoClass baseInfo;

    for (int i = 0; i < m_interfaceBaseInfoList.size(); i++)
    {
        baseInfo = m_interfaceBaseInfoList.at(i);

        if (baseInfo.getBusId() != "train")
        {
            coordStr = QString::number(baseInfo.getPageNum()) + QString::number(baseInfo.getXCoord()) + QString::number(baseInfo.getYCoord());

            m_mapCoordAndTableWidgetItemPtr[coordStr] = new primaryWidgetTableItemModel;
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLabelTitle(baseInfo.getMeaning());
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberData("0.00");
            m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberColor("#00fe00");
            ui->TableWidget_showContent->setCellWidget(baseInfo.getYCoord(), baseInfo.getXCoord(), m_mapCoordAndTableWidgetItemPtr[coordStr]);
        }
    }
}

void PrimaryWidget::initWidget()
{
   // QString verhicleNum;
    verhicleNum="";
    ui->Label_TrainStatusShow->setStyleSheet("border-image: url(:/MainWidget/Image/MainWidget/train1.png);");
    ui->Label_FaultNum->setText(tr("故障数：%1").arg(0));
    ui->label_pageNum->setText(tr("第%1页").arg(1));
    ui->label_Totalpage->setText(tr("共%1页").arg(1));

    if(m_verhicleInfoMapKeyAndValue.size()!=0)
    {
        verhicleNum.append(m_verhicleInfoMapKeyAndValue.value("verhicle_type")); //车型
        verhicleNum.append(m_verhicleInfoMapKeyAndValue.value("verhicle_num")); //车号
        ui->label_vehicalInfo->setText(tr("车号:%1 版本:%2").arg(verhicleNum,VERSION));
    }else
    {
        ui->label_vehicalInfo->setText("NULL");
    }
}

//切换到二级界面
void PrimaryWidget::on_Btn_secondScreen_clicked()
{
    emit signal_setGuiContainerCurrentWidget(SECONDARY_INTERFACE);
}

//接收点击ListWidget项的槽函数
void PrimaryWidget::slot_receiveListWidgetItemClickedData(int index)
{
    emit signal_setClickedListItem(index);
    emit signal_setGuiContainerCurrentWidget(FAILURE_LIST); //跳转到故障列表界面
}

void PrimaryWidget::slot_pbt_pageUpClicked()
{
    if (m_currentPage > 0)
    {
        m_currentPage -= 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

void PrimaryWidget::slot_pbt_pageDownClicked()
{
    if(m_currentPage < m_totalPage-1)
    {
        m_currentPage += 1;
        ui->stackedWidget->setCurrentIndex(m_currentPage);
        ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    }
}

//接受数据
void PrimaryWidget::slot_receivePrimaryWidgetData()
{
    realtimeDataInfoList.clear();
    failureInfoList.clear();
    realtimeDataInfoList = DataAnalysis::m_primaryWidgetRealtimeDataList;
    failureInfoList = DataAnalysis::m_primaryWidgetFailureInfoList;

    QString coordStr;

    for (int i = 0; i < realtimeDataInfoList.size(); i++)
    {
        if (realtimeDataInfoList.at(i).getDataKey() == "train")               //判断实时数据是否为机车动态状态图数据
        {
           //qDebug() << QString(":/MainWidget/Image/MainWidget/%1.png").arg(realtimeDataInfoList.at(i).getImageName());
            ui->Label_TrainStatusShow->setPixmap(QPixmap(QString(":/MainWidget/Image/MainWidget/%1.png").arg(realtimeDataInfoList.at(i).getImageName())));
            continue;              //刷新完此次图片数据后，即可跳出本次循环
        }

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
                m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberData(QString::number(realtimeDataInfoList.at(i).getDblNumericalValue(),'f',1));
                switch(realtimeDataInfoList.at(i).getColor())
                {
                case WHITE:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberColor("#fefefe");
                    break;
                case GREEN:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberColor("#00fe00");
                    break;
                case RED:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberColor("#fe0000");
                    break;
                case YELLOW:
                    m_mapCoordAndTableWidgetItemPtr[coordStr]->setModelLCDNumberColor("#fefe00");
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

    m_totalPage = (failureInfoList.size()-1) / PRIMARY_FAILURE_LIST_ROW_NUM + 1;

    ui->Label_FaultNum->setText(tr("故障数：%1").arg(failureInfoList.size()));

    for ( ; m_createPage < m_totalPage; m_createPage++)                       //创建页数小于总页数，则创建新页
    {
        m_primaryListWidgetPage = new PrimaryListWidgetPage(m_createPage+1);
        ui->stackedWidget->addWidget(m_primaryListWidgetPage);
        connect(this, SIGNAL(signal_sendPageData()),
                m_primaryListWidgetPage, SLOT(slot_receivePageData()));
        connect(m_primaryListWidgetPage, SIGNAL(signal_sendListWidgetItemClickedData(int)),
                this, SLOT(slot_receiveListWidgetItemClickedData(int)));
    }

    emit signal_sendPageData();

    if (ui->stackedWidget->currentIndex() > m_totalPage-1)          //更新数据后，判断所在页面是否超出总页数
    {
        ui->stackedWidget->setCurrentIndex(m_totalPage-1);          //如果超出，则切换到最后一页
        m_currentPage = m_totalPage-1;
    }

    ui->label_pageNum->setText(tr("第%1页").arg(m_currentPage+1));
    ui->label_Totalpage->setText(tr("共%1页").arg(m_totalPage));
}

void PrimaryWidget::slot_receiveMasterFlag(bool flag)
{
   static int num=0;
   if(flag)//是主机
   {
        ui->label_vehicalInfo->setText(tr("*车号:%1 版本:%2 %3").arg(verhicleNum).arg(VERSION).arg(num));
   }
   else
   {
      ui->label_vehicalInfo->setText(tr("车号:%1 版本:%2 %3").arg(verhicleNum).arg(VERSION).arg(num));
   }
   num++;


//    QString info=ui->label_vehicalInfo->text();
//    if(info!="*")
//    {
//        if(flag)        //是主机
//        {
//            info.clear();
//            info.append("*");
//            info.append(ui->label_vehicalInfo->text());
//            info.append(num);
//             ui->label_vehicalInfo->setText(info);
//        }


//    }
//    else
//    {
//        if(!flag)        //是从机
//        {
//           QString info=ui->label_vehicalInfo->text();
//           info=info.mid(1,info.length()-1);
//           ui->label_vehicalInfo->setText(info);
//        }


//    }



}
