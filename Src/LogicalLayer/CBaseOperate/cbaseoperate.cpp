#include "cbaseoperate.h"

#include <stdio.h>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QDir>
#include "logfile.h"
#include "enumtypeheader.h"

#include   <unistd.h>
#include   <sys/types.h>
#include   <iconv.h>
#include   <sys/stat.h>
#include   <fcntl.h>
#include   <string.h>
#include   <unistd.h>
#include   <fcntl.h>
#include   <errno.h>
#include   <termios.h>
#include   <sys/time.h>
#include   <malloc.h>

int CBaseOperate::newDataTxtFileNumber = 0;
QStringList CBaseOperate::newDataTxtFileList;
QFile *CBaseOperate::file = new QFile;
QString CBaseOperate::oldNewDataTxtFileName = NULL;

int CBaseOperate::logFileNumber = 0;
QString CBaseOperate::oldLogFileName = NULL;
QStringList CBaseOperate::logTxtFileList;

QString CBaseOperate::oldRealDataFileName = NULL;
int CBaseOperate::RealDataFileNumber = 0;
QStringList CBaseOperate::RealDataTxtFileList;

int CBaseOperate::voiceFd;      //语音播报标识符

CBaseOperate::CBaseOperate()
{
}

CBaseOperate::~CBaseOperate()
{

}

bool CBaseOperate::getNewDataTxtFileNumber()
{
    QDir *dir = new QDir(NEWDATA);
    QStringList namefiler;

    newDataTxtFileList.clear();
    namefiler.clear();

    namefiler.append("*.txt");
    newDataTxtFileList.append(dir->entryList(namefiler));

    newDataTxtFileNumber = newDataTxtFileList.size();
    return true;
}

bool CBaseOperate::limitNewDataFileTxtNumber()
{
    QString smallDateTime;
    QDir *dir = new QDir(NEWDATA);
    smallDateTime.clear();

    //获取最新的数据文件列表
    getNewDataTxtFileNumber();
    if(newDataTxtFileList.isEmpty())
    {
        return false;
    }

    smallDateTime=newDataTxtFileList.at(0);
    for(int i=0;i < newDataTxtFileList.size();i++)
    {
        //获取列表中最小的文件名
        if(smallDateTime.compare(newDataTxtFileList.at(i)) > 0 )
        {
            smallDateTime = newDataTxtFileList.at(i);
        }
    }

    newDataTxtFileList.removeOne(smallDateTime);
    //删除最旧的文件
    return dir->remove(smallDateTime);
}

bool CBaseOperate::writeRealDataToFile(const RealDataRecordClass &realDataRecord)
{
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString fileName;
    QString textStream;

    fileName.append(NEWDATA);
    fileName.append(dateTime);
    fileName.append(".txt");

    //判断是否大于文件个数的限制31个
    if(newDataTxtFileNumber > NEWDATAFILE_SIZE)
    {
        //删除旧的文件
        if( limitNewDataFileTxtNumber() )
        {
            newDataTxtFileNumber--;
        }
    }

    //满足条件，不大于31，同时产生新文件并将操作新的文件
    if(oldNewDataTxtFileName.compare(fileName) != 0)
    {
        //文件增加
        newDataTxtFileNumber++;
        if(oldNewDataTxtFileName != NULL)
        {
            file->close();
        }

        oldNewDataTxtFileName = fileName;
        file->setFileName(fileName);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
#if DEBUG
            qDebug() << "error openFile error !";
#endif
#if LOG
            LogFile::debug("error openFile error !");
#endif
            return false;
        }
    }

    QTextStream stream(file);

    foreach (QString realData, realDataRecord.getFrontQStringTimeAndRealDataList()) {
        textStream.append(realData);
        textStream.append("\n");
    }

    foreach (FailureHistoryRecordClass failureHistory, realDataRecord.getfailureHistoryInfoList())
    {
        QString temp;
        temp.clear();
        temp.append(QString("故障号:%1,故障名称:%2,故障等级:%3,故障发生时间:%4").arg(failureHistory.getFailureId(),failureHistory.getFailureName(),QString::number(failureHistory.getFailureLv()),failureHistory.getStartTime()));
        textStream.append(temp);
        textStream.append("\n");
    }

    foreach (QString realData, realDataRecord.getAfteQStringTimeAndRealDataList()) {
        textStream.append(realData);
        textStream.append("\n");
    }

    stream << textStream << "\n";
    return true;
}

