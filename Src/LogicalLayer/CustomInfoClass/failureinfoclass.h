#ifndef FAILUREINFOCLASS_H
#define FAILUREINFOCLASS_H

#include <QString>
#include <QList>
#include <QDateTime>

class FailureInfoClass
{
public:
    FailureInfoClass() {}

    FailureInfoClass( const QString &failureId, const QString &failureName,
                      const int &failureTriggerConditionNum, const int &failureProcessMethodConditionNum,
                      const int &failureLv, const QString &outputBusId);

    FailureInfoClass( const QString &failureId, const QString &failureName,
                      const int &failureTriggerConditionNum, const int &failureProcessMethodConditionNum,
                      const int &failureLv, const QString &outputBusId,
                      const QDateTime &failureOccurTime,
                      const int &failureDurationNum = 0);

    FailureInfoClass( const QString &failureId, const QString &failureName, const int &failureLv);

    FailureInfoClass( const FailureInfoClass &other );

    FailureInfoClass operator = (const FailureInfoClass &other );

    bool operator == ( const FailureInfoClass &other ) const;

    bool operator < ( const FailureInfoClass &other ) const;

    QString getFailureId() const;
    void setFailureId(const QString &failureId);

    QString getFailureName() const;
    void setFailureName(const QString &failureName);

    int getFailureTriggerConditionNum() const;
    void setFailureTriggerConditionNum(const int &failureTriggerConditionNum);

    int getFailureProcessMethodConditionNum() const;
    void setFailureProcessMethodConditionNum(const int &failureProcessMethodConditionNum);

    int getFailureLv() const;
    void setFailureLv(const int &failureLv);

    QString getOutputBusId() const;
    void setOutputBusId(const QString &outputBusId);

    int getFailureDurationNum() const;
    void setFailureDurationNum(const int &failureDurationNum);

    QDateTime getFailureOccurTime() const;
    void setFailureOccurTime(const QDateTime failureOccurTime);

    QString getCurrentRealData() const;
    void setCurrentRealData(const QString &currentData);

    //排序规则
    static bool failureInfoListSort(const FailureInfoClass &failureInfoFirst,const FailureInfoClass &failureInfoSecond);
    //提示的故障的排序规则
    static bool failureInfoListPromptSort(const FailureInfoClass &failureInfoFirst,const FailureInfoClass &failureInfoSecond);

private:
    QString m_failureId;
    QString m_failureName;
    int m_failureTriggerConditionNum;
    int m_failureProcessMethodConditionNum;
    int m_failureLv;
    QString m_outputBusId;

    QString m_currentRealData;      //当前的实时数据
    int m_failureDurationNum;  //   故障发生的持续次数
    QDateTime m_failureOccurTime;     //故障发生时间
};

typedef QList<FailureInfoClass> FailureInfoList;

#endif // FAILUREINFOCLASS_H
