#include "failureprocessclass.h"

FailureProcessClass::FailureProcessClass()
{
}

FailureProcessClass::FailureProcessClass(const QString &failureId, const QString &processPoint,
                               const QString &processMethod, const QString &imageName)
{
    m_failureId = failureId;
    m_processPoint = processPoint;
    m_processMethod = processMethod;
    m_imageName = imageName;
}

FailureProcessClass::FailureProcessClass(const FailureProcessClass &other)
{
    this->m_failureId = other.m_failureId;
    this->m_processPoint = other.m_processPoint;
    this->m_processMethod = other.m_processMethod;
    this->m_imageName = other.m_imageName;
}

FailureProcessClass FailureProcessClass::operator =(const FailureProcessClass &other)
{
    if ( this == &other )
        return *this;

    this->m_failureId = other.m_failureId;
    this->m_processPoint = other.m_processPoint;
    this->m_processMethod = other.m_processMethod;
    this->m_imageName = other.m_imageName;

    return *this;
}

bool FailureProcessClass::operator ==(const FailureProcessClass &other) const
{
    if ((this->m_failureId == other.m_failureId) &&
        (this->m_processPoint == other.m_processPoint))
        return true;

    return false;
}

bool FailureProcessClass::operator <(const FailureProcessClass &other) const
{
    if (this->m_failureId < other.m_failureId)
        return true;

    return false;
}

QString FailureProcessClass::getFailureId() const
{
    return m_failureId;
}

void FailureProcessClass::setFailureId(const QString &failureId)
{
    m_failureId = failureId;
}

QString FailureProcessClass::getProcessPoint() const
{
    return m_processPoint;
}

void FailureProcessClass::setProcessPoint(const QString &processPoint)
{
    m_processPoint = processPoint;
}

QString FailureProcessClass::getProcessMethod() const
{
    return m_processMethod;
}

void FailureProcessClass::setProcessMethod(const QString &processMethod)
{
    m_processMethod = processMethod;
}

QString FailureProcessClass::getImageName() const
{
    return m_imageName;
}

void FailureProcessClass::setImageName(const QString &imageName)
{
    m_imageName = imageName;
}
