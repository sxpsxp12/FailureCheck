#include "canoperate.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QStringList>

#include "logfile.h"
#include "sys/socket.h"
#include "requestdatathread.h"

Posix_QextSerialPort *CanOperate::cdjCom;
struct sockaddr_can CanOperate::addr;
struct ifreq CanOperate::ifr;
int CanOperate::s;

CanOperate::CanOperate()
{
}

bool CanOperate::InitCan()
{
    QString portName = "/dev/ttyUSB0";
    //定义一个结构体，用来存放串口各个参数
    cdjCom = new Posix_QextSerialPort(portName,QextSerialBase::Polling);
    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    cdjCom->setBaudRate(BAUD115200);    //波特率
    cdjCom->setDataBits(DATA_8);      //数据位
    cdjCom->setParity(PAR_NONE);    //校验位
    cdjCom->setStopBits(STOP_1);   //停止位
    cdjCom->setFlowControl(FLOW_OFF);   //数据流
    cdjCom->setTimeout(500);
    return cdjCom ->open(QIODevice::ReadWrite);  //以可读写方式打开串口
}

bool CanOperate::openCanSocket()
{
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW); //创建套接字
    if(s < 0)
    {
#if DEBUG
        qDebug() << "openCanSocket socket false";
#endif
#if LOG
        LogFile::debug("openCanSocket socket false");
#endif
        return false;
    }

    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr); //指定can0设备
    fcntl(s, F_SETFL, O_NONBLOCK);  //非阻塞
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if(bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) //将套接字与can0绑定
    {
#if DEBUG
        qDebug() << "openCanSocket bind false";
#endif
#if LOG
        LogFile::debug("openCanSocket bind false");
#endif
        return false;
    }
    return true;
}

bool CanOperate::readCanHandshake(QString &data)
{
    data.clear();
    int nbytes;
    struct can_frame frame;

    nbytes = read(s, &frame, sizeof(frame));  //接收报文
    if(nbytes <= 0)
    {
#if DEBUG
        qDebug()  << " readCanHandshake false";
#endif
#if LOG
        LogFile::debug("readCanHandshake false");
#endif
        return false;
    }else
    {
        for(int i=0;i<8;i++)
        {
            if(frame.data[i] < 16)
            {
                data.append( QString::number(frame.data[i],16).prepend("0") );
            }else
            {
                data.append(QString::number(frame.data[i],16));
            }
        }
#if DEBUG
        qDebug() << "handshakeData:" << data;
#endif
#if LOG
        LogFile::debug(QString("handshakeData:%1").arg(data));
#endif
        return true;
    }
}

bool CanOperate::readCanRequestData(QString &data,const bool &is_master)
{
    data.clear();
    int nbytes;
    struct can_frame frame;
    QString lastDataID;
    lastDataID.clear();
    int dataLength = 0;
    int byteCount = 0;
    RequestDataThread::m_mapAddressAndCanBusDataPackage.clear();

    //如果是从机，进行数据判断
    if(!is_master)
    {
        dataLength = 26;    //从机的数据读26侦，主机读27侦
        while(1)
        {
            frame.can_id = 0;
            frame.can_dlc = 0;
            memset(frame.data,0,sizeof(frame.data));

            nbytes = read(s, &frame, sizeof(frame));  //接收报文
            if(nbytes <= 0)
            {
#if DEBUG
                qDebug()  << " readCanRequestData false";
#endif
#if LOG
                LogFile::debug("readCanRequestData false");
#endif
                return false;
            }else
            {
                //上包的地址是0D0，本包的地址是6C或者上包数据是空，本包数据是6C
                if( QString::number(frame.can_id,16).toUpper() == "6C" && lastDataID == "D0" )
                {
                    //qDebug() << "第一次id:" << QString::number(frame.can_id,16).toUpper();
                    for(int i=0;i<8;i++)
                    {
                        if(frame.data[i] < 16)
                        {
                            data.append( QString::number(frame.data[i],16).prepend("0") );
                        }else
                        {
                            data.append(QString::number(frame.data[i],16));
                        }
                        RequestDataThread::m_mapAddressAndCanBusDataPackage[byteCount++] = CanBusDataPackageClass(QString::number(frame.can_id,16).toUpper(),QString::number(frame.data[i],16));
                    }
                    //读取剩下的26侦数据
                    break;
                }else
                {
                    //将本包数据的ID进行记录
                    lastDataID = QString::number(frame.can_id,16).toUpper();
                }
            }
        }
    }else
    {
        dataLength = 27;
    }

    for(int m = 0;m < dataLength;++m)
    {
        frame.can_id = 0;
        frame.can_dlc = 0;
        memset(frame.data,0,sizeof(frame.data));

        nbytes = read(s, &frame, sizeof(frame));  //接收报文
        if(nbytes <= 0)
        {
#if DEBUG
            qDebug()  << " readCanRequestData false";
#endif
#if LOG
            LogFile::debug(" readCanRequestData false");
#endif
            return false;
        }else
        {
            //qDebug() << "id:" << QString::number(frame.can_id,16).toUpper();
            for(int i=0;i<8;i++)
            {
                if(frame.data[i] < 16)
                {
                    data.append( QString::number(frame.data[i],16).prepend("0") );
                }else
                {
                    data.append(QString::number(frame.data[i],16));
                }
                RequestDataThread::m_mapAddressAndCanBusDataPackage[byteCount++] = CanBusDataPackageClass(QString::number(frame.can_id,16).toUpper(),QString::number(frame.data[i],16));
            }
        }
    }

#if DEBUG
    qDebug() << "TotalRealData:" << data;
#endif
#if LOG
    LogFile::debug(QString("TotalRealData:%1").arg(data));
#endif
    return true;
}

