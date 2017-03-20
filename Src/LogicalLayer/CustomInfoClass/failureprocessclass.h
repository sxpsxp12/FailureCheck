#ifndef FAILUREPROCESSCLASS_H
#define FAILUREPROCESSCLASS_H

#include <QString>
#include <QList>

class FailureProcessClass
{
public:
    FailureProcessClass();

    FailureProcessClass( const QString &failureId, const QString &processPoint,
                    const QString &processMethod, const QString &imageName );

    FailureProcessClass( const FailureProcessClass &other );

    FailureProcessClass operator = (const FailureProcessClass &other );

    bool operator == ( const FailureProcessClass &other ) const;

    bool operator < ( const FailureProcessClass &other ) const;

    QString getFailureId() const;
    void setFailureId(const QString &failureId);

    QString getProcessPoint() const;
    void setProcessPoint(const QString &processPoint);

    QString getProcessMethod() const;
    void setProcessMethod(const QString &processMethod);

    QString getImageName() const;
    void setImageName(const QString &imageName);

private:
    QString m_failureId;
    QString m_processPoint;
    QString m_processMethod;
    QString m_imageName;
};

typedef QList<FailureProcessClass> FailureProcessInfoList;

#endif // FAILUREPROCESSCLASS_H
