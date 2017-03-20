#include "tablewidgetitemclass.h"

TableWidgetItemClass::TableWidgetItemClass()
{
}

TableWidgetItemClass::TableWidgetItemClass(const int &pageNum, const int &xCoord, const int &yCoord)
{
    m_pageNum = pageNum;
    m_xCoord = xCoord;
    m_yCoord = yCoord;
}

TableWidgetItemClass::TableWidgetItemClass(const TableWidgetItemClass &other)
{
    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;
}

TableWidgetItemClass TableWidgetItemClass::operator =(const TableWidgetItemClass &other)
{
    if ( this == &other)
        return *this;

    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;

    return *this;
}

bool TableWidgetItemClass::operator ==(const TableWidgetItemClass &other) const
{
    if ((this->m_pageNum == other.m_pageNum) &&
        (this->m_xCoord == other.m_xCoord) &&
        (this->m_yCoord == other.m_yCoord))
        return true;

    return false;
}

bool TableWidgetItemClass::operator <(const TableWidgetItemClass &other) const
{
    if (this->m_pageNum < other.m_pageNum)
        return true;

    return false;
}

int TableWidgetItemClass::getPageNum() const
{
    return m_pageNum;
}

void TableWidgetItemClass::setPageNum(const int &pageNum)
{
    m_pageNum = pageNum;
}

int TableWidgetItemClass::getXCoord() const
{
    return m_xCoord;
}

void TableWidgetItemClass::setXCoord(const int &xCoord)
{
    m_xCoord = xCoord;
}

int TableWidgetItemClass::getYCoord() const
{
    return m_yCoord;
}

void TableWidgetItemClass::setYCoord(const int &yCoord)
{
    m_yCoord = yCoord;
}
