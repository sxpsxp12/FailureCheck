#ifndef FILEINFOCLASS_H
#define FILEINFOCLASS_H

#include <QString>
#include <QList>

class FileInfoClass
{
public:
    FileInfoClass();
    FileInfoClass(const bool &isFileFormatOk,const bool isInitDataBaseOk,
                  const QList<int> &errorRowNumList,const QString &errorFileName);
    FileInfoClass(const FileInfoClass &other);
    FileInfoClass operator = (const FileInfoClass &other );
    bool operator == ( const FileInfoClass &other ) const;

    void setFileFormatOk(const bool &isFileFormatOk);
    bool getFileFormatOk() const;

    void setInitDataBaseOk(const bool &isInitDataBaseOk);
    bool getInitDataBaseOk() const;

    void seterrorRowNum(const int &errorRowNum);
    QList<int> geterrorRowNumList() const;

    void seterrorFileName(const QString &errorFileName);
    QString geterrorFileName() const;

private:
    bool m_isFileFormatOk;      //文件格式出错标志位
    bool m_isInitDataBaseOk;    //使用该文件初始化数据库出错标志位
    QList<int> m_errorRowNumList;          //错误行号列表
    QString m_errorFileName;    //错误文件名
};

typedef QList<FileInfoClass> FileInfoClassList;

#endif // FILEINFOCLASS_H
