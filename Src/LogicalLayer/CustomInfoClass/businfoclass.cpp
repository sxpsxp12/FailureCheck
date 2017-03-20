#include "businfoclass.h"

BusInfoClass::BusInfoClass(const QString &busId, const int &byteAddrH,
                           const int &byteAddrL, const int &bitAddr,
                           const int &formulaTemplateId, const QString &formulaParameter,
                           const int &addrType)
{
    m_busId = busId;
    m_byteAddrH = byteAddrH;
    m_byteAddrL = byteAddrL;
    m_bitAddr = bitAddr;
    m_formulaTemplateId = formulaTemplateId;
    m_formulaParameter = formulaParameter;
    m_addrType = addrType;
}

BusInfoClass::BusInfoClass(const BusInfoClass &other)
{
    this->m_busId = other.m_busId;
    this->m_byteAddrH = other.m_byteAddrH;
    this->m_byteAddrL = other.m_byteAddrL;
    this->m_bitAddr = other.m_bitAddr;
    this->m_formulaTemplateId = other.m_formulaTemplateId;
    this->m_formulaParameter = other.m_formulaParameter;
    this->m_addrType = other.m_addrType;
}

BusInfoClass BusInfoClass::operator =(const BusInfoClass &other)
{
    if ( this == &other )
        return *this;

    this->m_busId = other.m_busId;
    this->m_byteAddrH = other.m_byteAddrH;
    this->m_byteAddrL = other.m_byteAddrL;
    this->m_bitAddr = other.m_bitAddr;
    this->m_formulaTemplateId = other.m_formulaTemplateId;
    this->m_formulaParameter = other.m_formulaParameter;
    this->m_addrType = other.m_addrType;

    return *this;
}

bool BusInfoClass::operator ==(const BusInfoClass &other) const
{
    if (this->m_busId == other.m_busId)
        return true;

    return false;
}

bool BusInfoClass::operator <(const BusInfoClass &other) const
{
    if (this->m_busId < other.m_busId)
        return true;

    return false;
}

QString BusInfoClass::getBusId() const
{
    return m_busId;
}

void BusInfoClass::setBusId(const QString &busId)
{
    m_busId = busId;
}

int BusInfoClass::getByteAddrH() const
{
    return m_byteAddrH;
}

void BusInfoClass::setByteAddrH(const int &byteAddrH)
{
    m_byteAddrH = byteAddrH;
}

int BusInfoClass::getByteAddrL() const
{
    return m_byteAddrL;
}

void BusInfoClass::setByteAddrL(const int &byteAddrL)
{
    m_byteAddrL = byteAddrL;
}

int BusInfoClass::getBitAddr() const
{
    return m_bitAddr;
}

void BusInfoClass::setBitAddr(const int &bitAddr)
{
    m_bitAddr = bitAddr;
}

int BusInfoClass::getFormulaTemplateId() const
{
    return m_formulaTemplateId;
}

void BusInfoClass::setFormulaTemplateId(const int &formulaTemplateId)
{
    m_formulaTemplateId = formulaTemplateId;
}

QString BusInfoClass::getFormulaParameter() const
{
    return m_formulaParameter;
}

void BusInfoClass::setFormulaParameter(const QString &formulaParameter)
{
    m_formulaParameter = formulaParameter;
}

int BusInfoClass::getAddrType() const
{
    return m_addrType;
}

void BusInfoClass::setAddrType(const int &addrType)
{
    m_addrType = addrType;
}
