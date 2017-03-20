#include "ladderdiagramclass.h"

#include <QDebug>

LadderDiagramClass::LadderDiagramClass(const QString &pageNumStr, const QString &xCoordStr,
                                       const QString &x2CoordStr, const QString &yCoordStr,
                                       const QString &busId, const QString &partTypeStr,
                                       const QString &meaning)
{
    m_pageNumStr = pageNumStr.rightJustified(2, '0');
    m_pageNumInt = m_pageNumStr.toInt();

    m_xCoordStr = xCoordStr.rightJustified(2, '0');
    m_xCoordInt = m_xCoordStr.toInt();

    m_x2CoordStr = x2CoordStr.rightJustified(2, '0');
    m_x2CoordInt = m_x2CoordStr.toInt();

    m_yCoordStr = yCoordStr.rightJustified(2, '0');
    m_yCoordInt = m_yCoordStr.toInt();

    m_busId = busId;

    m_partTypeStr = partTypeStr.rightJustified(2, '0');
    m_partTypeInt = m_partTypeStr.toInt();

    m_meaning = meaning;
}

LadderDiagramClass::LadderDiagramClass(const QString &pageNumStr, const QString &xCoordStr,
                                       const QString &yCoordStr, const QString &busId,
                                       const QString &partTypeStr, const QString &meaning)
{
    m_pageNumStr = pageNumStr.rightJustified(2, '0');
    m_pageNumInt = m_pageNumStr.toInt();

    m_xCoordStr = xCoordStr.rightJustified(2, '0');
    m_xCoordInt = m_xCoordStr.toInt();

    m_yCoordStr = yCoordStr.rightJustified(2, '0');
    m_yCoordInt = m_yCoordStr.toInt();

    m_busId = busId;

    m_partTypeStr = partTypeStr.rightJustified(2, '0');
    m_partTypeInt = m_partTypeStr.toInt();

    m_meaning = meaning;
}

LadderDiagramClass::LadderDiagramClass(const LadderDiagramClass &other)
{
    this->m_pageNumStr = other.m_pageNumStr;
    this->m_pageNumInt = other.m_pageNumInt;
    this->m_xCoordStr = other.m_xCoordStr;
    this->m_xCoordInt = other.m_xCoordInt;
    this->m_x2CoordStr = other.m_x2CoordStr;
    this->m_x2CoordInt = other.m_x2CoordInt;
    this->m_yCoordStr = other.m_yCoordStr;
    this->m_yCoordInt = other.m_yCoordInt;
    this->m_busId = other.m_busId;
    this->m_partTypeStr = other.m_partTypeStr;
    this->m_partTypeInt = other.m_partTypeInt;
    this->m_meaning = other.m_meaning;

    this->m_color = other.m_color;
}

LadderDiagramClass LadderDiagramClass::operator =(const LadderDiagramClass &other)
{
    if ( this == &other )
        return *this;

    this->m_pageNumStr = other.m_pageNumStr;
    this->m_pageNumInt = other.m_pageNumInt;
    this->m_xCoordStr = other.m_xCoordStr;
    this->m_xCoordInt = other.m_xCoordInt;
    this->m_x2CoordStr = other.m_x2CoordStr;
    this->m_x2CoordInt = other.m_x2CoordInt;
    this->m_yCoordStr = other.m_yCoordStr;
    this->m_yCoordInt = other.m_yCoordInt;
    this->m_busId = other.m_busId;
    this->m_partTypeStr = other.m_partTypeStr;
    this->m_partTypeInt = other.m_partTypeInt;
    this->m_meaning = other.m_meaning;

    this->m_color = other.m_color;

    return *this;
}

bool LadderDiagramClass::operator ==(const LadderDiagramClass &other) const
{
    if( (this->m_pageNumStr == other.m_pageNumStr) &&
            (this->m_xCoordStr == other.m_xCoordStr) &&
            (this->m_partTypeStr == other.m_partTypeStr) &&
            (this->m_x2CoordStr == other.m_x2CoordStr) &&
            (this->m_yCoordStr == other.m_yCoordStr) )
    {
        return true;
    }

    return false;
}

