#ifndef WIFI_H
#define WIFI_H

#include "Sys.h"

typedef enum
{
    WIFI_MODE_NONE = 0,
    WIFI_MODE_PASSTHROUGH, //透传模式
    WIFI_MODE_COMMAND, //命令模式
    WIFI_MODE_NETCONFIG,
}WifiMode_t;

void WifiNetConfigStart(void);
WifiMode_t WifiGetWorkMode(void);
void WifiInitialize(void);
void WifiPoll(void);

#endif

