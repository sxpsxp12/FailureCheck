#ifndef FAILURETRIGGERCONDITIONCLASS_H
#define FAILURETRIGGERCONDITIONCLASS_H

#include <QString>
#include <QList>

class FailureTriggerConditionClass
{
public:
    FailureTriggerConditionClass();

    FailureTriggerConditionClass( const QString &failureId, const QString &busId,
                           const QString &logicalRelationship, const QString &triggerValue );

    FailureTriggerConditionClass( const FailureTriggerConditionClass &other );

    FailureTriggerConditionClass operator = (const FailureTriggerConditionClass &other );

    bool operator == ( const FailureTriggerConditionClass &other ) const;

    bool operator < ( const FailureTriggerConditionClass &other ) const;

    QString getFailureId() const;
    void setFailureId(const QString &failureId);

    QString getBusId() const;
    void setBusId(const QString &busId);

    QString getLogicalRelationship() const;
    void setLogicalRelationship(const QString &logicalRelationship);

    QString getTriggerValue() const;
    void setTriggerValue(const QString &triggerValue);

private:
    QString m_failureId;
    QString m_busId;
    QString m_logicalRelationship;
    QString m_triggerValue;
};

typedef QList<FailureTriggerConditionClass> FailureTriggerConditionInfoList;

#endif // FAILURETRIGGERCONDITIONCLASS_H
