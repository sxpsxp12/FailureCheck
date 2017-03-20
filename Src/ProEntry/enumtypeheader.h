#ifndef ENUMTYPEHEADER_H
#define ENUMTYPEHEADER_H

//启动界面显示时间，测试时为0ms，发布时为2000ms
#define STARTUP_TIME 10
#define REQUEST_BUS_DATA_TIME_INTERVAL 200     //请求数据的时间间隔
#define VOICE_PLAY_TIME 5000                      //语音播报周期
#define REQUEST_BUS_DATA_TIMEOUT 3000
#define FAILURE_DETECT_DELAY_TIME 1000          //故障确认显示和播报的时间
#define FAILURE_RECORDDATA_TIME_POINT 1000    //故障发生时，实时数据的记录时间点（记录前后1s的数据）
#define FAILURE_HISTORY_RECORD_MAXNUM 10000     //故障历史记录最大的条数，大于则删除旧的记录
#define PASSWORD "12344321"                     //密码
#define VERSION "V1.00"                         //版本号
#define REALDATALIST_LENGTH_MULTIPLE  5        //实时数据列表的长度的倍数
#define NEWDATA  "../FailureCheck/Files/newData/"   //新数据路径
#define HISTORYDATA "../FailureCheck/Files/historyData/"  //旧数据路径
#define LOGPATH "../FailureCheck/Files/Log/"            //日志路径
#define IMAGEPATH "../FailureCheck/Qrc/Image/"          //资源图片路径
#define SD_PATH ""                              //SD卡的路经
#define WRITEREALDATA_TOSD  0                   //将实时数据写到SD卡中，当SD卡路径添加上以后，将该宏定义成1
#define NEWDATAFILE_SIZE 31                     //实时数据文件夹中文件的个数
#define LOGFILE_SIZE 7                          //调试文件的个数
#define SDFILE_SIZE  7                           //SD卡中文件的个数

#define IMPORTFILENUMBER 10                     //导入文件的个数
#define FILE_CHECK "check.txt"
#define FILE_DCUINFO "dcuinfor.txt"
#define FILE_HOSTINFO "hostinfor.txt"
#define FILE_K1INFO "k1infor.txt"
#define FILE_K2INFO "k2infor.txt"
#define FILE_LCUINFO "lcuinfor.txt"
#define FILE_LGINFO "lginfor.txt"
#define FILE_LOGIC "logic.txt"
#define FILE_POINT "point.txt"
#define FILE_SCHEM "schem.txt"

//原理图初始化比例
#define SCHEMA_INITSCALE 0.2

#define MASTER_SLAVE_MACHINE_PATH "/sys/class/gpio/gpio113/"        //主从机路径
#define MASTER_SLAVE_MACHINE_FILE "value"        //主从机判断文件
#define VOICE_PATH "/dev/ttymxc1"               //语音设备路径


#define MASTER_MACHINE_FLAG "1"               //主机标志

#define SOCKET_CAN 1        //socket CAN
#define SERIAL_CAN 0       //串口can
#define SENDCAN_ADDR 0xD0   //发送CAN的地址

#define DEBUG 0             //debug信息
#define LOG 0               //log信息
#define LOGW 0               //log信息

enum InterfaceName
{
    NO_INTERFACE = 0,               //无界面
    PRIMARY_INTERFACE = 1,          //一级界面
    SECONDARY_INTERFACE = 2,        //二级界面
    BUS_ID_INTERFACE = 3,           //线号界面
    INTERFACE_LAMP_HOLE = 4,        //接口灯孔界面
    DCU = 5,                        //微机柜界面
    DCU2 = 6,                       //微机柜二架界面
    GCU = 7,                        //GCU界面
    LG = 8,                         //LG界面
    FAILURE_LIST = 9,               //故障列表界面
    FAILURE_DETAILS = 10,            //故障详情界面
    FAILURE_RECORD = 11,            //故障记录界面
    LADDER_DIAGRAM = 12,            //梯形图界面
    SCHEMATIC_DIRECTORY = 13,       //原理图目录界面
    SCHEMATIC_DISPLAY = 14,         //原理图图片界面
    IMPORT_AND_EXPORT = 15          //导入导出界面
};

enum FailureLv
{
    SERIOUS = 1,                    //严重
    MEDIUM = 2,                     //中等
    SLIGHT = 3,                     //轻微
    PROMPT = 4                      //提示
};

enum LogicalRelationship
{
    GREATER_OR_EQUAL = 1,           //>=
    GREATER = 2,                    //>
    LESS_OR_EQUAL = 3,              //<=
    LESS = 4,                       //<
    EQUAL = 5,                      //==
    NOT_EQUAL = 6                   //≠
};

