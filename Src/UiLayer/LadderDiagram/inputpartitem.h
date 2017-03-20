#ifndef PARTITEM_H
#define PARTITEM_H

#include <QWidget>

#include "ladderdiagramclass.h"

namespace Ui {
class InputPartItem;
}

class InputPartItem : public QWidget
{
    Q_OBJECT
    
public:
    explicit InputPartItem(const LadderDiagramClass &LadderDiagramInfo,
                           const int &partState, QWidget *parent = 0);
    ~InputPartItem();

    void setSelectedItemBorderColor();   //设置选中的颜色
    void resetSelectedItemBorderColor();
    void setDataUpdateColor(int color);           //设置数据刷新时的颜色

private:
    Ui::InputPartItem *ui;

    LadderDiagramClass m_LadderDiagramInfo;
    int m_partState;

    bool m_flagSelect;
    QString m_imageSuffix;
    QString m_color;
};

#endif // PARTITEM_H
