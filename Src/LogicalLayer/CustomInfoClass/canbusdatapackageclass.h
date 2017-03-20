#ifndef CANBUSDATAPACKAGECLASS_H
#define CANBUSDATAPACKAGECLASS_H

#include <QString>
#include <QMap>

class CanBusDataPackageClass
{
public:
    CanBusDataPackageClass();

    CanBusDataPackageClass(const QString &canBusDataIdStr,const QString &canBusByte);

    CanBusDataPackageClass(const CanBusDataPackageClass &other);

    CanBusDataPackageClass operator = (const CanBusDataPackageClass &other);

    bool operator == (const CanBusDataPackageClass &other) const;

    bool operator < (const CanBusDataPackageClass &other) const;

    int getBitAddress(const int &bit) const;

    QString getBitAddressStr(const int &bit) const;

    void setDecLowBitNumber(const int &decLowBitNumber);
    int getDecLowBitNumber() const;

    void setDecLowBitNumberStr(const QString &decLowBitNumberStr);
    QString getDecLowBitNumberStr() const;

    void setDecNumber(const int &decNumber);
    int getDecNumber() const;

    void setDecNumberStr(const QString &decNumberStr);
    QString getDecNumberStr() const;

    void setHexLowBitNumberStr(const QString &hexLowBitNumberStr);
    QString getHexLowBitNumberStr() const;

    void setHexNumberStr(const QString &hexNumberStr);
    QString getHexNumberStr() const;

    void setCanBusDataIDStr(const QString &canBusDataIdStr);
    QString getCanBusDataIDStr() const;

private:
    int m_bitAddress[8];                  //位数值
    QString m_bitAddressStr[8];           //位数值String

    int m_decLowBitNumber;                //低四位十进制数值
    QString m_decLowBitNumberStr;         //低四位十进制数值String
    int m_decNumber;                      //该字节十进制数值
    QString m_decNumberStr;               //该字节十进制数值String

    QString m_hexLowBitNumberStr;                //低四位十六进制数值
    QString m_hexNumberStr;                      //该字节十六进制数值
    QString m_CanBusDataIdStr;                  //总线数据帧ID
};

typedef QMap<int, CanBusDataPackageClass> MapAddressAndCanBusDataPackage;

#endif // CANBUSDATAPACKAGECLASS_H
