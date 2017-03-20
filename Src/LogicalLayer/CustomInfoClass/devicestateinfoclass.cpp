#include "devicestateinfoclass.h"

DeviceStateInfoClass::DeviceStateInfoClass()
{
}

DeviceStateInfoClass::DeviceStateInfoClass(const QString &deviceStateId, const QString &busId,
                                           const int &color, const QString &imageName,
                                           const QString &deviceStateDesc, const int &conditionNum)
{
    m_deviceStateId = deviceStateId;
    m_busId = busId;
    m_color = color;
    m_imageName = imageName;
    m_deviceStateDesc = deviceStateDesc;
    m_conditionNum = conditionNum;
}

DeviceStateInfoClass::DeviceStateInfoClass(const DeviceStateInfoClass &other)
{
    this->m_deviceStateId = other.m_deviceStateId;
    this->m_busId = other.m_busId;
    this->m_color = other.m_color;
    this->m_imageName = other.m_imageName;
    this->m_deviceStateDesc = other.m_deviceStateDesc;
    this->m_conditionNum = other.m_conditionNum;
}

DeviceStateInfoClass DeviceStateInfoClass::operator =(const DeviceStateInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_deviceStateId = other.m_deviceStateId;
    this->m_busId = other.m_busId;
    this->m_color = other.m_color;
    this->m_imageName = other.m_imageName;
    this->m_deviceStateDesc = other.m_deviceStateDesc;
    this->m_conditionNum = other.m_conditionNum;

    return *this;
}

bool DeviceStateInfoClass::operator ==(const DeviceStateInfoClass &other) const
{
    if (this->m_deviceStateId == other.m_deviceStateId)
        return true;

    return false;
}

bool DeviceStateInfoClass::operator <(const DeviceStateInfoClass &other) const
{
    if (this->m_deviceStateId < other.m_deviceStateId)
        return true;

    return false;
}

QString DeviceStateInfoClass::getDeviceStateId() const
{
    return m_deviceStateId;
}

void DeviceStateInfoClass::setDeviceStateId(const QString &deviceStateId)
{
    m_deviceStateId = deviceStateId;
}

QString DeviceStateInfoClass::getBusId() const
{
    return m_busId;
}

void DeviceStateInfoClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

int DeviceStateInfoClass::getColor() const
{
    return m_color;
}

void DeviceStateInfoClass::setColor(const int &color)
{
    m_color = color;
}

QString DeviceStateInfoClass::getImageName() const
{
    return m_imageName;
}

void DeviceStateInfoClass::setImageName(const QString &imageName)
{
    m_imageName = imageName;
}

QString DeviceStateInfoClass::getDeviceStateDesc() const
{
    return m_deviceStateDesc;
}

void DeviceStateInfoClass::setDeviceStateDesc(const QString &deviceStateDesc)
{
    m_deviceStateDesc = deviceStateDesc;
}

int DeviceStateInfoClass::getConditionNum() const
{
    return m_conditionNum;
}

void DeviceStateInfoClass::setConditionNum(const int &conditionNum)
{
    m_conditionNum = conditionNum;
}

