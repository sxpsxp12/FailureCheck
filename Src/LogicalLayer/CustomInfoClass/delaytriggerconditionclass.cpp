#include "delaytriggerconditionclass.h"

DelayTriggerConditionClass::DelayTriggerConditionClass()
{
}

DelayTriggerConditionClass::DelayTriggerConditionClass(const QString &delayId, const QString &busId,
                                                       const QString &logicalRelationship, const QString &triggerValue)
{
    m_delayId = delayId;
    m_busId = busId;
    m_logicalRelationship = logicalRelationship;
    m_triggerValue = triggerValue;
}

DelayTriggerConditionClass::DelayTriggerConditionClass(const DelayTriggerConditionClass &other)
{
    this->m_delayId = other.m_delayId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;
}

DelayTriggerConditionClass DelayTriggerConditionClass::operator =(const DelayTriggerConditionClass &other)
{
    if ( this == &other )
        return *this;

    this->m_delayId = other.m_delayId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;

    return *this;
}

bool DelayTriggerConditionClass::operator ==(const DelayTriggerConditionClass &other) const
{
    if ((this->m_delayId == other.m_delayId) &&
        (this->m_busId == other.m_busId))
        return true;

    return false;
}

bool DelayTriggerConditionClass::operator <(const DelayTriggerConditionClass &other) const
{
    if (this->m_delayId < other.m_delayId)
        return true;

    return false;
}

QString DelayTriggerConditionClass::getDelayId() const
{
    return m_delayId;
}

void DelayTriggerConditionClass::setDelayId(const QString &delayId)
{
    m_delayId = delayId;
}

QString DelayTriggerConditionClass::getBusId() const
{
    return m_busId;
}

void DelayTriggerConditionClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

QString DelayTriggerConditionClass::getLogicalRelationship() const
{
    return m_logicalRelationship;
}

void DelayTriggerConditionClass::setLogicalRelationship(const QString &logicalRelationship)
{
    m_logicalRelationship = logicalRelationship;
}

QString DelayTriggerConditionClass::getTriggerValue() const
{
    return m_triggerValue;
}

void DelayTriggerConditionClass::setTriggerValue(const QString &triggerValue)
{
    m_triggerValue = triggerValue;
}
