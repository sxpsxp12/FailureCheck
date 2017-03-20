#ifndef OUTPUTPARTITEM_H
#define OUTPUTPARTITEM_H

#include <QWidget>

#include "ladderdiagramclass.h"

namespace Ui {
class OutputPartItem;
}

class OutputPartItem : public QWidget
{
    Q_OBJECT

public:
    explicit OutputPartItem(const LadderDiagramClass &LadderDiagramInfo,
                            const int &partState, QWidget *parent = 0);
    ~OutputPartItem();

    void setSelectedItemBorderColor();   //设置选中的颜色
    void resetSelectedItemBorderColor();
    void setDataUpdateColor(int color);           //设置数据刷新时的颜色

private:
    Ui::OutputPartItem *ui;

    LadderDiagramClass m_LadderDiagramInfo;
    int m_partState;

    bool m_flagSelect;
    QString m_imageSuffix;
    QString m_color;
};

#endif // OUTPUTPARTITEM_H
