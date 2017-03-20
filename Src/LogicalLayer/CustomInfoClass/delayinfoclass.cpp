#include "delayinfoclass.h"

DelayInfoClass::DelayInfoClass()
{
}

DelayInfoClass::DelayInfoClass(const QString &delayId, const int &logicalRelationship, const int &delaySeconds,
                               const int &delayValue, const int &delayConditionNum, const bool &isStartTimer,
                               const int &conditionsHoldSeconds)
{
    m_delayId = delayId;
    m_logicalRelationship = logicalRelationship;
    m_delaySeconds = delaySeconds;
    m_delayValue = delayValue;
    m_delayConditionNum = delayConditionNum;
    m_isStartTimer = isStartTimer;
    m_conditionsHoldSeconds = conditionsHoldSeconds;
}

DelayInfoClass::DelayInfoClass(const DelayInfoClass &other)
{
    this->m_delayId = other.m_delayId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_delaySeconds = other.m_delaySeconds;
    this->m_delayValue = other.m_delayValue;
    this->m_delayConditionNum = other.m_delayConditionNum;
    this->m_isStartTimer = other.m_isStartTimer;
    this->m_conditionsHoldSeconds = other.m_conditionsHoldSeconds;
}

DelayInfoClass DelayInfoClass::operator =(const DelayInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_delayId = other.m_delayId;
    this->m_logicalRelationship = other.m_logicalRelationship;
    this->m_delaySeconds = other.m_delaySeconds;
    this->m_delayValue = other.m_delayValue;
    this->m_delayConditionNum = other.m_delayConditionNum;
    this->m_isStartTimer = other.m_isStartTimer;
    this->m_conditionsHoldSeconds = other.m_conditionsHoldSeconds;

    return *this;
}

bool DelayInfoClass::operator ==(const DelayInfoClass &other) const
{
    if (this->m_delayId == other.m_delayId)
        return true;

    return false;
}

bool DelayInfoClass::operator <(const DelayInfoClass &other) const
{
    if (this->m_delayId < other.m_delayId)
        return true;

    return false;
}

QString DelayInfoClass::getDelayId() const
{
    return m_delayId;
}

void DelayInfoClass::setDelayId(const QString &delayId)
{
    m_delayId = delayId;
}

int DelayInfoClass::getLogicalRelationship() const
{
    return m_logicalRelationship;
}

void DelayInfoClass::setLogicalRelationship(const int &logicalRelationship)
{
    m_logicalRelationship = logicalRelationship;
}

int DelayInfoClass::getDelaySeconds() const
{
    return m_delaySeconds;
}

void DelayInfoClass::setDelaySeconds(const int &delaySeconds)
{
    m_delaySeconds = delaySeconds;
}

int DelayInfoClass::getDelayConditionNum() const
{
    return m_delayConditionNum;
}

void DelayInfoClass::setDelayConditionNum(const int &delayConditionNum)
{
    m_delayConditionNum = delayConditionNum;
}

bool DelayInfoClass::isStartTimer() const
{
    return m_isStartTimer;
}

void DelayInfoClass::setIsStartTimer(const bool &isStartTimer)
{
    m_isStartTimer = isStartTimer;
}

int DelayInfoClass::getConditionsHoldSeconds() const
{
    return m_conditionsHoldSeconds;
}

void DelayInfoClass::setConditionsHoldSeconds(const int &conditionsHoldSeconds)
{
    if (conditionsHoldSeconds <= m_delaySeconds+1)
    {
        m_conditionsHoldSeconds = conditionsHoldSeconds;
    }
}

int DelayInfoClass::getDelayValue() const
{
    return m_delayValue;
}

void DelayInfoClass::setDelayValue(const int &delayValue)
{
    m_delayValue = delayValue;
}
