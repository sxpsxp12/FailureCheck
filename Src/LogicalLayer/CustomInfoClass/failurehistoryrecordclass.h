#ifndef FAILUREHISTORYRECORDCLASS_H
#define FAILUREHISTORYRECORDCLASS_H

#include <QString>
#include <QList>
#include <QStringList>

class FailureHistoryRecordClass
{
public:
    FailureHistoryRecordClass();

    FailureHistoryRecordClass( const QString &failureId, const int &failureLv,
                               const QString &startTime, const QString &endTime,
                               const QString &failureName);

    FailureHistoryRecordClass( const QString &failureId, const int &failureLv,
                               const QString &startTime, const QString &endTime,
                               const QString &failureName,const QString &currentData,
                               const int &timerHoldNum=0);

    FailureHistoryRecordClass( const FailureHistoryRecordClass &other );

    FailureHistoryRecordClass operator = (const FailureHistoryRecordClass &other );

    bool operator == ( const FailureHistoryRecordClass &other ) const;

    bool operator < ( const FailureHistoryRecordClass &other ) const;

    QString getFailureId() const;
    void setFailureId(const QString &failureId);

    QString getStartTime() const;
    void setStartTimee(const QString &startTime);

    QString getEndTime() const;
    void setEndTime(const QString &endTime);

    QString getFailureName() const;
    void setFailureName(const QString &failureName);

    int getFailureLv() const;
    void setFailureLv(const int &failureLv);

    int getTimerHoldNum() const;
    void setTimerHoldNum(const int timerHoldNum);

    QString getCurrentRealData() const;
    void setCurrentRealData(const QString &currentData);

private:
    QString m_failureId;
    QString m_startTime;
    QString m_endTime;
    int m_failurelv;
    QString m_failureName;

    QString m_currentRealData;  //故障发生时的实时数据
    int m_timerHoldNum;         //定时器保持次数
};

typedef QList<FailureHistoryRecordClass> FailureHistoryRecordInfoList;

#endif // FAILUREHISTORYRECORDCLASS_H