enum PartState
{
    DISCONNECT = 0,                                     //断开
    CONNECT = 1                                         //接通
};

enum SchematicDiagram           //目录中的原理图枚举
{
    MAINDIAGRAM = 0,               //主电路原理图
    ASSISTDIAGRAM = 1,              //辅助电路图
    CONTROLDIAGRAMONE = 2,          //控制电路图1
    CONTROLDIAGRAMTWO = 3,          //控制电路图2
    CONTROLDIAGRAMTHREE = 4,          //控制电路图3
    CONTROLDIAGRAMFOUR = 5,          //控制电路图4
    CONTROLDIAGRAMFIVE = 6,          //控制电路图5
    CONTROLDIAGRAMSIX = 7,          //控制电路图6
    CONTROLDIAGRAMSEVEN = 8,          //控制电路图7
    CHANNALDIAGRAMONE = 9,         //管道原理图1
    CHANNALDIAGRAMTWO = 10,         //管道原理图2
    DETAILONE = 11,                 //明细1
    DETAILTWO = 12,                 //明细2
    DETAILTHREE = 13                 //明细3
};

enum MachineStatus              //机器的状态
{
    NO_WORK = 1,        //不工作状态
    NORMAL_WORK = 2,   //正常工作
    ABNORMAL_WORK = 3   //异常工作
};

enum MachineType            //二级界面机器的类型
{
    ELECTRAIL_MACHINE = 2,     //电机
    OTHER_MACHINE = 1          //非电机
};

enum PartType
{
    NORMALLY_CLOSED_CONTACT = 1,                        //常闭
    NORMALLY_OPEN_CONTACT = 2,                          //常开
    REPEATER = 3,                                       //中继
    OUTPUT = 5,                                         //输出
    VERTICAL_LINE = 6,                                  //竖线
    HORIZONTAL_LINE = 7,                                //横线
    NORMALLY_CLOSED_CONTACT_ADD_VERTICAL_LINE = 8,      //常闭 + 竖线
    NORMALLY_OPEN_CONTACT_ADD_VERTICAL_LINE = 9,        //常开 + 竖线
    VERTICAL_LINE_ADD_HORIZONTAL_LINE = 10              //竖线 + 横线
};

enum TxtFileColumnNumber
{
    DCUINFOR_TXT_COLUMN = 10,                //dcuinfor.txt 列数
    HOSTINFOR_TXT_COLUMN = 10,               //hostinfor.txt 列数
    K1INFOR_TXT_COLUMN = 10,                 //k1infor.txt 列数
    K2INFOR_TXT_COLUMN = 10,                 //k2infor.txt 列数
    LCUINFOR_TXT_COLUMN_NUM = 10,             //lcuinfor.txt 列数
    LGINFOR_TXT_COLUMN_NUM = 10,              //lginfor.txt 列数
    POINT_TXT_COLUMN_NUM = 7,                 //point.txt 列数
    CHECK_TXT_COLUMN_NUM = 34,               //check.txt 列数
    SCHEM_TXT_COLUMN_NUM = 2,                //schem.txt 列数
    LOGIC_TXT_COLUMN_NUM = 126               //logic.txt 列数
};

enum InterfaceShowCoord
{
    K1_COLUMN_NUM = 3,                      //一级界面显示列数
    K2_COLUMN_NUM = 6,                      //二级界面显示列数
    BUS_ID_PAGE_NUM = 176,                  //线号界面显示总数
    BUS_ID_COLUMN_NUM = 16,                 //线号界面显示列数
    INTERFACE_PAGE_NUM = 20,                //接口灯孔界面显示总数
    INTERFACE_ROW_NUM = 10,                  //接口灯孔界面显示行数
    INTERFACE_1_X_COORD = 0,                 //接口灯孔界面第一个tableWidget的第一项的列坐标
    INTERFACE_2_X_COORD = 4,                 //接口灯孔界面第二个tableWidget的第一项的列坐标
    DCU_ROW_NUM = 18,                       //微机柜显示行数
    DCU1_X_COORD = 0,                       //微机柜一架显示列坐标
    DCU2_X_COORD = 2,                       //微机柜一架显示列坐标
    HOST_PAGE_NUM = 30,                     //GCU显示总数
    HOST_ROW_NUM = 15,                      //GCU显示行数
    HOST_X1_COORD = 0,                       //GCU显示第一列坐标
    HOST_X2_COORD = 2,                       //GCU显示第二列坐标
    LG_PAGE_NUM = 15,                     //LG显示总数
    LG_ROW_NUM = 15,                      //LG显示行数
    LG_X_COORD = 0,                          //LG显示列坐标
    FAILURE_LIST_ROW_NUM = 5,                 //故障列表界面行数
    PRIMARY_FAILURE_LIST_ROW_NUM = 9,          //一级界面故障列表行数
    SECONDARY_FAILURE_LIST_ROW_NUM = 9         //二级界面提示列表行数
};

