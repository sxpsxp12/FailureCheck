#ifndef SELFHOLDINFOCLASS_H
#define SELFHOLDINFOCLASS_H

#include <QString>
#include <QList>

class SelfHoldInfoClass
{
public:
    SelfHoldInfoClass();

    SelfHoldInfoClass( const QString &selfFlagId, const QString &selfHoldId,
                       const int &selfHoldValue, const int &selfHoldConditionNum );

    SelfHoldInfoClass( const SelfHoldInfoClass &other );

    SelfHoldInfoClass operator = (const SelfHoldInfoClass &other );

    bool operator == ( const SelfHoldInfoClass &other ) const;

    bool operator < ( const SelfHoldInfoClass &other ) const;

    QString getSelfFlagId() const;
    void setSelfFlagId(const QString &selfFlagId);

    QString getSelfHoldId() const;
    void setSelfHoldId(const QString &selfHoldId);

    int getSelfHoldValue() const;
    void setSelfHoldValue(const int &selfHoldValue);

    int getSelfHoldConditionNum() const;
    void setSelfHoldConditionNum(const int &selfHoldConditionNum);

private:
    QString m_selfFlagId;
    QString m_selfHoldId;
    int m_selfHoldValue;
    int m_selfHoldConditionNum;
};

typedef QList<SelfHoldInfoClass> SelfHoldInfoList;

#endif // SELFHOLDINFOCLASS_H
