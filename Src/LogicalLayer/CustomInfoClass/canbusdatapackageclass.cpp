#include "canbusdatapackageclass.h"

#include <QDebug>

CanBusDataPackageClass::CanBusDataPackageClass()
{
}

CanBusDataPackageClass::CanBusDataPackageClass(const QString &canBusDataIdStr, const QString &canBusByte)
{
    int mBusByteTemp;
    int mBusByte;

    m_CanBusDataIdStr = canBusDataIdStr;
    mBusByteTemp = canBusByte.toInt(0, 16);
    mBusByte = canBusByte.toInt(0, 16);

    for (int i = 0; i < 8; i++)
    {
        m_bitAddress[i] = mBusByteTemp % 2;
        m_bitAddressStr[i] = QString::number(mBusByteTemp % 2);
        mBusByteTemp /= 2;
    }

    m_decLowBitNumber = mBusByte % 16;
    m_decLowBitNumberStr = QString::number(m_decLowBitNumber);
    m_hexLowBitNumberStr = QString::number(m_decLowBitNumber, 16);

    m_decNumber = mBusByte;
    m_hexNumberStr = canBusByte;
    m_decNumberStr = QString::number(m_decNumber);
}

CanBusDataPackageClass::CanBusDataPackageClass(const CanBusDataPackageClass &other)
{
    for (int i = 0; i < 8; i++)
    {
        this->m_bitAddress[i] = other.m_bitAddress[i];
        this->m_bitAddressStr[i] = other.m_bitAddressStr[i];
    }
    this->m_CanBusDataIdStr = other.m_CanBusDataIdStr;
    this->m_decLowBitNumber = other.m_decLowBitNumber;
    this->m_decLowBitNumberStr = other.m_decLowBitNumberStr;
    this->m_decNumber = other.m_decNumber;
    this->m_decNumberStr = other.m_decNumberStr;
    this->m_hexLowBitNumberStr = other.m_hexLowBitNumberStr;
    this->m_hexNumberStr = other.m_hexNumberStr;
}

CanBusDataPackageClass CanBusDataPackageClass::operator =(const CanBusDataPackageClass &other)
{
    if (this == &other)
        return *this;

    for (int i = 0; i < 8; i++)
    {
        this->m_bitAddress[i] = other.m_bitAddress[i];
        this->m_bitAddressStr[i] = other.m_bitAddressStr[i];
    }
    this->m_CanBusDataIdStr = other.m_CanBusDataIdStr;
    this->m_decLowBitNumber = other.m_decLowBitNumber;
    this->m_decLowBitNumberStr = other.m_decLowBitNumberStr;
    this->m_decNumber = other.m_decNumber;
    this->m_decNumberStr = other.m_decNumberStr;
    this->m_hexLowBitNumberStr = other.m_hexLowBitNumberStr;
    this->m_hexNumberStr = other.m_hexNumberStr;

    return *this;
}

bool CanBusDataPackageClass::operator ==(const CanBusDataPackageClass &other) const
{
    if (this->m_decNumber == other.m_decNumber)
        return true;

    return false;
}

bool CanBusDataPackageClass::operator <(const CanBusDataPackageClass &other) const
{
    if (this->m_decNumber < other.m_decNumber)
        return true;

    return false;
}

int CanBusDataPackageClass::getBitAddress(const int &bit) const
{
    return m_bitAddress[bit];
}

QString CanBusDataPackageClass::getBitAddressStr(const int &bit) const
{
    return m_bitAddressStr[bit];
}

void CanBusDataPackageClass::setDecLowBitNumber(const int &decLowBitNumber)
{
    m_decLowBitNumber = decLowBitNumber;
}

int CanBusDataPackageClass::getDecLowBitNumber() const
{
    return m_decLowBitNumber;
}

void CanBusDataPackageClass::setDecLowBitNumberStr(const QString &decLowBitNumberStr)
{
    m_decLowBitNumberStr = decLowBitNumberStr;
}

QString CanBusDataPackageClass::getDecLowBitNumberStr() const
{
    return m_decLowBitNumberStr;
}

void CanBusDataPackageClass::setDecNumber(const int &decNumber)
{
    m_decNumber = decNumber;
}

int CanBusDataPackageClass::getDecNumber() const
{
    return m_decNumber;
}

void CanBusDataPackageClass::setDecNumberStr(const QString &decNumberStr)
{
    m_decNumberStr = decNumberStr;
}

QString CanBusDataPackageClass::getDecNumberStr() const
{
    return m_decNumberStr;
}

void CanBusDataPackageClass::setHexLowBitNumberStr(const QString &hexLowBitNumberStr)
{
    m_hexLowBitNumberStr = hexLowBitNumberStr;
}

QString CanBusDataPackageClass::getHexLowBitNumberStr() const
{
    return m_hexLowBitNumberStr;
}

void CanBusDataPackageClass::setHexNumberStr(const QString &hexNumberStr)
{
    m_hexNumberStr = hexNumberStr;
}

QString CanBusDataPackageClass::getHexNumberStr() const
{
    return m_hexNumberStr;
}

void CanBusDataPackageClass::setCanBusDataIDStr(const QString &canBusDataIdStr)
{
    m_CanBusDataIdStr = canBusDataIdStr;
}

QString CanBusDataPackageClass::getCanBusDataIDStr() const
{
    return m_CanBusDataIdStr;
}
