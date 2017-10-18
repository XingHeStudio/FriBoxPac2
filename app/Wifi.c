#include "Wifi.h"
#include "VTList.h"
#include "SysTimer.h"

#define WIFI_RECV_BUFF_LEN  64

typedef enum
{
    CMD_END_FLAG_NONE = 0,
    CMD_END_FLAG_A,
    CMD_END_FLAG_OK,
    CMD_END_FLAG_AT,
}WifiCmdEndFlag_t;


/*
AT+ENTM 切换到命令模式
AT+WSMAC 获取MAC
AT+WMODE=STA 
AT+NETP=TCP,CLIENT,server.com,12345
AT+TCPTO=60/0 超时
AT+SMTSL=air  airkiss

AT+SMTLK
*/
typedef enum
{
    CMD_ID_NONE = 0,
    CMD_ID_3PLUS,     //+++
    CMD_ID_ENTM,      //从命令模式切换到透传模式
    CMD_ID_WMODE,     //设置/查询 WIFI 操作模式（AP/STA/APSTA）
    CMD_ID_NETP,      //设置/查询网络协议参数
    CMD_ID_TCPTO,     //设置/查询超时时间
    CMD_ID_SMTSL,     //设置智能配网方式
    CMD_ID_SMTLK,     //启动 Simple Config 功能
    CMD_ID_WSMAC,     //查询 STA 的 MAC 地址参数
    CMD_ID_WSLQ,      //查询 STA 的无线信号强度
    CMD_ID_WSCAN,     //搜索 AP
    CMD_ID_Z,         //重启模块
    CMD_ID_RELD,      //恢复出厂设置(重启)
}WifiCmdID_t;

typedef struct WifiCmdList_st
{
    uint8_t dlen;
    uint8_t sendCount;
    uint8_t retries;
    uint16_t intevalTime;
    WifiCmdID_t id;
    uint8_t *data;
    SysTime_t lastTime;
    VTLIST_ENTRY(struct WifiCmdList_st);
}WifiCmdList_t;

static WifiMode_t g_wifiWorkMode = WIFI_MODE_NONE;
//static WifiMode_t g_wifiSetMode = WIFI_MODE_PASSTHROUGH;
static uint8_t g_buff[WIFI_RECV_BUFF_LEN];
static uint16_t g_buffCount = 0;
static WifiCmdEndFlag_t g_cmdWaitFlag = CMD_END_FLAG_NONE;
static WifiCmdList_t g_cmdListHead;
static WifiCmdID_t g_currentCmdID = CMD_ID_NONE;

static void cmdSend(const char *cmd, uint8_t retries, uint16_t inteval, WifiCmdID_t id);

static void lowSend(uint8_t *data, uint16_t len)
{
    SysLog("%s", data);
    HalUartWrite(HAL_UART_PORT_3, data, len);
}

static uint8_t *getMacAddr(char *msg, uint8_t *mac)
{
    uint8_t i;
    char val[3] = {0};

    SysLog("%s", msg);
    for(i = 0; i < 6; i++)
    {
        memcpy(val, &mac[i*2], 2);
        mac[i] = (uint8_t)(uint32_t)strtol(val, NULL, 16);
    }
    return mac;
}

static void cmdNodeDel(WifiCmdID_t id)
{
    WifiCmdList_t *node;
    
    VTListForeach(&g_cmdListHead, node)
    {
        if(node->id == id)
        {
            VTListDel(node);
            if(node->data)
            {
                free(node->data);
            }
            free(node);
            break;
        }
    }
}

