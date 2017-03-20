#ifndef SECONDWIDGETTABLEITEMMODELCLASS_H
#define SECONDWIDGETTABLEITEMMODELCLASS_H

#include <QWidget>
#include "enumtypeheader.h"
#include <QString>

namespace Ui {
class SecondWidgetTableItemModelClass;
}

class SecondWidgetTableItemModelClass : public QWidget
{
    Q_OBJECT
    
public:
    explicit SecondWidgetTableItemModelClass(QWidget *parent = 0);
    ~SecondWidgetTableItemModelClass();

    void setMachineStatus(int type,int status);          //设置机器类型及其工作状态，共有三种工作状态，两种类型
    void setMachineSymbol(QString machineSymbol);        //设置机器的标志
    void setMachineName(QString name);                  //设置机器的名字

private:
    Ui::SecondWidgetTableItemModelClass *ui;
};

#endif // SECONDWIDGETTABLEITEMMODELCLASS_H
