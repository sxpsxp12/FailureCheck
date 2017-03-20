#include "fileinfoclass.h"

FileInfoClass::FileInfoClass()
{
}

FileInfoClass::FileInfoClass(const bool &isFileFormatOk,const bool isInitDataBaseOk,
                             const QList<int> &errorRowNumList,const QString &errorFileName)
{
    m_isFileFormatOk = isFileFormatOk;
    m_isInitDataBaseOk = isInitDataBaseOk;
    m_errorRowNumList = errorRowNumList;
    m_errorFileName = errorFileName;
}

FileInfoClass::FileInfoClass(const FileInfoClass &other)
{
    this->m_isFileFormatOk = other.m_isFileFormatOk;
    this->m_isInitDataBaseOk = other.m_isInitDataBaseOk;
    this->m_errorRowNumList = other.m_errorRowNumList;
    this->m_errorFileName = other.m_errorFileName;
}

FileInfoClass FileInfoClass::operator =(const FileInfoClass &other)
{
    if(this == &other)
    {
        return *this;
    }

    this->m_isFileFormatOk = other.m_isFileFormatOk;
    this->m_isInitDataBaseOk = other.m_isInitDataBaseOk;
    this->m_errorRowNumList = other.m_errorRowNumList;
    this->m_errorFileName = other.m_errorFileName;

    return *this;
}

bool FileInfoClass::operator ==(const FileInfoClass &other) const
{
    if(this->m_errorFileName.compare(other.m_errorFileName) == 0 &&
            this->m_errorRowNumList == other.m_errorRowNumList)
    {
        return true;
    }
    return false;
}

void FileInfoClass::setFileFormatOk(const bool &isFileFormatOk)
{
    m_isFileFormatOk = isFileFormatOk;
}

bool FileInfoClass::getFileFormatOk() const
{
    return m_isFileFormatOk;
}

void FileInfoClass::setInitDataBaseOk(const bool &isInitDataBaseOk)
{
    m_isInitDataBaseOk = isInitDataBaseOk;
}

bool FileInfoClass::getInitDataBaseOk() const
{
    return m_isInitDataBaseOk;
}

void FileInfoClass::seterrorRowNum(const int &errorRowNum)
{
    m_errorRowNumList.append(errorRowNum);
}

QList<int> FileInfoClass::geterrorRowNumList() const
{
    return m_errorRowNumList;
}

void FileInfoClass::seterrorFileName(const QString &errorFileName)
{
    m_errorFileName = errorFileName;
}

QString FileInfoClass::geterrorFileName() const
{
    return m_errorFileName;
}
