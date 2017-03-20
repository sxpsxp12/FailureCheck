#ifndef REALTIMEDATACLASS_H
#define REALTIMEDATACLASS_H

#include <QString>
#include <QList>

class RealtimeDataClass
{
public:
    RealtimeDataClass();

    RealtimeDataClass( const QString &dataKey, const int &categoryId, const int &pageNum,
                       const int &xCoord, const int &yCoord, const int &dataDisplayMethod,
                       const QString &strNumericalValue, const int &color,
                       const QString &imageName, const QString &deviceStateDesc,const bool &setDeviceState = false);

    RealtimeDataClass( const QString &dataKey, const int &color,
                       const QString &imageName, const QString &deviceStateDesc,const bool &setDeviceState = false);

    RealtimeDataClass( const RealtimeDataClass &other );

    RealtimeDataClass operator = (const RealtimeDataClass &other );

    bool operator == ( const RealtimeDataClass &other ) const;

    bool operator < ( const RealtimeDataClass &other ) const;

    double getDblNumericalValue() const;
    void setDblNumericalValue(const double &dblNumericalValue);

    QString getStrNumericalValue() const;
    void setStrNumericalValue(const QString &strNumericalValue);

    int getColor() const;
    void setColor(const int &color);

    QString getImageName() const;
    void setImageName(const QString &imageName);

    QString getDeviceStateDesc() const;
    void setDeviceStateDesc(const QString &deviceStateDesc);

    int getCategoryId() const;
    void setCategoryId(const int &categoryId);

    int getPageNum() const;
    void setPageNum(const int &pageNum);

    int getXCoord() const;
    void setXCoord(const int &xCoord);

    int getYCoord() const;
    void setYCoord(const int &yCoord);

    QString getDataKey() const;
    void setDataKey(const QString &dataKey);

    int getDataDisplayMethod() const;
    void setDataDisplayMethod(const int &dataDisplayMethod);

    bool getSetDeviceState() const;
    void setSetDeviceState(const bool &isSetDeviceState);

private:
    QString m_dataKey;  //数据键  包含：真实线号、虚拟线号、延迟Id、自锁Id
    int m_categoryId;
    int m_pageNum;
    int m_xCoord;
    int m_yCoord;
    int m_dataDisplayMethod;

    //数据项
    double m_dblNumericalValue;  //用于逻辑运算  所有通过线号码表算数运算后的值都保存为double类型
    QString m_strNumericalValue;    //用于界面展示
    int m_color;
    QString m_imageName;
    QString m_deviceStateDesc;

    bool m_isSetDeviceState;

};

typedef QList<RealtimeDataClass> RealtimeDataInfoList;

#endif // REALTIMEDATACLASS_H
