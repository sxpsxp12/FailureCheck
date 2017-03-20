#ifndef PRIMARYWIDGETTABLEITEMMODEL_H
#define PRIMARYWIDGETTABLEITEMMODEL_H

#include <QWidget>
#include <QString>

namespace Ui {
class primaryWidgetTableItemModel;
}

class primaryWidgetTableItemModel : public QWidget
{
    Q_OBJECT
    
public:
    explicit primaryWidgetTableItemModel(QWidget *parent = 0);
    ~primaryWidgetTableItemModel();

    void setModelLabelTitle(QString title);      //设置模板的标题
    void setModelLCDNumberData(QString lcdData);    //设置模板的LCDNumber的数据
    void setModelLCDNumberColor(QString color);  //设置模板的LCDNumber的颜色
    
private:
    Ui::primaryWidgetTableItemModel *ui;
};

#endif // PRIMARYWIDGETTABLEITEMMODEL_H