bool CBaseOperate::createFileDir(QString &outputPath)
{
    QDir *dir = new QDir;

    return dir->mkdir(outputPath);
}

bool CBaseOperate::exportDataToUSB(const QString &fileName, const QString &outputPath)
{
    FILE *stream;
    char arr[1024] = {0};

    //连文件夹一块考到u盘中
    stream = popen(QString("cp -r %1 %2").arg(fileName,outputPath).toAscii().data(),"r");
    if(!stream)
    {
        return false;
    }

    fread(arr,sizeof(char),sizeof(arr),stream);
    pclose(stream);

    if(strlen(arr) == 0)    //创建成功
    {
        return true;
    }else
    {
        return false;
    }
}

bool CBaseOperate::clearNewData()
{
    FILE *stream;
    stream = popen(QString("rm %1*").arg(NEWDATA).toAscii().data(),"r");
    if(!stream)
    {
        return false;
    }

    pclose(stream);
    return true;
}

bool CBaseOperate::clearHistoryData()
{
    FILE *stream;

    stream = popen(QString("rm %1*").arg(HISTORYDATA).toAscii().data(),"r");
    if(!stream)
    {
        return false;
    }

    pclose(stream);
    return true;
}

bool CBaseOperate::copyNewDataToHistoryData()
{
    FILE *stream;

    //连文件夹一块考到u盘中
    stream = popen(QString("cp %1* %2").arg(NEWDATA,HISTORYDATA).toAscii().data(),"r");
    if(!stream)
    {
        return false;
    }

    pclose(stream);
    return true;
}

//导出之后初始化
bool CBaseOperate::exportAfterInit()
{
    oldNewDataTxtFileName.clear();
    //故障信息记录
    if( getNewDataTxtFileNumber() )
    {
        //判断是否大于文件个数的限制31个
        if(newDataTxtFileNumber > NEWDATAFILE_SIZE)
        {
            //删除旧的文件
            if(limitNewDataFileTxtNumber())
            {
                newDataTxtFileNumber--;
            }
        }
    }
}

bool CBaseOperate::checkImportPathFile(const QString &path)
{
    QDir *dir = new QDir(path);
    QStringList namefiler;
    QStringList fileList;

    fileList.clear();
    namefiler.clear();

    namefiler.append("*.txt");
    fileList.append(dir->entryList(namefiler));

    if(fileList.size() < IMPORTFILENUMBER)
    {
        return false;
    }

    if( fileList.contains(FILE_LCUINFO) && fileList.contains(FILE_CHECK) && fileList.contains(FILE_LGINFO) && fileList.contains(FILE_SCHEM)
            && fileList.contains(FILE_K2INFO) && fileList.contains(FILE_HOSTINFO) && fileList.contains(FILE_POINT)
            && fileList.contains(FILE_LOGIC) && fileList.contains(FILE_K1INFO) && fileList.contains(FILE_DCUINFO) )
    {
        return true;
    }else
    {
        return false;
    }
}

int CBaseOperate::getUSBMountPath(QString &path)
{
    path.clear();

    FILE *stream;
    char arr[1024] = {0};
    stream = popen("cat /proc/mounts | grep '/dev/sd' | cut -d ' ' -f 2 ","r");
    if(!stream)     //失败
    {
        return 1;
    }

    fread(arr,sizeof(char),sizeof(arr),stream);
    if(strlen(arr) == 0)
    {
        return 1;
    }

    path.append(arr);
    //取消换行符
    path.replace("\n","\0");
    pclose(stream);
    return 0;
}

int CBaseOperate::getUSBDir(QString path, QStringList &dirList)
{
    QString dirStr;
    QString command;
    FILE *stream;
    dirList.clear();
    char arr[1024] = {0};

    command = "ls ";
    command.append(path);
    command.append(" -F | grep '/$'");

    stream = popen(command.toAscii().data(),"r");
    if(!stream)     //失败
    {
        return 1;
    }

    fread(arr,sizeof(char),sizeof(arr),stream);
    if(strlen(arr) == 0)
    {
        return 1;
    }

    dirStr.append(arr);
    //取消换行符
    dirStr.replace("\n"," ");
    dirStr.replace("/","");
    dirList = QString(dirStr).split(" ",QString::SkipEmptyParts);
    pclose(stream);
    return 0;
}

