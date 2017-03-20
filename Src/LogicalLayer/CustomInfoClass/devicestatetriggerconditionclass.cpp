#include "devicestatetriggerconditionclass.h"

DeviceStateTriggerConditionClass::DeviceStateTriggerConditionClass()
{
}

DeviceStateTriggerConditionClass::DeviceStateTriggerConditionClass(
        const QString &deviceStateId, const QString &busId,
        const int &logicalRelationship, const double &triggerValue)
{
    m_deviceStateId = deviceStateId;
    m_busId = busId;
    m_logicalRelationship = logicalRelationship;
    m_triggerValue = triggerValue;
}

DeviceStateTriggerConditionClass::DeviceStateTriggerConditionClass(const DeviceStateTriggerConditionClass &other)
{
    this->m_deviceStateId = other.m_deviceStateId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;
}

DeviceStateTriggerConditionClass DeviceStateTriggerConditionClass::operator =(const DeviceStateTriggerConditionClass &other)
{
    if ( this == &other )
        return *this;

    this->m_deviceStateId = other.m_deviceStateId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;

    return *this;
}

bool DeviceStateTriggerConditionClass::operator ==(const DeviceStateTriggerConditionClass &other) const
{
    if (this->m_deviceStateId == other.m_deviceStateId)
        return true;

    return false;
}

bool DeviceStateTriggerConditionClass::operator <(const DeviceStateTriggerConditionClass &other) const
{
    if (this->m_deviceStateId < other.m_deviceStateId)
        return true;

    return false;
}

QString DeviceStateTriggerConditionClass::getDeviceStateId() const
{
    return m_deviceStateId;
}

void DeviceStateTriggerConditionClass::setDeviceStateId(const QString &deviceStateId)
{
    m_deviceStateId = deviceStateId;
}

QString DeviceStateTriggerConditionClass::getBusId() const
{
    return m_busId;
}

void DeviceStateTriggerConditionClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

int DeviceStateTriggerConditionClass::getLogicalRelationship() const
{
    return m_logicalRelationship;
}

void DeviceStateTriggerConditionClass::setLogicalRelationship(const int &logicalRelationship)
{
    m_logicalRelationship = logicalRelationship;
}

double DeviceStateTriggerConditionClass::getTriggerValue() const
{
    return m_triggerValue;
}

void DeviceStateTriggerConditionClass::setTriggerValue(const double &triggerValue)
{
    m_triggerValue = triggerValue;
}
