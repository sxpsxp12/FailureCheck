#include "baseinfoclass.h"

BaseInfoClass::BaseInfoClass(const QString &busId, const int &categoryId, const QString &interfaces,
                             const QString &indicatorLight, const QString &meaning,
                             const int &pageNum, const int &xCoord, const int &yCoord, const int &dataDisplayMethod)
{
    m_busId = busId;
    m_categoryId = categoryId;
    m_interfaces = interfaces;
    m_indicatorLight = indicatorLight;
    m_meaning = meaning;
    m_pageNum = pageNum;
    m_xCoord = xCoord;
    m_yCoord = yCoord;
    m_dataDisplayMethod = dataDisplayMethod;
}

BaseInfoClass::BaseInfoClass(const BaseInfoClass &other)
{
    this->m_busId = other.m_busId;
    this->m_categoryId = other.m_categoryId;
    this->m_interfaces = other.m_interfaces;
    this->m_indicatorLight = other.m_indicatorLight;
    this->m_meaning = other.m_meaning;
    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;
    this->m_dataDisplayMethod = other.m_dataDisplayMethod;
}

BaseInfoClass BaseInfoClass::operator =(const BaseInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_busId = other.m_busId;
    this->m_categoryId = other.m_categoryId;
    this->m_interfaces = other.m_interfaces;
    this->m_indicatorLight = other.m_indicatorLight;
    this->m_meaning = other.m_meaning;
    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;
    this->m_dataDisplayMethod = other.m_dataDisplayMethod;

    return *this;
}

bool BaseInfoClass::operator ==(const BaseInfoClass &other) const
{
    if (this->m_busId == other.m_busId)
        return true;

    return false;
}

bool BaseInfoClass::operator <(const BaseInfoClass &other) const
{
    if (this->m_busId < other.m_busId)
        return true;

    return false;
}

QString BaseInfoClass::getBusId() const
{
    return m_busId;
}

void BaseInfoClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

QString BaseInfoClass::getInterfaces() const
{
    return m_interfaces;
}

void BaseInfoClass::setInterfaces(const QString &interfaces)
{
    m_interfaces = interfaces;
}

QString BaseInfoClass::getIndicatorLight() const
{
    return m_indicatorLight;
}

void BaseInfoClass::setIndicatorLight(const QString &indicatorLight)
{
    m_indicatorLight = indicatorLight;
}

QString BaseInfoClass::getMeaning() const
{
    return m_meaning;
}

void BaseInfoClass::setMeaning(const QString &meaning)
{
    m_meaning = meaning;
}

int BaseInfoClass::getCategoryId() const
{
    return m_categoryId;
}

void BaseInfoClass::setCategoryId(const int &categoryId)
{
    m_categoryId = categoryId;
}

int BaseInfoClass::getPageNum() const
{
    return m_pageNum;
}

void BaseInfoClass::setPageNum(const int &pageNum)
{
    m_pageNum = pageNum;
}

int BaseInfoClass::getXCoord() const
{
    return m_xCoord;
}

void BaseInfoClass::setXCoord(const int &xCoord)
{
    m_xCoord = xCoord;
}

int BaseInfoClass::getYCoord() const
{
    return m_yCoord;
}

void BaseInfoClass::setYCoord(const int &yCoord)
{
    m_yCoord = yCoord;
}

int BaseInfoClass::getDataDisplayMethod() const
{
    return m_dataDisplayMethod;
}

void BaseInfoClass::setDataDisplayMethod(const int &dataDisplayMethod)
{
    m_dataDisplayMethod = dataDisplayMethod;
}
