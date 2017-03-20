#ifndef LADDERDIAGRAM_H
#define LADDERDIAGRAM_H

#include <QWidget>
#include <QMap>
#include <QMessageBox>

#include "ladderdiagramclass.h"
#include "ladderdiagrampage.h"

class RequestDataThread;
class InputPartItem;
class OutputPartItem;

#define LADDER_DEAGRAM_INPUT_INFO_ROW_COUNT 12
#define LADDER_DEAGRAM_INPUT_INFO_COLUMN_COUNT 14

#define LADDER_DEAGRAM_OUTPUT_INFO_ROW_COUNT 12
#define LADDER_DEAGRAM_OUTPUT_INFO_COLUMN_COUNT 1

namespace Ui {
class LadderDiagram;
}

class LadderDiagram : public QWidget
{
    Q_OBJECT
    
public:
    explicit LadderDiagram(RequestDataThread *requestDataThread, QWidget *parent = 0);
    ~LadderDiagram();

    void resizeEvent(QResizeEvent *);
    void initTableWidget();

signals:
    void signal_searchInput(QString);
    void signal_searchOutput(QString);

private slots:
    void on_pbt_prePage_clicked();
    void on_pbt_nextPage_clicked();
    void on_pbt_firstPage_clicked();
    void on_pbt_trailerPage_clicked();

    void on_pbt_clearSearch_clicked();
    void on_pbt_searchInput_clicked();
    void on_pbt_searchOutput_clicked();

    void on_comboBox_activated(const QString &arg1);
    void slot_receiveOutputBusId(QString outputBusId);
    void slot_receiveLadderDiagramWidgetData();

private:
    Ui::LadderDiagram *ui;

    RequestDataThread *m_requestDataThread;
    LadderDiagramPage *m_ladderDiagramPage;

    LadderDiagramInfoList m_ladderDiagramInputInfoList;
    LadderDiagramInfoList m_ladderDiagramOutPutInfoList;
    QString m_searchId;
    QStringList m_searchIdCoordList;

    int m_maxPageNum;
    int m_curPageNum;
    int m_searchCurPageNum;
    int m_searchCurNum;
    bool m_flagSearchInput;
    bool m_flagSearch;         //搜索的标志位
    bool m_mutexSearch;        //保证搜索输出和搜索输入不冲突

    LadderDiagramInfoList m_PageListSearch;    //符合搜索内容的列表
    QList<int> m_searchPageNumList;             //搜索到的列号
    LadderDiagramInfoList m_AllLadderDiagramInfo;

    LadderDiagramInfoList m_realtimeDataList;
    QMap<int,LadderDiagramPage *> m_mapIntAndLadderDiagramPagePtr;
};

#endif // LADDERDIAGRAM_H
