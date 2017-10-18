#ifndef SYS_H
#define SYS_H

#include "HalCommon.h"

#define SYS_UART_LOGS_PORT HAL_UART_PORT_1

#define SYS_DEVICE_ID_LEN 16
#define SYS_PASSWORD_LEN   8

#define DEBUG_ENABLE 1
#define SysTime_t uint32_t
#define SysTime HalGetSysTimeCount
#define SysTimeHasPast(oldtime, pass) ((SysTime() - (oldtime)) > pass)

#if DEBUG_ENABLE

#define HalPrintf(...) printf(__VA_ARGS__)
#define SysPrintf HalPrintf
#define SysLog(...) SysPrintf("%s: ", __FUNCTION__); SysPrintf(__VA_ARGS__); SysPrintf("\n");
#else
#define HalPrintf(...)
#define SysPrintf HalPrintf
#define SysLog(...)
#endif

typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  hour;
    uint8_t  minute;
    uint8_t  second;
    uint16_t msec;
}SysDateTime_t;

void SysSetMacAddr(const uint8_t *mac);
const uint8_t *SysGetMacAddr(void);
const char *SysGetServerUrl(void);
uint16_t SysGetServerPort(void);
void SysStatusLedSet(uint8_t blink);
void SysSetDevType(char *type);
const char *SysGetDevType(void);
const char *SysGetDevicePwd(void);
const char *SysGetDeviceID(void);
const char *SysGetVersion(void);
void SysInterruptSet(bool enable);
void SysInitialize(void);
void SysPoll(void);

#endif

