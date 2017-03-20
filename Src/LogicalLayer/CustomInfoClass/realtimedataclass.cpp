#include "realtimedataclass.h"

RealtimeDataClass::RealtimeDataClass()
{
}

RealtimeDataClass::RealtimeDataClass(const QString &dataKey, const int &categoryId, const int &pageNum,
                                     const int &xCoord, const int &yCoord, const int &dataDisplayMethod,
                                     const QString &strNumericalValue, const int &color,
                                     const QString &imageName, const QString &deviceStateDesc,const bool &setDeviceState)
{
    m_dataKey = dataKey;
    m_categoryId = categoryId;
    m_pageNum = pageNum;
    m_xCoord = xCoord;
    m_yCoord = yCoord;
    m_dataDisplayMethod = dataDisplayMethod;
    m_dblNumericalValue = strNumericalValue.toDouble();
    m_strNumericalValue = strNumericalValue;
    m_color = color;
    m_imageName = imageName;
    m_deviceStateDesc = deviceStateDesc;
    m_isSetDeviceState = setDeviceState;
}

RealtimeDataClass::RealtimeDataClass(const QString &dataKey, const int &color, const QString &imageName,
                                     const QString &deviceStateDesc,const bool &setDeviceState)
{
    m_dataKey = dataKey;
    m_color = color;
    m_imageName = imageName;
    m_deviceStateDesc = deviceStateDesc;
    m_isSetDeviceState = setDeviceState;
}

RealtimeDataClass::RealtimeDataClass(const RealtimeDataClass &other)
{
    this->m_dataKey = other.m_dataKey;
    this->m_categoryId = other.m_categoryId;
    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;
    this->m_dataDisplayMethod = other.m_dataDisplayMethod;
    this->m_dblNumericalValue = other.m_dblNumericalValue;
    this->m_strNumericalValue = other.m_strNumericalValue;
    this->m_color = other.m_color;
    this->m_imageName = other.m_imageName;
    this->m_deviceStateDesc = other.m_deviceStateDesc;
    this->m_isSetDeviceState = other.m_isSetDeviceState;
}

RealtimeDataClass RealtimeDataClass::operator =(const RealtimeDataClass &other)
{
    if ( this == &other )
        return *this;

    this->m_dataKey = other.m_dataKey;
    this->m_categoryId = other.m_categoryId;
    this->m_pageNum = other.m_pageNum;
    this->m_xCoord = other.m_xCoord;
    this->m_yCoord = other.m_yCoord;
    this->m_dataDisplayMethod = other.m_dataDisplayMethod;
    this->m_dblNumericalValue = other.m_dblNumericalValue;
    this->m_strNumericalValue = other.m_strNumericalValue;
    this->m_color = other.m_color;
    this->m_imageName = other.m_imageName;
    this->m_deviceStateDesc = other.m_deviceStateDesc;
    this->m_isSetDeviceState = other.m_isSetDeviceState;

    return *this;
}

bool RealtimeDataClass::operator ==(const RealtimeDataClass &other) const
{
    if (this->m_dataKey == other.m_dataKey)
        return true;

    return false;
}

bool RealtimeDataClass::operator <(const RealtimeDataClass &other) const
{
    if (this->m_dataKey < other.m_dataKey)
        return true;

    return false;
}

double RealtimeDataClass::getDblNumericalValue() const
{
    return m_dblNumericalValue;
}

void RealtimeDataClass::setDblNumericalValue(const double &dblNumericalValue)
{
    m_dblNumericalValue = dblNumericalValue;
    m_strNumericalValue = QString::number(QString::number(m_dblNumericalValue, 'f', 1).toDouble());
}

QString RealtimeDataClass::getStrNumericalValue() const
{
    return m_strNumericalValue;
}

void RealtimeDataClass::setStrNumericalValue(const QString &strNumericalValue)
{
    m_strNumericalValue = strNumericalValue;
}

int RealtimeDataClass::getColor() const
{
    return m_color;
}

void RealtimeDataClass::setColor(const int &color)
{
    m_color = color;
}

QString RealtimeDataClass::getImageName() const
{
    return m_imageName;
}

void RealtimeDataClass::setImageName(const QString &imageName)
{
    m_imageName = imageName;
}

QString RealtimeDataClass::getDeviceStateDesc() const
{
    return m_deviceStateDesc;
}

void RealtimeDataClass::setDeviceStateDesc(const QString &deviceStateDesc)
{
    m_deviceStateDesc = deviceStateDesc;
}

int RealtimeDataClass::getCategoryId() const
{
    return m_categoryId;
}

void RealtimeDataClass::setCategoryId(const int &categoryId)
{
    m_categoryId = categoryId;
}

int RealtimeDataClass::getPageNum() const
{
    return m_pageNum;
}

void RealtimeDataClass::setPageNum(const int &pageNum)
{
    m_pageNum = pageNum;
}

int RealtimeDataClass::getXCoord() const
{
    return m_xCoord;
}

void RealtimeDataClass::setXCoord(const int &xCoord)
{
    m_xCoord = xCoord;
}

int RealtimeDataClass::getYCoord() const
{
    return m_yCoord;
}

void RealtimeDataClass::setYCoord(const int &yCoord)
{
    m_yCoord = yCoord;
}

QString RealtimeDataClass::getDataKey() const
{
    return m_dataKey;
}

void RealtimeDataClass::setDataKey(const QString &dataKey)
{
    m_dataKey = dataKey;
}

int RealtimeDataClass::getDataDisplayMethod() const
{
    return m_dataDisplayMethod;
}

void RealtimeDataClass::setDataDisplayMethod(const int &dataDisplayMethod)
{
    m_dataDisplayMethod = dataDisplayMethod;
}

bool RealtimeDataClass::getSetDeviceState() const
{
    return m_isSetDeviceState;
}

void RealtimeDataClass::setSetDeviceState(const bool &isSetDeviceState)
{
    m_isSetDeviceState = isSetDeviceState;
}
