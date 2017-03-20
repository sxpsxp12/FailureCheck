#ifndef SCHEMATICCLASS_H
#define SCHEMATICCLASS_H

#include <QString>
#include <QList>

class SchematicClass
{
public:
    SchematicClass();

    SchematicClass( const QString &SchematicCatalog, const QString &SchematicName );

    SchematicClass( const SchematicClass &other );

    SchematicClass operator = (const SchematicClass &other );

    bool operator == ( const SchematicClass &other ) const;

    bool operator < ( const SchematicClass &other ) const;

    QString getSchematicCatalog() const;
    void setSchematicCatalog(const QString &SchematicCatalog);

    QString getSchematicName() const;
    void setSchematicName(const QString &SchematicName);

private:
    QString m_SchematicCatalog;
    QString m_SchematicName;
};

typedef QList<SchematicClass> SchematicInfoList;

#endif // SCHEMATICCLASS_H
