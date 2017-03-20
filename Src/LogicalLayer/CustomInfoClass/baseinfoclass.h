#ifndef BASEINFOCLASS_H
#define BASEINFOCLASS_H

#include <QString>
#include <QList>

class BaseInfoClass
{
public:
    BaseInfoClass() {}

    BaseInfoClass( const QString &busId, const int &categoryId, const QString &interfaces,
                   const QString &indicatorLight, const QString &meaning,
                   const int &pageNum, const int &xCoord, const int &yCoord, const int &dataDisplayMethod );

    BaseInfoClass( const BaseInfoClass &other );

    BaseInfoClass operator = (const BaseInfoClass &other );

    bool operator == ( const BaseInfoClass &other ) const;

    bool operator < ( const BaseInfoClass &other ) const;

    QString getBusId() const;
    void setBusId(const QString &busId);

    QString getInterfaces() const;
    void setInterfaces(const QString &interfaces);

    QString getIndicatorLight() const;
    void setIndicatorLight(const QString &indicatorLight);

    QString getMeaning() const;
    void setMeaning(const QString &meaning);

    int getCategoryId() const;
    void setCategoryId(const int &categoryId);

    int getPageNum() const;
    void setPageNum(const int &pageNum);

    int getXCoord() const;
    void setXCoord(const int &xCoord);

    int getYCoord() const;
    void setYCoord(const int &yCoord);

    int getDataDisplayMethod() const;
    void setDataDisplayMethod(const int &dataDisplayMethod);

private:
    QString m_busId;
    int m_categoryId;
    QString m_interfaces;
    QString m_indicatorLight;
    QString m_meaning;
    int m_pageNum;
    int m_xCoord;
    int m_yCoord;
    int m_dataDisplayMethod;
};

typedef QList<BaseInfoClass> BaseInfoList;

#endif // BASEINFOCLASS_H
