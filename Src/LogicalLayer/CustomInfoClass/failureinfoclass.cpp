#include "failureinfoclass.h"
#include "enumtypeheader.h"

FailureInfoClass::FailureInfoClass(const QString &failureId, const QString &failureName, const int &failureTriggerConditionNum, const int &failureProcessMethodConditionNum, const int &failureLv, const QString &outputBusId)
{
    m_failureId = failureId;
    m_failureName = failureName;
    m_failureTriggerConditionNum = failureTriggerConditionNum;
    m_failureProcessMethodConditionNum = failureProcessMethodConditionNum;
    m_failureLv = failureLv;
    m_outputBusId = outputBusId;
}

FailureInfoClass::FailureInfoClass(const QString &failureId, const QString &failureName,
                                   const int &failureTriggerConditionNum, const int &failureProcessMethodConditionNum,
                                   const int &failureLv, const QString &outputBusId,const QDateTime &failureOccurTime,
                                   const int &failureDurationNum)
{
    m_failureId = failureId;
    m_failureName = failureName;
    m_failureTriggerConditionNum = failureTriggerConditionNum;
    m_failureProcessMethodConditionNum = failureProcessMethodConditionNum;
    m_failureLv = failureLv;
    m_outputBusId = outputBusId;
    m_failureDurationNum = failureDurationNum;
    m_failureOccurTime = failureOccurTime;
}

FailureInfoClass::FailureInfoClass(const QString &failureId, const QString &failureName, const int &failureLv)
{
    m_failureId = failureId;
    m_failureName = failureName;
    m_failureLv = failureLv;
}

FailureInfoClass::FailureInfoClass(const FailureInfoClass &other)
{
    this->m_failureId = other.m_failureId;
    this->m_failureName = other.m_failureName;
    this->m_failureTriggerConditionNum = other.m_failureTriggerConditionNum;
    this->m_failureProcessMethodConditionNum = other.m_failureProcessMethodConditionNum;
    this->m_failureLv = other.m_failureLv;
    this->m_outputBusId = other.m_outputBusId;
    this->m_failureDurationNum = other.m_failureDurationNum;
    this->m_currentRealData = other.m_currentRealData;
    this->m_failureOccurTime = other.m_failureOccurTime;
}

FailureInfoClass FailureInfoClass::operator =(const FailureInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_failureId = other.m_failureId;
    this->m_failureName = other.m_failureName;
    this->m_failureTriggerConditionNum = other.m_failureTriggerConditionNum;
    this->m_failureProcessMethodConditionNum = other.m_failureProcessMethodConditionNum;
    this->m_failureLv = other.m_failureLv;
    this->m_outputBusId = other.m_outputBusId;
    this->m_failureDurationNum = other.m_failureDurationNum;
    this->m_failureOccurTime = other.m_failureOccurTime;
    this->m_currentRealData = other.m_currentRealData;
    return *this;
}

bool FailureInfoClass::operator ==(const FailureInfoClass &other) const
{
    if (this->m_failureId == other.m_failureId)
        return true;

    return false;
}

bool FailureInfoClass::operator<(const FailureInfoClass &other) const
{
    if (this->m_failureLv < other.m_failureLv)
        return true;
    return false;
}

QString FailureInfoClass::getFailureId() const
{
    return m_failureId;
}

void FailureInfoClass::setFailureId(const QString &failureId)
{
    m_failureId = failureId;
}

QString FailureInfoClass::getFailureName() const
{
    return m_failureName;
}

void FailureInfoClass::setFailureName(const QString &failureName)
{
    m_failureName = failureName;
}

int FailureInfoClass::getFailureTriggerConditionNum() const
{
    return m_failureTriggerConditionNum;
}

void FailureInfoClass::setFailureTriggerConditionNum(const int &failureTriggerConditionNum)
{
    m_failureTriggerConditionNum = failureTriggerConditionNum;
}

int FailureInfoClass::getFailureProcessMethodConditionNum() const
{
    return m_failureProcessMethodConditionNum;
}

void FailureInfoClass::setFailureProcessMethodConditionNum(const int &failureProcessMethodConditionNum)
{
    m_failureProcessMethodConditionNum = failureProcessMethodConditionNum;
}

int FailureInfoClass::getFailureLv() const
{
    return m_failureLv;
}

void FailureInfoClass::setFailureLv(const int &failureLv)
{
    m_failureLv = failureLv;
}

QString FailureInfoClass::getOutputBusId() const
{
    return m_outputBusId;
}

void FailureInfoClass::setOutputBusId(const QString &outputBusId)
{
    m_outputBusId = outputBusId;
}

int FailureInfoClass::getFailureDurationNum() const
{
    return m_failureDurationNum;
}

void FailureInfoClass::setFailureDurationNum(const int &failureDurationNum)
{
    m_failureDurationNum = failureDurationNum;
}

QDateTime FailureInfoClass::getFailureOccurTime() const
{
    return m_failureOccurTime;
}

void FailureInfoClass::setFailureOccurTime(const QDateTime failureOccurTime)
{
    m_failureOccurTime = failureOccurTime;
}

QString FailureInfoClass::getCurrentRealData() const
{
    return m_currentRealData;
}

void FailureInfoClass::setCurrentRealData(const QString &currentData)
{
    m_currentRealData = currentData;
}

bool FailureInfoClass::failureInfoListSort(const FailureInfoClass &failureInfoFirst, const FailureInfoClass &failureInfoSecond)
{
    //按等级的从大到小排
    return failureInfoSecond < failureInfoFirst;
}

//提示的故障的排序规则
bool FailureInfoClass::failureInfoListPromptSort(const FailureInfoClass &failureInfoFirst, const FailureInfoClass &failureInfoSecond)
{
    //按时间先后排序,时间大的排到前面
    if(failureInfoFirst.getFailureOccurTime().secsTo(failureInfoSecond.getFailureOccurTime()) > 0)
        return true;
    return false;
}
