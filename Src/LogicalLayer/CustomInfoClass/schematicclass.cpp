#include "schematicclass.h"

SchematicClass::SchematicClass()
{
}

SchematicClass::SchematicClass(const QString &SchematicCatalog, const QString &SchematicName)
{
    m_SchematicCatalog = SchematicCatalog;
    m_SchematicName = SchematicName;
}

SchematicClass::SchematicClass(const SchematicClass &other)
{
    this->m_SchematicCatalog = other.m_SchematicCatalog;
    this->m_SchematicName = other.m_SchematicName;
}

SchematicClass SchematicClass::operator =(const SchematicClass &other)
{
    if ( this == &other )
        return *this;

    this->m_SchematicCatalog = other.m_SchematicCatalog;
    this->m_SchematicName = other.m_SchematicName;

    return *this;
}

bool SchematicClass::operator ==(const SchematicClass &other) const
{
    if(this->m_SchematicCatalog == other.m_SchematicCatalog)
        return true;

    return false;
}

bool SchematicClass::operator <(const SchematicClass &other) const
{
    if(this->m_SchematicCatalog < other.m_SchematicCatalog)
        return true;

    return false;
}

QString SchematicClass::getSchematicCatalog() const
{
    return m_SchematicCatalog;
}

void SchematicClass::setSchematicCatalog(const QString &SchematicCatalog)
{
    m_SchematicCatalog = SchematicCatalog;
}

QString SchematicClass::getSchematicName() const
{
    return m_SchematicName;
}

void SchematicClass::setSchematicName(const QString &SchematicName)
{
    m_SchematicName = SchematicName;
}