bool CBaseOperate::rebootSystem()
{
    FILE *stream;
    char arr[1024] = {0};

    stream = popen("reboot","r");
    if(!stream)
    {
        return false;
    }

    fread(arr,sizeof(char),sizeof(arr),stream);
    pclose(stream);

    if(strlen(arr) == 0)
    {
        return true;
    }else
    {
        return false;
    }
}

bool CBaseOperate::limitLogFileNumber()
{
    QString smallDateTime;
    QDir *dir = new QDir(LOGPATH);

    //限制调试信息文件的大小之前。先去得到当前的调试信息的文件列表
    getLogFileNumber();
    if( logTxtFileList.isEmpty() )
    {
        return false;
    }

    smallDateTime.clear();
    smallDateTime=logTxtFileList.at(0);
    for(int i=0;i < logTxtFileList.size();i++)
    {
        //获取列表中最小的文件名
        if(smallDateTime.compare(logTxtFileList.at(i)) > 0 )
        {
            smallDateTime = logTxtFileList.at(i);
        }
    }
    logTxtFileList.removeOne(smallDateTime);
    //删除最旧的文件
    return dir->remove(smallDateTime);
}

bool CBaseOperate::getLogFileNumber()
{
    QDir *dir = new QDir(LOGPATH);
    QStringList namefiler;

    logTxtFileList.clear();
    namefiler.clear();

    namefiler.append("*.txt");
    logTxtFileList.append(dir->entryList(namefiler));

    logFileNumber = logTxtFileList.size();
    return true;
}
#if 0
bool CBaseOperate::isMasterMachine(bool &isMaster)
{
    QDir *dir = new QDir(MASTER_SLAVE_MACHINE_PATH);
    QFile *valueFile = new QFile;
    QStringList fileList;

    fileList.clear();
    fileList.append(dir->entryList(QDir::Files));

    //主从机判断路径下没有所需的文件
    if(fileList.size() == 0)
    {
        return false;
    }

    if( fileList.contains(MASTER_SLAVE_MACHINE_FILE) )
    {

        valueFile->setFileName(QString("%1%2").arg(MASTER_SLAVE_MACHINE_PATH,MASTER_SLAVE_MACHINE_FILE));
        if (!valueFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
#if DEBUG
            qDebug() << "can't open value file!";
#endif
#if LOG
            LogFile::debug("can't open value file!");
#endif
            return false;
        }
        QString fileLine;
        QTextStream stream(valueFile);

        fileLine = stream.read(1);
        if( !fileLine.compare(MASTER_MACHINE_FLAG) )   //主机
        {
            isMaster = true;
        }else
        {
            isMaster = false;
        }

    }else       //不包含value返回false
    {
        return false;
    }
    return true;
}
#endif
bool CBaseOperate::isMasterMachine(bool &isMaster)
{
    bool ret=false;
    QDir *dir = new QDir(MASTER_SLAVE_MACHINE_PATH);
    QFile *valueFile = new QFile;
    do
    {
        if (dir==NULL)
        {
          break;
        }
        if(valueFile==NULL)
        {
            break;
        }

        QStringList fileList;

        fileList.clear();
        fileList.append(dir->entryList(QDir::Files));

        //主从机判断路径下没有所需的文件
        if(fileList.size() == 0)
        {
            break;
        }

        if( fileList.contains(MASTER_SLAVE_MACHINE_FILE) )
        {

            valueFile->setFileName(QString("%1%2").arg(MASTER_SLAVE_MACHINE_PATH,MASTER_SLAVE_MACHINE_FILE));
            if (!valueFile->open(QIODevice::ReadOnly | QIODevice::Text))
            {
    #if DEBUG
                qDebug() << "can't open value file!";
    #endif
    #if LOG
                LogFile::debug("can't open value file!");
    #endif
               break;
            }
            QString fileLine;
            QTextStream stream(valueFile);

            fileLine = stream.read(1);
            if( !fileLine.compare(MASTER_MACHINE_FLAG) )   //主机
            {
                isMaster = true;
            }else
            {
                isMaster = false;
            }
            ret=true;

        }
    }while(false);

    delete dir;
    delete valueFile;


    return ret;
}

