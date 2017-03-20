#ifndef DELAYINFOCLASS_H
#define DELAYINFOCLASS_H

#include <QString>
#include <QList>

class DelayInfoClass
{
public:
    DelayInfoClass();

    DelayInfoClass( const QString &delayId, const int &logicalRelationship, const int &delaySeconds,
                    const int &delayValue, const int &delayConditionNum, const bool &isStartTimer = false,
                    const int &conditionsHoldSeconds = 0);

    DelayInfoClass( const DelayInfoClass &other );

    DelayInfoClass operator = (const DelayInfoClass &other );

    bool operator == ( const DelayInfoClass &other ) const;

    bool operator < ( const DelayInfoClass &other ) const;

    QString getDelayId() const;
    void setDelayId(const QString &delayId);

    int getLogicalRelationship() const;
    void setLogicalRelationship(const int &logicalRelationship);

    int getDelaySeconds() const;
    void setDelaySeconds(const int &delaySeconds);

    int getDelayValue() const;
    void setDelayValue(const int &delayValue);

    int getDelayConditionNum() const;
    void setDelayConditionNum(const int &delayConditionNum);

    bool isStartTimer() const;
    void setIsStartTimer(const bool &isStartTimer);

    int getConditionsHoldSeconds() const;
    void setConditionsHoldSeconds(const int &conditionsHoldSeconds);

private:
    QString m_delayId;
    int m_logicalRelationship;
    int m_delaySeconds;
    int m_delayValue;
    int m_delayConditionNum;

    bool m_isStartTimer;
    int m_conditionsHoldSeconds;
};

typedef QList<DelayInfoClass> DelayInfoList;

#endif // DELAYINFOCLASS_H