bool LadderDiagramClass::operator <(const LadderDiagramClass &other) const
{
    QString thisCompositeKey =
            this->getPageNumStr()+ this->getYCoordStr()
            + this->getXCoordStr() + this->getX2CoordStr();
    QString otherCompositeKey =
            other.getPageNumStr() + other.getYCoordStr()
            + other.getXCoordStr() + other.getX2CoordStr();

    if (thisCompositeKey < otherCompositeKey)
    {
        return true;
    }

    return false;
}

QString LadderDiagramClass::getPageNumStr() const
{
    return m_pageNumStr;
}

void LadderDiagramClass::setPageNumStr(const QString &pageNumStr)
{
    m_pageNumStr = pageNumStr.rightJustified(2, '0');
    m_pageNumInt = m_pageNumStr.toInt();
}

int LadderDiagramClass::getPageNumInt() const
{
    return m_pageNumInt;
}

void LadderDiagramClass::setPageNumInt(const int &pageNumInt)
{
    m_pageNumInt = pageNumInt;
    m_pageNumStr = QString::number(m_pageNumInt).rightJustified(2, '0');
}

QString LadderDiagramClass::getXCoordStr() const
{
    return m_xCoordStr;
}

void LadderDiagramClass::setXCoordStr(const QString &xCoordStr)
{
    m_xCoordStr = xCoordStr.rightJustified(2, '0');
    m_xCoordInt = m_xCoordStr.toInt();
}

int LadderDiagramClass::getXCoordInt() const
{
    return m_xCoordInt;
}

void LadderDiagramClass::setXCoordInt(const int &xCoordInt)
{
    m_xCoordInt = xCoordInt;
    m_xCoordStr = QString::number(m_xCoordInt).rightJustified(2, '0');
}

QString LadderDiagramClass::getX2CoordStr() const
{
    return m_x2CoordStr;
}

void LadderDiagramClass::setX2CoordStr(const QString &x2CoordStr)
{
    m_x2CoordStr = x2CoordStr.rightJustified(2, '0');
    m_x2CoordInt = m_x2CoordStr.toInt();
}

int LadderDiagramClass::getX2CoordInt() const
{
    return m_x2CoordInt;
}

void LadderDiagramClass::setX2CoordInt(const int &x2CoordInt)
{
    m_x2CoordInt = x2CoordInt;
    m_x2CoordStr = QString::number(m_x2CoordInt).rightJustified(2, '0');
}

QString LadderDiagramClass::getYCoordStr() const
{
    return m_yCoordStr;
}

void LadderDiagramClass::setYCoordStr(const QString &yCoordStr)
{
    m_yCoordStr = yCoordStr.rightJustified(2, '0');
    m_yCoordInt = m_yCoordStr.toInt();
}

int LadderDiagramClass::getYCoordInt() const
{
    return m_yCoordInt;
}

void LadderDiagramClass::setYCoordInt(const int &yCoordInt)
{
    m_yCoordInt = yCoordInt;
    m_yCoordStr = QString::number(m_yCoordInt).rightJustified(2, '0');
}

QString LadderDiagramClass::getBusId() const
{
    return m_busId;
}

void LadderDiagramClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

QString LadderDiagramClass::getPartTypeStr() const
{
    return m_partTypeStr;
}

void LadderDiagramClass::setPartTypeStr(const QString &partTypeStr)
{
    m_partTypeStr = partTypeStr.rightJustified(2, '0');
    m_partTypeInt = m_partTypeStr.toInt();
}

int LadderDiagramClass::getPartTypeInt() const
{
    return m_partTypeInt;
}

void LadderDiagramClass::setPartTypeInt(const int &partTypeInt)
{
    m_partTypeInt = partTypeInt;
    m_partTypeStr = QString::number(m_partTypeInt).rightJustified(2, '0');
}

QString LadderDiagramClass::getMeaning() const
{
    return m_meaning;
}

void LadderDiagramClass::setMeaning(const QString &meaning)
{
    m_meaning = meaning;
}

int LadderDiagramClass::getColor() const
{
    return m_color;
}

void LadderDiagramClass::setColor(const int &color)
{
    m_color = color;
}

bool LadderDiagramClass::LadderDiagramClassInfoSort(const LadderDiagramClass LadderDiagramClassInfoFirst,
                                                    const LadderDiagramClass LadderDiagramClassInfoSecond)
{
    return (LadderDiagramClassInfoFirst < LadderDiagramClassInfoSecond);
}