bool CBaseOperate::limitRealDataFileNumber()
{
    QString smallDateTime;
    QDir *dir = new QDir(SD_PATH);

    getRealDataFileNumber();
    if(RealDataTxtFileList.isEmpty())
    {
        return false;
    }

    smallDateTime.clear();
    smallDateTime=RealDataTxtFileList.at(0);
    for(int i=0;i < RealDataTxtFileList.size();i++)
    {
        //获取列表中最小的文件名
        if(smallDateTime.compare(RealDataTxtFileList.at(i)) > 0 )
        {
            smallDateTime = RealDataTxtFileList.at(i);
        }
    }
    RealDataTxtFileList.removeOne(smallDateTime);
    //删除最旧的文件
    return dir->remove(smallDateTime);
}

bool CBaseOperate::getRealDataFileNumber()
{
    QDir *dir = new QDir(SD_PATH);
    QStringList namefiler;

    RealDataTxtFileList.clear();
    namefiler.clear();

    namefiler.append("*.txt");
    RealDataTxtFileList.append(dir->entryList(namefiler));

    RealDataFileNumber = RealDataTxtFileList.size();
    return true;
}

bool CBaseOperate::writeRealDataToSD(const QString &realData)
{
    QString filePath;
    filePath = SD_PATH + QDate::currentDate().toString("yyyy-MM-dd") + ".txt";

    //判断是否大于文件个数的限制7个
    if(RealDataFileNumber > SDFILE_SIZE)
    {
        //删除旧的文件
        if( limitRealDataFileNumber() )
        {
            RealDataFileNumber--;
        }
    }

    //满足条件，不大于7
    if(oldRealDataFileName.compare(filePath) != 0)
    {
        RealDataFileNumber++;

        if(oldRealDataFileName != NULL)
        {
            file->close();
        }

        oldRealDataFileName = filePath;
        file->setFileName(filePath);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
#if DEBUG
            qDebug() << "debug SDFile error !";
#endif
            return false;
        }
    }

    QTextStream stream(file);
    stream << realData << "\n";
}