static void atCmdParse(const char *atcmd)
{
    bool delCmd = false;
    char *pos;
    uint8_t mac[6];
    char buff[64] = "";

    //SysLog("%s", atcmd);
    
    switch(g_currentCmdID)
    {
        case CMD_ID_NONE:
            break;
        case CMD_ID_ENTM:
            if(strstr(atcmd, "+ok"))
            {
                delCmd = true;
                g_wifiWorkMode = WIFI_MODE_PASSTHROUGH;
                //event(passthrough);
            }
            break;
        case CMD_ID_WMODE:
            if(strstr(atcmd, "+ok"))
            {
                delCmd = true;
                sprintf(buff, "AT+NETP=TCP,CLIENT,%d,%s\r", SysGetServerPort(), SysGetServerUrl());
                cmdSend(buff, 3, 500, CMD_ID_NETP);
            }
            break;
        case CMD_ID_NETP:
            if(strstr(atcmd, "+ok"))
            {
                cmdSend("AT+TCPTO=60\r", 3, 500, CMD_ID_TCPTO);
                delCmd = true;
            }
            break;
        case CMD_ID_TCPTO:
            if(strstr(atcmd, "+ok"))
            {
                cmdSend("AT+SMTSL=air\r", 3, 500, CMD_ID_SMTSL);
                delCmd = true;
            }
            break;
        case CMD_ID_SMTSL:
            if(strstr(atcmd, "+ok"))
            {
                cmdSend("AT+SMTLK\r", 3, 500, CMD_ID_SMTLK);
                delCmd = true;
            }
            break;
        case CMD_ID_SMTLK:
            break;
        case CMD_ID_WSMAC:
            pos = strstr(atcmd, "+ok=");
            if(pos)
            {
                SysSetMacAddr(getMacAddr(strchr(pos, '=') + 1, mac));
                cmdSend("AT+WMODE=STA\r", 3, 500, CMD_ID_WMODE);
                delCmd = true;
            }
            break;
        case CMD_ID_WSLQ:
            break;
        case CMD_ID_WSCAN:
            break;
        case CMD_ID_RELD:
            break;
          
    }

    if(delCmd)
    {
        cmdNodeDel(g_currentCmdID);
        g_currentCmdID = CMD_ID_NONE;
    }
}

static void cmdSendTimeoutHandle(WifiCmdID_t id)
{
    SysLog("id = %d", id);
}

static void cmdListPoll(void)
{
    WifiCmdList_t *node = VTListFirst(&g_cmdListHead);
    
    if(node != NULL && SysTimeHasPast(node->lastTime, node->intevalTime))
    {
        if(node->sendCount >= node->retries)
        {
            VTListDel(node);
            if(node->data)
            {
                free(node->data);
            }
            free(node);
            cmdSendTimeoutHandle(node->id);
        }
        else
        {
            g_currentCmdID = node->id;
            lowSend(node->data, node->dlen);
            node->lastTime = SysTime();
            node->sendCount++;
        }
    }
}

static void cmdSend(const char *cmd, uint8_t retries, uint16_t inteval, WifiCmdID_t id)
{
    WifiCmdList_t *node;
    uint8_t len = strlen(cmd);

    if(retries)
    {
        node = (WifiCmdList_t *)malloc(sizeof(WifiCmdList_t));
        if(node)
        {
            node->data = (uint8_t *)malloc(len);
            if(node->data)
            {
                memcpy(node->data, cmd, len);
                node->dlen = len;
                node->retries = retries;
                node->sendCount = 0;
                node->intevalTime = inteval;
                node->lastTime = 0;
                node->id = id;
                VTListAdd(&g_cmdListHead, node);
            }
        }
    }
    else
    {
        lowSend((uint8_t *)cmd, len);
    }
}

static void cmdRecv(uint8_t byte)
{
    uint8_t chrA;
    
    g_buff[g_buffCount++] = byte;
    //SysPrintf("%c", byte);
    
    if(g_cmdWaitFlag == CMD_END_FLAG_A)
    {
        if(byte ==  'a')
        {
            chrA = 'a';
            lowSend(&chrA, 1);
            g_cmdWaitFlag = CMD_END_FLAG_OK;
        }
        g_buffCount = 0;
    }
    else if(g_cmdWaitFlag == CMD_END_FLAG_OK)
    {
        if(g_buffCount ==  3)
        {
            if(g_buff[0] == '+' && g_buff[1] == 'o' && g_buff[2] == 'k')
            {
                cmdNodeDel(CMD_ID_3PLUS);
                cmdSend("AT+WSMAC\r", 3, 500, CMD_ID_WSMAC);
                g_cmdWaitFlag = CMD_END_FLAG_AT;
                g_wifiWorkMode = WIFI_MODE_COMMAND;
            }
            g_buffCount = 0;
        }
    }
    else if(g_cmdWaitFlag == CMD_END_FLAG_AT)
    {
        if(byte == 0x0a)
        {
            atCmdParse((const char *)g_buff);
            g_buffCount = 0;
        }
    }

    if(g_buffCount >= WIFI_RECV_BUFF_LEN)
    {
        g_buffCount = 0;
    }
}

