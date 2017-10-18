#include "Sys.h"
#include "SysTimer.h"

static void testPrint(void)
{
    static SysTime_t lastPrintTime;

    if(SysTimeHasPast(lastPrintTime, 1000))
    {
        SysLog("SysTime:%d", SysTime());
        lastPrintTime = SysTime();
    }
}

int main(void)
{
    SysInitialize();
    SysStatusLedSet(3);

    while(1)
    {
        SysPoll();
        //testPrint();
    }
}

