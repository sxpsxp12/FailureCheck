#include "selfholdtriggerconditionclass.h"

SelfHoldTriggerConditionClass::SelfHoldTriggerConditionClass()
{
}

SelfHoldTriggerConditionClass::SelfHoldTriggerConditionClass(const QString &selfFlagId, const QString &busId,
                                                             const QString &logicalRelationship, const QString &triggerValue)
{
    m_selfFlagId = selfFlagId;
    m_busId = busId;
    m_logicalRelationship = logicalRelationship;
    m_triggerValue = triggerValue;
}

SelfHoldTriggerConditionClass::SelfHoldTriggerConditionClass(const SelfHoldTriggerConditionClass &other)
{
    this->m_selfFlagId = other.m_selfFlagId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;
}

SelfHoldTriggerConditionClass SelfHoldTriggerConditionClass::operator =(const SelfHoldTriggerConditionClass &other)
{
    if ( this == &other )
        return *this;

    this->m_selfFlagId = other.m_selfFlagId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;

    return *this;
}

bool SelfHoldTriggerConditionClass::operator ==(const SelfHoldTriggerConditionClass &other) const
{
    if (this->m_selfFlagId == other.m_selfFlagId)
        return true;

    return false;
}

bool SelfHoldTriggerConditionClass::operator <(const SelfHoldTriggerConditionClass &other) const
{
    if (this->m_selfFlagId < other.m_selfFlagId)
        return true;

    return false;
}

QString SelfHoldTriggerConditionClass::getSelfFlagId() const
{
    return m_selfFlagId;
}

void SelfHoldTriggerConditionClass::setSelfFlagId(const QString &selfFlagId)
{
    m_selfFlagId = selfFlagId;
}

QString SelfHoldTriggerConditionClass::getBusId() const
{
    return m_busId;
}

void SelfHoldTriggerConditionClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

QString SelfHoldTriggerConditionClass::getLogicalRelationship() const
{
    return m_logicalRelationship;
}

void SelfHoldTriggerConditionClass::setLogicalRelationship(const QString &logicalRelationship)
{
    m_logicalRelationship = logicalRelationship;
}

QString SelfHoldTriggerConditionClass::getTriggerValue() const
{
    return m_triggerValue;
}

void SelfHoldTriggerConditionClass::setTriggerValue(const QString &triggerValue)
{
    m_triggerValue = triggerValue;
}
