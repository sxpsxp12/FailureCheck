#ifndef LGWIDGET_H
#define LGWIDGET_H

#include "enumtypeheader.h"
#include "baseinfoclass.h"
#include "lgwidgetpage.h"
#include "realtimedataclass.h"

#include <QWidget>
#include <QMap>

class RequestDataThread;
namespace Ui {
class LGWidget;
}

class LGWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LGWidget(RequestDataThread *, QWidget *parent = 0);
    ~LGWidget();

    void Inittable();

signals:
    void signal_setGuiContainerCurrentWidget(int);
    void signal_setFocusToGCUWidget();                  //设置按钮焦点信号

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pbt_pageUp_clicked();
    void on_pbt_pageDown_clicked();

    void slot_receiveLgWidgetData();

    void slot_setFocus();             //设置按钮焦点

private:
    Ui::LGWidget *ui;

    RequestDataThread *m_requestDataThread;
    BaseInfoList m_lgBaseInfoList;
    LGWidgetPage *m_lgWidgetPage;

    int m_currentPage;
    int m_totalPage;
    RealtimeDataInfoList m_realTimeDataInfoList;
    QMap<int,LGWidgetPage *> m_mapIntAndLGWidgetPagePtr;
};

#endif // LGWIDGET_H
