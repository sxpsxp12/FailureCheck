#include "failuretriggerconditionclass.h"

FailureTriggerConditionClass::FailureTriggerConditionClass()
{
}

FailureTriggerConditionClass::FailureTriggerConditionClass(const QString &failureId, const QString &busId,
                                                 const QString &logicalRelationship, const QString &triggerValue)
{
    m_failureId = failureId;
    m_busId = busId;
    m_logicalRelationship = logicalRelationship;
    m_triggerValue = triggerValue;
}

FailureTriggerConditionClass::FailureTriggerConditionClass(const FailureTriggerConditionClass &other)
{
    this->m_failureId = other.m_failureId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;
}

FailureTriggerConditionClass FailureTriggerConditionClass::operator =(const FailureTriggerConditionClass &other)
{
    if ( this == &other )
        return *this;

    this->m_failureId = other.m_failureId;
    this->m_busId = other.m_busId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_triggerValue = other.m_triggerValue;

    return *this;
}

bool FailureTriggerConditionClass::operator ==(const FailureTriggerConditionClass &other) const
{
    if (this->m_failureId == other.m_failureId)
        return true;

    return false;
}

bool FailureTriggerConditionClass::operator <(const FailureTriggerConditionClass &other) const
{
    if (this->m_failureId < other.m_failureId)
        return true;

    return false;
}

QString FailureTriggerConditionClass::getFailureId() const
{
    return m_failureId;
}

void FailureTriggerConditionClass::setFailureId(const QString &failureId)
{
    m_failureId = failureId;
}

QString FailureTriggerConditionClass::getBusId() const
{
    return m_busId;
}

void FailureTriggerConditionClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

QString FailureTriggerConditionClass::getLogicalRelationship() const
{
    return m_logicalRelationship;
}

void FailureTriggerConditionClass::setLogicalRelationship(const QString &logicalRelationship)
{
    m_logicalRelationship = logicalRelationship;
}

QString FailureTriggerConditionClass::getTriggerValue() const
{
    return m_triggerValue;
}

void FailureTriggerConditionClass::setTriggerValue(const QString &triggerValue)
{
    m_triggerValue = triggerValue;
}
