#ifndef DEVICESTATETRIGGERCONDITIONCLASS_H
#define DEVICESTATETRIGGERCONDITIONCLASS_H

#include <QString>
#include <QList>

class DeviceStateTriggerConditionClass
{
public:
    DeviceStateTriggerConditionClass();

    DeviceStateTriggerConditionClass( const QString &deviceStateId, const QString &busId,
                                      const int &logicalRelationship, const double &triggerValue );

    DeviceStateTriggerConditionClass( const DeviceStateTriggerConditionClass &other );

    DeviceStateTriggerConditionClass operator = ( const DeviceStateTriggerConditionClass &other );

    bool operator == ( const DeviceStateTriggerConditionClass &other ) const;

    bool operator < ( const DeviceStateTriggerConditionClass &other ) const;

    QString getDeviceStateId() const;
    void setDeviceStateId(const QString &deviceStateId);

    QString getBusId() const;
    void setBusId(const QString &busId);

    int getLogicalRelationship() const;
    void setLogicalRelationship(const int &logicalRelationship);

    double getTriggerValue() const;
    void setTriggerValue(const double &triggerValue);

private:
    QString m_deviceStateId;
    QString m_busId;
    int m_logicalRelationship;
    double m_triggerValue;
};

typedef QList<DeviceStateTriggerConditionClass> DeviceStateTriggerConditionInfoList;

#endif // DEVICESTATETRIGGERCONDITIONCLASS_H
