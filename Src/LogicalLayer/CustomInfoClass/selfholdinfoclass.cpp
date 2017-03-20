#include "selfholdinfoclass.h"

SelfHoldInfoClass::SelfHoldInfoClass()
{
}

SelfHoldInfoClass::SelfHoldInfoClass(const QString &selfFlagId, const QString &selfHoldId,
                                     const int &selfHoldValue, const int &selfHoldConditionNum)
{
    m_selfFlagId = selfFlagId;
    m_selfHoldId = selfHoldId;
    m_selfHoldValue = selfHoldValue;
    m_selfHoldConditionNum = selfHoldConditionNum;
}

SelfHoldInfoClass::SelfHoldInfoClass(const SelfHoldInfoClass &other)
{
    this->m_selfFlagId = other.m_selfFlagId;
    this->m_selfHoldId = other.m_selfHoldId;
    this->m_selfHoldValue = other.m_selfHoldValue;
    this->m_selfHoldConditionNum = other.m_selfHoldConditionNum;
}

SelfHoldInfoClass SelfHoldInfoClass::operator =(const SelfHoldInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_selfFlagId = other.m_selfFlagId;
    this->m_selfHoldId = other.m_selfHoldId;
    this->m_selfHoldValue = other.m_selfHoldValue;
    this->m_selfHoldConditionNum = other.m_selfHoldConditionNum;

    return *this;
}

bool SelfHoldInfoClass::operator ==(const SelfHoldInfoClass &other) const
{
    if (this->m_selfFlagId == other.m_selfFlagId)
        return true;

    return false;
}

bool SelfHoldInfoClass::operator <(const SelfHoldInfoClass &other) const
{
    if (this->m_selfFlagId < other.m_selfFlagId)
        return true;

    return false;
}

QString SelfHoldInfoClass::getSelfFlagId() const
{
    return m_selfFlagId;
}

void SelfHoldInfoClass::setSelfFlagId(const QString &selfFlagId)
{
    m_selfFlagId = selfFlagId;
}

QString SelfHoldInfoClass::getSelfHoldId() const
{
    return m_selfHoldId;
}

void SelfHoldInfoClass::setSelfHoldId(const QString &selfHoldId)
{
    m_selfHoldId = selfHoldId;
}

int SelfHoldInfoClass::getSelfHoldValue() const
{
    return m_selfHoldValue;
}

void SelfHoldInfoClass::setSelfHoldValue(const int &selfHoldValue)
{
    m_selfHoldValue = selfHoldValue;
}

int SelfHoldInfoClass::getSelfHoldConditionNum() const
{
    return m_selfHoldConditionNum;
}

void SelfHoldInfoClass::setSelfHoldConditionNum(const int &selfHoldConditionNum)
{
    m_selfHoldConditionNum = selfHoldConditionNum;
}
