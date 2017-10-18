#include "Sys.h"
#include "SysTimer.h"
#include "ili9341.h"
#include "Button.h"
#include "PMS7003.h"
#include "ir.h"
#include "Wifi.h"
#include "Motor.h"

static uint8_t g_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

//redirect "printf()"
int fputc(int ch, FILE *f)
{
	HalUartWrite(SYS_UART_LOGS_PORT, (const uint8_t *)&ch, 1);
	return ch;
}

const uint8_t *SysGetMacAddr(void)
{
    return (const uint8_t *)g_mac;
}

void SysSetMacAddr(const uint8_t *mac)
{
    memcpy(g_mac, mac, 6);
    
    SysLog("%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

const char *SysGetServerUrl(void)
{
    return "192.168.123.194";
}

uint16_t SysGetServerPort(void)
{
    return 10086;
}

void SysStatusLedSet(uint8_t blink)
{
    HalCommonStatusLedSet(blink);
}

void SysInterruptSet(bool enable)
{
    static bool irqEnable = true;

    if(irqEnable != enable)
    {
        HalInterruptSet(enable);
    }
    
    irqEnable = enable;
}

static void sysLogOutputInit(void)
{
    HalUartConfig_t config;
    config.baudrate = 115200;
    config.flowControl = 0;
    config.parity = 0;
    config.wordLength = USART_WordLength_8b;
    config.recvCb = NULL;
    HalUartConfig(SYS_UART_LOGS_PORT, &config);
}

static void testWifi(void *args)
{
    if(WifiGetWorkMode() == WIFI_MODE_PASSTHROUGH)
    {
        WifiNetConfigStart();
    }
}

void SysInitialize(void)
{
    HalCommonInitialize();
    sysLogOutputInit();
    SysTimerInitialize();
    ILI9341Initialize();
    ButtonInitialize();
    PMS7003Initailize();
    IRInitialize();
    WifiInitialize();
    MotorInitialize();
    SysLog("");

    SysTimerSet(testWifi, 1500, 0, NULL);
}

void SysPoll(void)
{
    HalCommonPoll();
    SysTimerPoll();
    ButtonPoll();
    PMS7003Poll();
    IRPoll();
    WifiPoll();
    MotorPoll();
}

