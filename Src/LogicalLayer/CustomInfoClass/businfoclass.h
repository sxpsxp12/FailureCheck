#ifndef BUSINFOCLASS_H
#define BUSINFOCLASS_H

#include <QString>
#include <QList>

class BusInfoClass
{
public:
    BusInfoClass() {}

    BusInfoClass( const QString &busId, const int &byteAddrH,
                  const int &byteAddrL, const int &bitAddr,
                  const int &formulaTemplateId, const QString &formulaParameter,
                  const int &addrType );

    BusInfoClass( const BusInfoClass &other );

    BusInfoClass operator = (const BusInfoClass &other );

    bool operator == ( const BusInfoClass &other ) const;

    bool operator < ( const BusInfoClass &other ) const;

    QString getBusId() const;
    void setBusId(const QString &busId);

    int getByteAddrH() const;
    void setByteAddrH(const int &byteAddrH);

    int getByteAddrL() const;
    void setByteAddrL(const int &byteAddrL);

    int getBitAddr() const;
    void setBitAddr(const int &bitAddr);

    int getFormulaTemplateId() const;
    void setFormulaTemplateId(const int &formulaTemplateId);

    QString getFormulaParameter() const;
    void setFormulaParameter(const QString &formulaParameter);

    int getAddrType() const;
    void setAddrType(const int &addrType);

private:
    QString m_busId;
    int m_byteAddrH;
    int m_byteAddrL;
    int m_bitAddr;
    int m_formulaTemplateId;
    QString m_formulaParameter;
    int m_addrType;
};

typedef QList<BusInfoClass> BusInfoList;

#endif // BUSINFOCLASS_H