static void wifiRecvByte(uint8_t *data, uint16_t len)
{
    uint16_t i;

    for(i = 0; i < len; i++)
    {
        if(g_currentCmdID != CMD_ID_NONE)
        {
            cmdRecv(data[i]);
        }
        else
        {
            //ProtocolDataRecv(byte);
            SysPrintf("%c", data[i]);
        }
    }
}

static void startCommandMode(void)
{
    char cmd[3] = "+++"; 
    cmdSend(cmd, 3, 2000, CMD_ID_3PLUS);
    g_cmdWaitFlag = CMD_END_FLAG_A;
}

static void stopCommandMode(void)
{
    char *cmd = "AT+ENTM\r";
    cmdSend(cmd, 3, 1000, CMD_ID_ENTM);
}

static void wifiChangeMode(WifiMode_t mode)
{
    if(g_wifiWorkMode != mode)
    {
        if(mode == WIFI_MODE_COMMAND)
        {
            startCommandMode();
        }
        else
        {
            stopCommandMode();
        }
    }
    //g_wifiSetMode = mode;
}

WifiMode_t WifiGetWorkMode(void)
{
    return g_wifiWorkMode;
}

void WifiNetConfigStop(void)
{
}

void WifiNetConfigStart(void)
{
    SysLog("");
    startCommandMode();
}

static void ioInit(void)
{
    HalGPIOConfig(HAL_WIFI_RESET_PIN, HAL_IO_OUTPUT); //reset
    HalGPIOConfig(HAL_WIFI_RELOAD_PIN, HAL_IO_OUTPUT); //reload
    HalGPIOConfig(HAL_WIFI_READY_PIN, HAL_IO_INPUT); //ready
    HalGPIOSetLevel(HAL_WIFI_RELOAD_PIN, HAL_IO_HIGH_LEVEL);
    HalGPIOSetLevel(HAL_WIFI_RESET_PIN, HAL_IO_HIGH_LEVEL);
}

static void uartInit(void)
{
    HalUartConfig_t config;
        
    config.baudrate = HAL_WIFI_DEFAULT_BAUDRATE;
    config.flowControl = 0;
    config.parity = 0;
    config.wordLength = USART_WordLength_8b;
    config.recvCb = wifiRecvByte;
    HalUartConfig(HAL_UART_PORT_3, &config);
}

static void resetDone(void *args)
{
    SysLog("");
    HalGPIOSetLevel(HAL_WIFI_RESET_PIN, HAL_IO_HIGH_LEVEL);
}

static void startReset(void)
{
    SysLog("");
    g_wifiWorkMode = WIFI_MODE_NONE;
    HalGPIOSetLevel(HAL_WIFI_RESET_PIN, HAL_IO_LOW_LEVEL);
    SysTimerSet(resetDone, 600, 0, NULL);
}

static void moduleIsReady(void)
{
    if(g_wifiWorkMode == WIFI_MODE_NONE)
    {
        if(HalGPIOGetLevel(HAL_WIFI_READY_PIN) == HAL_IO_LOW_LEVEL)
        {
            g_wifiWorkMode = WIFI_MODE_PASSTHROUGH;
        }
    }
    
}

void WifiInitialize(void)
{
    VTListInit(&g_cmdListHead);
    ioInit();
    uartInit();
    startReset();
}

void WifiPoll(void)
{
    cmdListPoll();
    moduleIsReady();
}

