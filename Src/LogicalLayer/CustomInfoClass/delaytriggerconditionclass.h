#ifndef DELAYTRIGGERCONDITIONCLASS_H
#define DELAYTRIGGERCONDITIONCLASS_H

#include <QString>
#include <QList>

class DelayTriggerConditionClass
{
public:
    DelayTriggerConditionClass();

    DelayTriggerConditionClass( const QString &delayId, const QString &busId,
                                const QString &logicalRelationship, const QString &triggerValue );

    DelayTriggerConditionClass( const DelayTriggerConditionClass &other );

    DelayTriggerConditionClass operator = (const DelayTriggerConditionClass &other );

    bool operator == ( const DelayTriggerConditionClass &other ) const;

    bool operator < ( const DelayTriggerConditionClass &other ) const;

    QString getDelayId() const;
    void setDelayId(const QString &delayId);

    QString getBusId() const;
    void setBusId(const QString &busId);

    QString getLogicalRelationship() const;
    void setLogicalRelationship(const QString &logicalRelationship);

    QString getTriggerValue() const;
    void setTriggerValue(const QString &triggerValue);

private:
    QString m_delayId;
    QString m_busId;
    QString m_logicalRelationship;
    QString m_triggerValue;
};

typedef QList<DelayTriggerConditionClass> DelayTriggerConditionInfoList;

#endif // DELAYTRIGGERCONDITIONCLASS_H