bool CanOperate::readCan(QString &data)
{
    data.clear();
    QByteArray dataByteArray;
    dataByteArray.clear();

    dataByteArray = cdjCom->readAll().toHex();

    if(dataByteArray.isEmpty())
    {
#if DEBUG
        qDebug() << "readCan false";
#endif
#if LOG
        LogFile::debug("readCan false");
#endif
        return false;
    }

    data.append(dataByteArray);
#if DEBUG
    qDebug() << "data:" << data;
#endif
#if LOG
    LogFile::debug(QString("data:%1").arg(data));
#endif
    return true;
}

bool CanOperate::writeCanHandshake()
{
    char str[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#if SERIAL_CAN

    qint64 flag = cdjCom->write(QByteArray(str));
    if(flag == -1)
    {
        return false;
    }else
    {
#if DEBUG
        qDebug() << "Bytesize:" <<flag;
#endif
#if LOG
        LogFile::debug(QString("Bytesize:%1").arg(flag));
#endif
        return true;
    }
#endif

#if SOCKET_CAN

    int nbytes;
    struct can_frame frame;

    frame.can_id = SENDCAN_ADDR;
    frame.can_dlc = 8;

    for(int i=0;i<8;i++)
    {
        frame.data[i] = str[i];
    }

    //循环发送两个报文
    nbytes = write(s, &frame, sizeof(frame));     //发送frame
    if(nbytes != sizeof(frame))
    {
#if DEBUG
        qDebug() << " writeCanHandshake Send Error frame!";
#endif
#if LOG
        LogFile::debug(" writeCanHandshake Send Error frame!");
#endif
        return false; //发送错误，退出
    }
    return true;
#endif
}

bool CanOperate::writeCanRequestData()
{
    qint64 flag;
    QByteArray sendDataHex;
    QString str;
    sendDataHex.clear();
    str.clear();

    QDateTime dateTime = QDateTime::currentDateTime();
    int yearDec = dateTime.toString("yyyy").right(2).toInt();
    int monthDec = dateTime.toString("MM").toInt();
    int dayDec = dateTime.toString("dd").toInt();
    int hourDec = dateTime.toString("hh").toInt();
    int minuteDec = dateTime.toString("mm").toInt();
    int secondDec = dateTime.toString("ss").toInt();

    QString checkHex = QString::number((~(yearDec+monthDec+dayDec+hourDec+minuteDec+secondDec)+1),16).right(2);
    if(checkHex.size() == 1)
    {
        checkHex.prepend("0");
    }

    QString yearHex = QString::number(yearDec,16);
    if(yearHex.size() == 1)
    {
        yearHex.prepend("0");
    }
    QString monthHex = QString::number(monthDec,16);
    if(monthHex.size() == 1)
    {
        monthHex.prepend("0");
    }
    QString dayHex = QString::number(dayDec,16);
    if(dayHex.size() == 1)
    {
        dayHex.prepend("0");
    }
    QString hourHex = QString::number(hourDec,16);
    if(hourHex.size() == 1)
    {
        hourHex.prepend("0");
    }
    QString minuteHex = QString::number(minuteDec,16);
    if(minuteHex.size() == 1)
    {
        minuteHex.prepend("0");
    }
    QString secondHex = QString::number(secondDec,16);
    if(secondHex.size() == 1)
    {
        secondHex.prepend("0");
    }
#if DEBUG
    qDebug() << "Hex:" << yearHex << "," << monthHex << "," << dayHex << "," << hourHex << "," << minuteHex << "," << secondHex;
#endif
#if LOG
    LogFile::debug(QString("Hex:%1,%2,%3,%4,%5,%6").arg(yearHex,monthHex,dayHex,hourHex,minuteHex,secondHex));
#endif

#if DEBUG
    qDebug()<< "Dec:" << yearDec << "," << monthDec << "," << dayDec << "," << hourDec << "," << minuteDec << "," << secondDec;
#endif
#if LOG
    LogFile::debug(QString("Dec:%1,%2,%3,%4,%5,%6").arg(QString::number(yearDec),
                                                        QString::number(monthDec),
                                                        QString::number(dayDec),
                                                        QString::number(hourDec),
                                                        QString::number(minuteDec),
                                                        QString::number(secondDec)));
#endif
    QString zeroHex = QString::number(0,16);
    if(zeroHex.size() == 1)
    {
        zeroHex.prepend("0");
    }

    str.append(yearHex);
    str.append(monthHex);
    str.append(dayHex);
    str.append(hourHex);
    str.append(minuteHex);
    str.append(secondHex);
    for(int i = 0;i < 25;i++)
    {
        str.append(zeroHex);
    }
    str.append(checkHex);
#if DEBUG
    qDebug() <<"QuestdataStr:"<< str;
#endif
#if LOG
    LogFile::debug(QString("QuestdataStr:%1").arg(str));
#endif
    String2Hex(str,sendDataHex);

#if SERIAL_CAN
    flag = cdjCom->write(sendDataHex);
    if(flag == -1)
    {
        return false;
    }
#if DEBUG
    qDebug() << "Bytesize:" <<flag;
#endif
#if LOG
    LogFile::debug(QString("Bytesize:%1").arg(flag));
#endif
    return true;
#endif

#if SOCKET_CAN

    int nbytes,i = 0;
    struct can_frame frame[4];

    frame[0].can_id = SENDCAN_ADDR;
    frame[0].can_dlc = 8;

    frame[1].can_id = SENDCAN_ADDR;
    frame[1].can_dlc = 8;

    frame[2].can_id = SENDCAN_ADDR;
    frame[2].can_dlc = 8;

    frame[3].can_id = SENDCAN_ADDR;
    frame[3].can_dlc = 8;

    //将请求数据指令赋值

    for(i=0;i<8;i++)
    {
        frame[0].data[i] = sendDataHex.at(i);
    }

    for(i;i < 16;i++)
    {
        frame[1].data[i-8] = sendDataHex.at(i);
    }

    for(i;i < 24;i++)
    {
        frame[2].data[i-16] = sendDataHex.at(i);
    }

    for(i;i < 32;i++)
    {
        frame[3].data[i-24] = sendDataHex.at(i);
    }

    nbytes = write(s, &frame[0], sizeof(frame[0]));     //发送frame[0]
    if(nbytes != sizeof(frame[0]))
    {
#if DEBUG
        qDebug() << "writeCanRequestData Send Error frame[0]\n!";
#endif
#if LOG
        LogFile::debug("writeCanRequestData Send Error frame[0]\n!");
#endif
        return false; //发送错误，退出
    }

    nbytes = write(s, &frame[1], sizeof(frame[1]));     //发送frame[0]
    if(nbytes != sizeof(frame[1]))
    {
#if DEBUG
        qDebug() << "writeCanRequestData Send Error frame[1]\n!";
#endif
#if LOG
        LogFile::debug("writeCanRequestData Send Error frame[1]\n!");
#endif
        return false; //发送错误，退出
    }

    nbytes = write(s, &frame[2], sizeof(frame[2]));     //发送frame[0]
    if(nbytes != sizeof(frame[2]))
    {
#if DEBUG
        qDebug() << "writeCanRequestData Send Error frame[2]\n!";
#endif
#if LOG
        LogFile::debug("writeCanRequestData Send Error frame[2]\n!");
#endif
        return false; //发送错误，退出
    }

    nbytes = write(s, &frame[3], sizeof(frame[3]));     //发送frame[0]
    if(nbytes != sizeof(frame[3]))
    {
#if DEBUG
        qDebug() << "writeCanRequestData Send Error frame[3]\n!";
#endif
#if LOG
        LogFile::debug("writeCanRequestData Send Error frame[3]\n!");
#endif
        return false; //发送错误，退出
    }
    return true;
#endif
}

void CanOperate::closeCanSocket()
{
    close(s);
}

void CanOperate::String2Hex(QString str, QByteArray &senddata)
{
    QStringList strList;
    strList.clear();

    int len = str.length();
    for(int i = 0;i < len;i+=2)
    {
        QString temp = str.mid(i,2);
        strList.append(temp);
    }

    for(int i = 0;i < strList.count();i++)
    {
        senddata.append(strList.at(i).toInt(0,16));
    }
}
