#include "primarywidgettableitemmodel.h"
#include "ui_primarywidgettableitemmodel.h"

primaryWidgetTableItemModel::primaryWidgetTableItemModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::primaryWidgetTableItemModel)
{
    ui->setupUi(this);
    ui->lcdNumber_data->setSegmentStyle(QLCDNumber::Flat);    //设置数字字体为平面型
}

primaryWidgetTableItemModel::~primaryWidgetTableItemModel()
{
    delete ui;
}

//设置模板的标题
void primaryWidgetTableItemModel::setModelLabelTitle(QString title)
{
    ui->label_title->setText(title);
}

//设置模板的LCDNumber的数据
void primaryWidgetTableItemModel::setModelLCDNumberData(QString lcdData)
{
    ui->lcdNumber_data->display(lcdData);
}

//设置模板的LCDNumber的颜色
void primaryWidgetTableItemModel::setModelLCDNumberColor(QString color)
{
    ui->lcdNumber_data->setStyleSheet(tr("color:%1;").arg(color));
}
