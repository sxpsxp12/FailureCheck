#ifndef REALDATARECORDCLASS_H
#define REALDATARECORDCLASS_H

#include "failurehistoryrecordclass.h"
#include <QStringList>
#include <QString>
#include <QList>

class RealDataRecordClass
{
public:
    RealDataRecordClass();
    RealDataRecordClass(const RealDataRecordClass &other);

    bool operator == ( const RealDataRecordClass &other ) const;
    RealDataRecordClass operator =(const RealDataRecordClass &other);

    FailureHistoryRecordInfoList getfailureHistoryInfoList() const;
    void setFailureHistoryInfoList(const FailureHistoryRecordInfoList &failureHistoryInfoList);

    QStringList getFrontQStringTimeAndRealDataList() const;
    void setFrontQStringTimeAndRealDataList(const QStringList &frontQStringTimeAndRealDataList);

    QStringList getAfteQStringTimeAndRealDataList() const;
    void setAfterQStringTimeAndRealDataList(const QStringList &afterDateTimeAndRealDataList);

private:
    FailureHistoryRecordInfoList m_failureHistoryInfoList;
    QStringList m_frontQStringTimeAndRealDataList;      //前段时间间隔的格式化时间和实时数据
    QStringList m_AfterQStringTimeAndRealDataList;      //后段时间间隔的格式化时间和实时数据
};

typedef QList<RealDataRecordClass> realDataRecordClassList;
#endif // REALDATARECORDCLASS_H
