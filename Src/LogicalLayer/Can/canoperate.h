#ifndef CANOPERATE_H
#define CANOPERATE_H

#include "enumtypeheader.h"
#include "posix_qextserialport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <errno.h>

class CanOperate
{
public:
    CanOperate();

    static bool InitCan();
    static bool openCanSocket();

    static bool readCan(QString &data);
    static bool readCanHandshake(QString &data);
    static bool readCanRequestData(QString &data, const bool &is_master);

    static bool writeCanHandshake();
    static bool writeCanRequestData();

    static void closeCanSocket();

    static Posix_QextSerialPort *cdjCom;
private:
    static void String2Hex(QString str, QByteArray &senddata);

    static struct sockaddr_can addr;
    static struct ifreq ifr;
    static int s;
};

#endif // CANOPERATE_H
