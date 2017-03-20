#ifndef TABLEWIDGETITEMCLASS_H
#define TABLEWIDGETITEMCLASS_H

#include <QString>
#include <QList>

class TableWidgetItemClass
{
public:
    TableWidgetItemClass();

    TableWidgetItemClass( const int &pageNum, const int &xCoord, const int &yCoord );

    TableWidgetItemClass( const TableWidgetItemClass &other );

    TableWidgetItemClass operator = (const TableWidgetItemClass &other );

    bool operator == ( const TableWidgetItemClass &other ) const;

    bool operator < ( const TableWidgetItemClass &other ) const;

    int getPageNum() const;
    void setPageNum(const int &pageNum);

    int getXCoord() const;
    void setXCoord(const int &xCoord);

    int getYCoord() const;
    void setYCoord(const int &yCoord);

private:
    int m_pageNum;
    int m_xCoord;
    int m_yCoord;
};

typedef QList<TableWidgetItemClass> TableWidgetItemList;

#endif // TABLEWIDGETITEMCLASS_H
