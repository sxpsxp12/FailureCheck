#ifndef LADDERDIAGRAMCLASS_H
#define LADDERDIAGRAMCLASS_H

#include <QString>
#include <QList>

class LadderDiagramClass
{
public:
    LadderDiagramClass() {}

    LadderDiagramClass( const QString &pageNumStr, const QString &xCoordStr,
                        const QString &x2CoordStr, const QString &yCoordStr,
                        const QString &busId, const QString &partTypeStr,
                        const QString &meaning );

    LadderDiagramClass( const QString &pageNumStr, const QString &xCoordStr,
                        const QString &yCoordStr, const QString &busId,
                        const QString &partTypeStr, const QString &meaning );

    LadderDiagramClass( const LadderDiagramClass &other );

    LadderDiagramClass operator = (const LadderDiagramClass &other );

    bool operator == ( const LadderDiagramClass &other ) const;

    bool operator < ( const LadderDiagramClass &other ) const;

    QString getPageNumStr() const;
    void setPageNumStr(const QString &pageNumStr);

    int getPageNumInt() const;
    void setPageNumInt(const int &pageNumInt);

    QString getXCoordStr() const;
    void setXCoordStr(const QString &xCoordStr);

    int getXCoordInt() const;
    void setXCoordInt(const int &xCoordInt);

    QString getX2CoordStr() const;
    void setX2CoordStr(const QString &x2CoordStr);

    int getX2CoordInt() const;
    void setX2CoordInt(const int &x2CoordInt);

    QString getYCoordStr() const;
    void setYCoordStr(const QString &yCoordStr);

    int getYCoordInt() const;
    void setYCoordInt(const int &yCoordInt);

    QString getBusId() const;
    void setBusId(const QString &busId);

    QString getPartTypeStr() const;
    void setPartTypeStr(const QString &partTypeStr);

    int getPartTypeInt() const;
    void setPartTypeInt(const int &partTypeInt);

    QString getMeaning() const;
    void setMeaning(const QString &meaning);

    int getColor() const;
    void setColor(const int &color);

    static bool LadderDiagramClassInfoSort(const LadderDiagramClass gatewayInfoFirst,
                                      const LadderDiagramClass gatewayInfoSecond);

private:
    QString m_pageNumStr;
    int m_pageNumInt;
    QString m_xCoordStr;
    int m_xCoordInt;
    QString m_x2CoordStr;
    int m_x2CoordInt;
    QString m_yCoordStr;
    int m_yCoordInt;
    QString m_busId;
    QString m_partTypeStr;
    int m_partTypeInt;
    QString m_meaning;

    int m_color;
};

typedef QList<LadderDiagramClass> LadderDiagramInfoList;

#endif // LADDERDIAGRAMCLASS_H
