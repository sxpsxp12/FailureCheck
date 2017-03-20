#ifndef LADDERDIAGRAMPAGE_H
#define LADDERDIAGRAMPAGE_H

#include <QWidget>
#include <QMap>

#include "ladderdiagramclass.h"
#include "enumtypeheader.h"
#include "inputpartitem.h"
#include "outputpartitem.h"

class InputPartItem;
class OutputPartItem;

namespace Ui {
class LadderDiagramPage;
}

class LadderDiagramPage : public QWidget
{
    Q_OBJECT
    
public:
    explicit LadderDiagramPage(int pageNum, LadderDiagramInfoList ladderDiagramInputInfoList,
                               LadderDiagramInfoList ladderDiagramOutputInfoList,QWidget *parent = 0);
    ~LadderDiagramPage();

    void initTableWidget( int pageNum,LadderDiagramInfoList ladderDiagramInputInfoList,
                         LadderDiagramInfoList ladderDiagramOutputInfoList );

    void refreshWidget(const LadderDiagramInfoList &realtimeDataList, int curPageNum);
private slots:
    void slot_searchInput(QString coordStr);
    void slot_searchOutput(QString coordStr);
private:
    Ui::LadderDiagramPage *ui;

    QString m_preCoordStr;

    QMap<QString, InputPartItem*> m_mapCoordAndInputTableWidgetItemPtr;
    QMap<QString, OutputPartItem*> m_mapCoordAndOutputTableWidgetItemPtr;

    LadderDiagramInfoList m_currentLadderDiagramInfoList;
    LadderDiagramInfoList m_historyLadderDiagramInfoList;
};

#endif // LADDERDIAGRAMPAGE_H
