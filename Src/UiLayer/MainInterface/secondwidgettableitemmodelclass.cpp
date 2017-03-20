#include "secondwidgettableitemmodelclass.h"
#include "ui_secondwidgettableitemmodelclass.h"

SecondWidgetTableItemModelClass::SecondWidgetTableItemModelClass(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecondWidgetTableItemModelClass)
{
    ui->setupUi(this);
}

SecondWidgetTableItemModelClass::~SecondWidgetTableItemModelClass()
{
    delete ui;
}

//设置机器类型及其工作状态，共有三种工作状态，两种类型
void SecondWidgetTableItemModelClass::setMachineStatus(int type, int status)
{
    if(type == ELECTRAIL_MACHINE)   //电机类型
    {
        switch(status)
        {
        case NO_WORK:               //不工作状态
            ui->widget_machineBackground->setStyleSheet(tr(	"QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/no_work1.png);}"));
            break;
        case NORMAL_WORK:           //正常工作状态
            ui->widget_machineBackground->setStyleSheet(tr(	"QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/normal_work1.png);}"));
            break;
        case ABNORMAL_WORK:         //异常工作状态
            ui->widget_machineBackground->setStyleSheet(tr(	"QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/abnormal_work1.png);}"));
            break;
        default:break;
        }
    }
    else if(type == OTHER_MACHINE)                           //其他类型
    {
        switch(status)
        {
        case NO_WORK:               //不工作状态
            //保持图片的原有大小不变
            ui->widget_machineBackground->setStyleSheet("QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/no_work2.png);}");
            break;
        case NORMAL_WORK:           //正常工作状态
            //保持图片的原有大小不变
            ui->widget_machineBackground->setStyleSheet("QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/normal_work2.png);}");
            break;
        case ABNORMAL_WORK:         //异常工作状态
            //保持图片的原有大小不变
            ui->widget_machineBackground->setStyleSheet("QWidget#widget_machineBackground{image: url(:/secondWidget/Image/secondWidget/abnormal_work2.png);}");
            break;
        default:break;
        }
    }
}
//设置机器的标志
void SecondWidgetTableItemModelClass::setMachineSymbol(QString machineSymbol)
{
    ui->label_symbol->setText(machineSymbol);
}
//设置机器的名字
void SecondWidgetTableItemModelClass::setMachineName(QString name)
{
    ui->label_machineExplain->setText(tr("%1").arg(name));
}
