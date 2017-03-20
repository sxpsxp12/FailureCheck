#ifndef DEVICESTATEINFOCLASS_H
#define DEVICESTATEINFOCLASS_H

#include <QString>
#include <QList>

class DeviceStateInfoClass
{
public:
    DeviceStateInfoClass();

    DeviceStateInfoClass( const QString &deviceStateId, const QString &busId,
                          const int &color, const QString &imageName,
                          const QString &deviceStateDesc, const int &conditionNum);

    DeviceStateInfoClass( const DeviceStateInfoClass &other );

    DeviceStateInfoClass operator = (const DeviceStateInfoClass &other );

    bool operator == ( const DeviceStateInfoClass &other ) const;

    bool operator < ( const DeviceStateInfoClass &other ) const;

    QString getDeviceStateId() const;
    void setDeviceStateId(const QString &deviceStateId);

    QString getBusId() const;
    void setBusId(const QString &busId);

    int getColor() const;
    void setColor(const int &color);

    QString getImageName() const;
    void setImageName(const QString &imageName);

    QString getDeviceStateDesc() const;
    void setDeviceStateDesc(const QString &deviceStateDesc);

    int getConditionNum() const;
    void setConditionNum(const int &conditionNum);

private:
    QString m_deviceStateId;
    QString m_busId;
    int m_color;
    QString m_imageName;
    QString m_deviceStateDesc;
    int m_conditionNum;
};

typedef QList<DeviceStateInfoClass> DeviceStateInfoList;

#endif // DEVICESTATEINFOCLASS_H
