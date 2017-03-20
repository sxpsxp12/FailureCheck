#ifndef SELFHOLDTRIGGERCONDITIONCLASS_H
#define SELFHOLDTRIGGERCONDITIONCLASS_H

#include <QString>
#include <QList>

class SelfHoldTriggerConditionClass
{
public:
    SelfHoldTriggerConditionClass();

    SelfHoldTriggerConditionClass( const QString &selfFlagId, const QString &busId,
                                   const QString &logicalRelationship, const QString &triggerValue );

    SelfHoldTriggerConditionClass( const SelfHoldTriggerConditionClass &other );

    SelfHoldTriggerConditionClass operator = (const SelfHoldTriggerConditionClass &other );

    bool operator == ( const SelfHoldTriggerConditionClass &other ) const;

    bool operator < ( const SelfHoldTriggerConditionClass &other ) const;

    QString getSelfFlagId() const;
    void setSelfFlagId(const QString &selfFlagId);

    QString getBusId() const;
    void setBusId(const QString &busId);

    QString getLogicalRelationship() const;
    void setLogicalRelationship(const QString &logicalRelationship);

    QString getTriggerValue() const;
    void setTriggerValue(const QString &triggerValue);

private:
    QString m_selfFlagId;
    QString m_busId;
    QString m_logicalRelationship;
    QString m_triggerValue;
};

typedef QList<SelfHoldTriggerConditionClass> SelfHoldTriggerConditionInfoList;

#endif // SELFHOLDTRIGGERCONDITIONCLASS_H