enum ColorEnumValue
{
    WHITE = 0,
    GREEN = 1,
    RED = 2,
    YELLOW = 3
};

enum AddrType
{
    NO_ADDR = 0,                        //无地址
    ONLY_BYTE_ADDR_L = 1,               //只有低字节地址, 计算整个字节，保存成十进制数
    BYTE_ADDR_L_ADD_BIT_ADDR = 2,       //低字节地址加位地址
    BYTE_ADDR_H_ADD_BYTE_ADDR_L = 3,    //高字节地址加低字节地址
    ONLY_BYTE_ADDR_H = 4,               //只有高字节地址 (未使用)
    BYTE_ADDR_H_ADD_BIT_ADDR = 5,       //高字节地址加位地址 (未使用)
    ONLY_BYTE_ADDR_L_FOUR_BIT_L = 6     //只有低字节地址，计算低四位的值(工况中使用)
};

enum FormulaTemplateId
{
    NO_FORMULA_TEMPLATE = 0,                                    //无公式
    FORMULA_TEMPLATE_ONE = 1,                                   //公式模板一
    FORMULA_TEMPLATE_ONE_AND_SECONDS_TRANSFORM_HH_MM_SS = 2,    //公式模板一计算出的值转换为时分秒
    FORMULA_TEMPLATE_ONE_AND_RESULT_ADD_PERCENT = 3             //公式模板一计算出的值后加‘%’
};

enum DataDisplayMethod
{
    NO_DISPLAY = 0,                             //不显示这个设备
    ONLY_DISPLAY_IMAGE = 1,                     //只显示图片(一级界面中的机车图片和二级界面控件)
    DISPLAY_NUMERICAL_VALUE_AND_COLOR = 2,      //显示数值和颜色(一级界面3个控件、微机柜部分内容)
    ONLY_DISPLAY_COLOR = 3,                     //只显示颜色(LCU线号、灯孔)
    ONLY_DISPLAY_DEVICE_STATE_DESC = 4,         //只显示设备状态描述(工况)
    ONLY_DISPLAY_DECIMAL_NUMERICAL_VALUE = 5,   //只显示十进制数值(牵引风速1继电器)
    NOT_ANY_DATA_ONLY_MEANING = 6,              //只显示含义，不显示任何数据(油温)
    ONLY_DISPLAY_HEX_NUMERICAL_VALUE = 7        //只显示十六进制数值(故障优先级、故障代码1、故障代码2、故障代码3)
};

enum TitleBar_Status        //标题栏状态
{
    NORMAL_STATUS = 1,          //正常状态
    ABNORMAL_STATUS = 2,        //异常状态
    FAILE_STATUS = 3,           //失败状态
    UNKNOWN_STATUS = 4          //未知状态

};

enum CanData_Id      //Can数据帧ID
{
    COMMUNICATION_CANDATA_ID = 0x000000d0,  //通信帧
    LCU_CANDATA_ID = 0x0000006c,        //LCUcan总线帧ID
    DCU_CANDATA_ID = 0x00000045,        //微机柜一架帧ID
    DCU2_CANDATA_ID = 0x00000046,       //微机柜二架帧ID
    LG_CANDATA_ID = 0x0000005a,         //列供一路帧ID
    LG2_CANDATA_ID =  0x0000005b,       //列供二路帧ID
    HOST_CANDATA_ID = 0x000000a0        //主机帧ID
};

enum CanData_Size      //Can数据大小
{
    LCU_CANDATA_Size = 48,        //LCUcan数据大小
    DCU_CANDATA_Size = 56,        //微机柜数据大小
    LG_CANDATA_Size = 8,         //列供数据大小
    HOST_CANDATA_Size = 40        //主机数据大小
};

enum GuiContainer_TitleBar      //容器的标题栏
{
    LCU_TITLEBAR = 1,           //lcd标题栏
    DCU_TITLEBAR = 2,           //微机柜标题栏
    DCU2_TITLEBAR = 3,          //微机柜二架标题栏
    HOST_TITLEBAR = 4           //主机标题栏
};

#endif // ENUMTYPEHEADER_H
