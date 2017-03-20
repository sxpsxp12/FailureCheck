#ifndef CBASEOPERATE_H
#define CBASEOPERATE_H

#include <QString>
#include <QStringList>
#include "realdatarecordclass.h"
#include <QFile>

class CBaseOperate
{
public:
    CBaseOperate();
    virtual ~CBaseOperate();

/******************记录故障到txt文档中的相关函数******************/
    static int newDataTxtFileNumber;
    static QStringList newDataTxtFileList;
    static QFile *file;
    static QString oldNewDataTxtFileName;
    //获取txt文件的个数
    static bool getNewDataTxtFileNumber();
    //将新数据的文件的大小进行限制
    static bool limitNewDataFileTxtNumber();
    //将实时数据记录到txt文档中
    static bool writeRealDataToFile(const RealDataRecordClass &realDataRecord);

/******************USB导出的相关函数******************/
    static bool createFileDir(QString &outputPath);
    static bool exportDataToUSB(const QString &fileName,const QString &outputPath);
    static bool clearNewData();
    static bool clearHistoryData();
    static bool copyNewDataToHistoryData();
    static bool exportAfterInit();

/******************USB导入的相关函数******************/
    static bool checkImportPathFile(const QString &path);       //检查导入文件夹中的文件的个数和文件名是否正确
    static int getUSBMountPath(QString &path);  //获取USB挂载的目录
    static int getUSBDir(QString path,QStringList &dirList);     //获取USB中的目录
    static bool rebootSystem();

/******************Log记录到txt文档中相关函数******************/
    static QString oldLogFileName;
    static int logFileNumber;
    static QStringList logTxtFileList;
    static bool limitLogFileNumber();
    static bool getLogFileNumber();
/**********************主从机判断****************************/
    static bool isMasterMachine(bool &isMaster);      //主从机判断

/*********************记录数据到SD卡************************/
    static QString oldRealDataFileName;
    static int RealDataFileNumber;
    static QStringList RealDataTxtFileList;
    static bool limitRealDataFileNumber();
    static bool getRealDataFileNumber();
    static bool writeRealDataToSD(const QString &realData);

    /*******************语音播报*****************************/
    static int syn6288_send_cmd(int cmd,int baudrate_mode,int fd,char sendata[]);
    static int convert(const char *fromset,const char *toset,char *from,size_t from_len,char *to,size_t to_len);
    static int openport(char *Dev);
    static int setport(int fd, int baud,int databits,int stopbits,int parity);
    static int voiceFd;
    static int voicePlayInit();     //语音播报初始化
    static int voicePlay(const QString &playContent);
    static int clostVoiceFd();
};

#endif // CBASEOPERATE_H