int CBaseOperate::syn6288_send_cmd(int cmd, int baudrate_mode, int fd, char sendata[])
{
    char rbuf[5];
    char wbuf[5];

    int i,ecc = 0;
    size_t len,length;
    char sendata_true[255]={'\0'};

    for(i=0;i<5;i++)
        rbuf[i]='0';

    switch(cmd)
    {
    case 1:
    {
        length=strlen(sendata);
        if(0==convert("UTF-8","GB2312",sendata,length,sendata_true,255))
            return 0;

        len = strlen(sendata_true);

        wbuf[0] = 0xFD;	//构造帧头FD
        wbuf[1] = 0x00;  //构造数据区长度的高字节
        wbuf[2] = len + 3;	//构造数据区长度的低字节
        wbuf[3] = 0x01;	//构造命令字：合成播放命令
        wbuf[4] = 0x10;	//构造命令参数：编码格式为GB2312,背景音乐1

        while(1)
        {
            ecc = 0;
            for(i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }

            for(i=0;i<len;i++)
            {
                write(fd,&sendata_true[i],1);
                ecc = ecc^sendata_true[i];
            }

            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    case 2:
    {
        switch(baudrate_mode)
        {
        case 1://9600
        {
            wbuf[0] = 0xFD;
            wbuf[1] = 0x00;
            wbuf[2] = 0x03;
            wbuf[4] = 0x31;
            wbuf[5] = 0x00;
            break;
        }
        case 2://19200
        {
            wbuf[0] = 0xFD;
            wbuf[1] = 0x00;
            wbuf[2] = 0x03;
            wbuf[4] = 0x31;
            wbuf[5] = 0x01;
            break;
        }
        case 3: //38400
        {
            wbuf[0] = 0xFD;
            wbuf[1] = 0x00;
            wbuf[2] = 0x03;
            wbuf[4] = 0x31;
            wbuf[5] = 0x02;
            break;
        }
        default:  //error
        {
            //break;
            //printf("set baudrate_mode error!\n");
#if LOG
            LogFile::debug("set baudrate_mode error!");
#endif
            return 0;
        }
        }


        while(1)
        {
            for (i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }
            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            //printf("(set baudrate)call back = %x\n",rbuf[0]);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    case 3:
    {
        wbuf[0] = 0xFD;
        wbuf[1] = 0x00;
        wbuf[2] = 0x03;
        wbuf[4] = 0x02;
        wbuf[5] = 0x00;
        while(1)
        {
            for (i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }
            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            // printf("(stop) call back = %x\n",rbuf[0]);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    case 4:
    {
        wbuf[0] = 0xFD;
        wbuf[1] = 0x00;
        wbuf[2] = 0x03;
        wbuf[4] = 0x03;
        wbuf[5] = 0x00;
        while(1)
        {
            for (i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }
            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            //printf("(pause)call back = %x\n",rbuf[0]);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    case 5:
    {
        wbuf[0] = 0xFD;
        wbuf[1] = 0x00;
        wbuf[2] = 0x03;
        wbuf[4] = 0x04;
        wbuf[5] = 0x00;
        while(1)
        {
            for (i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }
            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            //printf("(recover)call back = %x\n",rbuf[0]);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    case 6:
    {
        wbuf[0] = 0xFD;
        wbuf[1] = 0x00;
        wbuf[2] = 0x03;
        wbuf[4] = 0x21;
        wbuf[5] = 0x00;

        for (i=0;i<5;i++)
        {
            write(fd,&wbuf[i],1);
            ecc = ecc^wbuf[i];
        }
        write(fd,&ecc,1);
        usleep(15000);
        read(fd,rbuf,1);
        //printf("(status)call back = %x\n",rbuf[0]);

        if(0x4a==rbuf[0])
            return 40;
        else if(0x41==rbuf[0])
            return 41;
        else if(0x45==rbuf[0])
            return 42;
        else if(0x4e==rbuf[0])
            return 43;
        else if(0x4f==rbuf[0])
            return 44;

        break;
    }
    case 7:
    {
        wbuf[0] = 0xFD;
        wbuf[1] = 0x00;
        wbuf[2] = 0x03;
        wbuf[4] = 0x88;
        wbuf[5] = 0x00;
        while(1)
        {
            for (i=0;i<5;i++)
            {
                write(fd,&wbuf[i],1);
                ecc = ecc^wbuf[i];
            }
            write(fd,&ecc,1);
            usleep(15000);
            read(fd,rbuf,1);
            //printf("(powerdown)call back = %x\n",rbuf[0]);
            if(rbuf[0] == 0x41)
                break;
        }
        break;
    }
    default:
    {
        //break;
        //printf("cmd error!\n");
#if LOG
        LogFile::debug("cmd error!");
#endif
        return 0;
        break;
    }
    }

    return 41;
}

int CBaseOperate::convert(const char *fromset, const char *toset, char *from, size_t from_len, char *to, size_t to_len)
{
    iconv_t cd;

    cd=iconv_open(toset,fromset);
    char **from2=&from;
    char **to2=&to;

    if(iconv(cd,from2,&from_len,to2,&to_len)==-1)
    {
#if LOG
        LogFile::debug("Convert fail!");
#endif
        iconv_close(cd);
        return 0;
    }

    iconv_close(cd);
    return 1;
}

int CBaseOperate::openport(char *Dev)
{
    int fd = open(Dev, O_RDWR|O_NOCTTY|O_NDELAY );
    if (-1 == fd)
    {
        //perror("Can''t Open Serial Port");
#if LOG
        LogFile::debug("Can't Open Serial Port");
#endif
        return -1;
    }else
    {
        //printf("Open Serial %s Succeed!\n",Dev);
        return fd;
    }
}

int CBaseOperate::setport(int fd, int baud, int databits, int stopbits, int parity)
{
    int baudrate;
    struct termios newtio;
    switch(baud)
    {
    case 300:
        baudrate=B300;
        break;
    case 600:
        baudrate=B600;
        break;
    case 1200:
        baudrate=B1200;
        break;
    case 2400:
        baudrate=B2400;
        break;
    case 4800:
        baudrate=B4800;
        break;
    case 9600:
        baudrate=B9600;
        break;
    case 19200:
        baudrate=B19200;
        break;
    case 38400:
        baudrate=B38400;
        break;
    default :
        baudrate=B9600;
        break;
    }
    tcgetattr(fd,&newtio);
    bzero(&newtio,sizeof(newtio));
    //setting   c_cflag
    newtio.c_cflag   &=~CSIZE;
    switch (databits)
    {
    case 7:
        newtio.c_cflag |= CS7;
        break;
    case 8:
        newtio.c_cflag |= CS8;
        break;
    default:
        newtio.c_cflag |= CS8;
        break;
    }
    switch (parity)
    {
    case 'n':
    case 'N':
        newtio.c_cflag &= ~PARENB;   /* Clear parity enable */
        newtio.c_iflag &= ~INPCK;     /* Enable parity checking */
        break;
    case 'o':
    case 'O':
        newtio.c_cflag |= (PARODD | PARENB);
        newtio.c_iflag |= INPCK;             /* Disnable parity checking */
        break;
    case 'e':
    case 'E':
        newtio.c_cflag |= PARENB;     /* Enable parity */
        newtio.c_cflag &= ~PARODD;
        newtio.c_iflag |= INPCK;       /* Disnable parity checking */
        break;
    case 'S':
    case 's':  /*as no parity*/
        newtio.c_cflag &= ~PARENB;
        newtio.c_cflag &= ~CSTOPB;break;
    default:
        newtio.c_cflag &= ~PARENB;   /* Clear parity enable */
        newtio.c_iflag &= ~INPCK;     /* Enable parity checking */
        break;
    }
    switch (stopbits)
    {
    case 1:
        newtio.c_cflag &= ~CSTOPB;  //1
        break;
    case 2:
        newtio.c_cflag |= CSTOPB;  //2
        break;
    default:
        newtio.c_cflag &= ~CSTOPB;
        break;
    }
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
    newtio.c_cflag   |=   (CLOCAL|CREAD);
    newtio.c_oflag|=OPOST;
    newtio.c_iflag   &=~(IXON|IXOFF|IXANY);
    cfsetispeed(&newtio,baudrate);
    cfsetospeed(&newtio,baudrate);
    tcflush(fd,   TCIFLUSH);
    if (tcsetattr(fd,TCSANOW,&newtio) != 0)
    {
        //perror("SetupSerial 3");
        return -1;
    }
    //printf("Setup Serial Succeed!\n");
    return 0;
}

/**
    函数说明：
    syn6288_send_cmd(int cmd,int baudrate_mode,int fd,char sendata[])
    参数说明：
    int cmd:
             1--语音合成播放命令;
             2--改通讯波特率命令;
             3--停止合成命令;
             4--暂停合成命令;
             5--恢复合成命令;
             6--芯片状态查询命令");
             7--进入Power Down模式命令");

    int baudrate_mode:
            1--9600bps
            2--19200bps
            3--38400bps
    int fd:
            打开串口句柄
    char sendata[]:
            所要播放语音内容

    注意：
        当cmd设置为1时，参数sendata有效，cmd设置为非1时，参数sendata值忽略
        当cmd设置为2时，参数baudrate_mode有效，cmd设置为非2时，参数baudrate_mode值忽略

    返回值：如果正确，返回1，否则返回0。
    **/

int CBaseOperate::voicePlayInit()
{
    int ret;
    voiceFd = openport(VOICE_PATH);
    if(voiceFd>0)
    {
        ret=setport(voiceFd,9600,8,1,'n');
        if(ret<0)
        {
            //printf("Can't Set Serial Port!\n");
#if LOG
            LogFile::debug("voicePlay Can't Set Serial Port!");
#endif
            return -1;
        }

        return 0;
    }else
    {
#if LOG
        LogFile::debug("voicePlay Can't Open Serial Port");
#endif
        return -1;
    }
}

int CBaseOperate::voicePlay(const QString &playContent)
{
    QByteArray content = playContent.toUtf8();
    char *ch = content.data();
    syn6288_send_cmd(1,1,voiceFd,ch);
}

int CBaseOperate::clostVoiceFd()
{
    close(voiceFd);
}
