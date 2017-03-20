#include "realdatarecordclass.h"
#include <QDateTime>

RealDataRecordClass::RealDataRecordClass()
{
    m_frontQStringTimeAndRealDataList.clear();
    m_AfterQStringTimeAndRealDataList.clear();
    m_failureHistoryInfoList.clear();
}

RealDataRecordClass::RealDataRecordClass(const RealDataRecordClass &other)
{
    this->m_frontQStringTimeAndRealDataList = other.m_frontQStringTimeAndRealDataList;
    this->m_failureHistoryInfoList = other.m_failureHistoryInfoList;
    this->m_AfterQStringTimeAndRealDataList = other.m_AfterQStringTimeAndRealDataList;
}

bool RealDataRecordClass::operator ==(const RealDataRecordClass &other) const
{
    foreach (FailureHistoryRecordClass failureHistoryRecord, this->m_failureHistoryInfoList) {
        if(!( other.m_failureHistoryInfoList.contains(failureHistoryRecord) &&
              other.m_failureHistoryInfoList.at(other.m_failureHistoryInfoList.indexOf(failureHistoryRecord)).getStartTime().compare(failureHistoryRecord.getStartTime()) == 0
             )
          )
        {
            return false;
        }
    }

    foreach (FailureHistoryRecordClass failureHistoryRecord, other.m_failureHistoryInfoList) {
        if( !(this->m_failureHistoryInfoList.contains(failureHistoryRecord) &&
              this->m_failureHistoryInfoList.at(this->m_failureHistoryInfoList.indexOf(failureHistoryRecord)).getStartTime().compare(failureHistoryRecord.getStartTime()) == 0
              )
          )
        {
            return false;
        }
    }
    return true;
}

FailureHistoryRecordInfoList RealDataRecordClass::getfailureHistoryInfoList() const
{
    return m_failureHistoryInfoList;
}

void RealDataRecordClass::setFailureHistoryInfoList(const FailureHistoryRecordInfoList &failureHistoryInfoList)
{
    m_failureHistoryInfoList = failureHistoryInfoList;
}

QStringList RealDataRecordClass::getFrontQStringTimeAndRealDataList() const
{
    return m_frontQStringTimeAndRealDataList;
}

void RealDataRecordClass::setFrontQStringTimeAndRealDataList(const QStringList &frontQStringTimeAndRealDataList)
{
    m_frontQStringTimeAndRealDataList = frontQStringTimeAndRealDataList;
}

QStringList RealDataRecordClass::getAfteQStringTimeAndRealDataList() const
{
    return m_AfterQStringTimeAndRealDataList;
}

void RealDataRecordClass::setAfterQStringTimeAndRealDataList(const QStringList &afterDateTimeAndRealDataList)
{
    m_AfterQStringTimeAndRealDataList = afterDateTimeAndRealDataList;
}

RealDataRecordClass RealDataRecordClass::operator =(const RealDataRecordClass &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->m_failureHistoryInfoList = other.m_failureHistoryInfoList;
    this->m_frontQStringTimeAndRealDataList = other.m_frontQStringTimeAndRealDataList;
    this->m_AfterQStringTimeAndRealDataList = other.m_AfterQStringTimeAndRealDataList;

    return *this;
}
