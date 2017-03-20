#include "failurehistoryrecordclass.h"

FailureHistoryRecordClass::FailureHistoryRecordClass()
{
}

FailureHistoryRecordClass::FailureHistoryRecordClass(const QString &failureId,
                                                     const int &failureLv,
                                                     const QString &startTime,
                                                     const QString &endTime,
                                                     const QString &failureName)
{
    m_failureId = failureId;
    m_startTime = startTime;
    m_endTime = endTime;
    m_failureName = failureName;
    m_failurelv = failureLv;
}

FailureHistoryRecordClass::FailureHistoryRecordClass(const QString &failureId, const int &failureLv,
                                                     const QString &startTime,const QString &endTime,
                                                     const QString &failureName, const QString &currentData,
                                                     const int &timerHoldNum)
{
    m_failureId = failureId;
    m_startTime = startTime;
    m_endTime = endTime;
    m_failureName = failureName;
    m_failurelv = failureLv;
    m_currentRealData = currentData;
    m_timerHoldNum = timerHoldNum;
}

FailureHistoryRecordClass::FailureHistoryRecordClass(const FailureHistoryRecordClass &other)
{
    this->m_failureId = other.m_failureId;
    this->m_startTime = other.m_startTime;
    this->m_endTime = other.m_endTime;
    this->m_failureName = other.m_failureName;
    this->m_failurelv = other.m_failurelv;
    this->m_timerHoldNum = other.m_timerHoldNum;
    this->m_currentRealData = other.m_currentRealData;
}

FailureHistoryRecordClass FailureHistoryRecordClass::operator =(const FailureHistoryRecordClass &other)
{
    if ( this == &other )
        return *this;

    this->m_failureId = other.m_failureId;
    this->m_startTime = other.m_startTime;
    this->m_endTime = other.m_endTime;
    this->m_failureName = other.m_failureName;
    this->m_failurelv = other.m_failurelv;
    this->m_timerHoldNum = other.m_timerHoldNum;
    this->m_currentRealData = other.m_currentRealData;

    return *this;
}

bool FailureHistoryRecordClass::operator ==(const FailureHistoryRecordClass &other) const
{
    if (this->m_failureId == other.m_failureId)
        return true;

    return false;
}

bool FailureHistoryRecordClass::operator <(const FailureHistoryRecordClass &other) const
{
    if (this->m_failureId < other.m_failureId)
        return true;

    return false;
}

QString FailureHistoryRecordClass::getFailureId() const
{
    return m_failureId;
}

void FailureHistoryRecordClass::setFailureId(const QString &failureId)
{
    m_failureId = failureId;
}

QString FailureHistoryRecordClass::getStartTime() const
{
    return m_startTime;
}

void FailureHistoryRecordClass::setStartTimee(const QString &startTime)
{
    m_startTime = startTime;
}

QString FailureHistoryRecordClass::getEndTime() const
{
    return m_endTime;
}

void FailureHistoryRecordClass::setEndTime(const QString &endTime)
{
    m_endTime = endTime;
}

QString FailureHistoryRecordClass::getFailureName() const
{
    return m_failureName;
}

void FailureHistoryRecordClass::setFailureName(const QString &failureName)
{
    m_failureName = failureName;
}

int FailureHistoryRecordClass::getFailureLv() const
{
    return m_failurelv;
}

void FailureHistoryRecordClass::setFailureLv(const int &failureLv)
{
    m_failurelv = failureLv;
}

int FailureHistoryRecordClass::getTimerHoldNum() const
{
    return m_timerHoldNum;
}

void FailureHistoryRecordClass::setTimerHoldNum(const int timerHoldNum)
{
    m_timerHoldNum = timerHoldNum;
}

QString FailureHistoryRecordClass::getCurrentRealData() const
{
    return m_currentRealData;
}

void FailureHistoryRecordClass::setCurrentRealData(const QString &currentData)
{
    m_currentRealData = currentData;
}

