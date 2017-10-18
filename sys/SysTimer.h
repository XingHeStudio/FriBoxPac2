#ifndef SYSTIMER_H
#define SYSTIMER_H
#include "Sys.h"

#define SYS_TIMER_MAX_NUM 10

void SysTimerInitialize(void);
void SysTimerPoll(void);

#define SYS_TIMER_REPEAT        1 //重复执行
#define SYS_TIMER_IMME          2 //立即执行
#define SYS_TIMER_NEED_UNSET    4 //需要手动unset

typedef void (*SysTimerRun_t)(void *arg);

typedef struct
{
    uint8_t alloced : 1;        //已分配
    uint8_t flag : 7;           //类型
    uint16_t span;         //间隔
    SysTime_t startTime;    //上次执行时间
    SysTimerRun_t run;      //执行函数
    void *arg;              //参数
}SysTimer_t;

//设置定时执行
SysTimer_t *SysTimerSet(SysTimerRun_t run, uint32_t span, uint8_t flag, void *arg);

//取消定时执行
void SysTimerUnSet(SysTimer_t *timer);

#endif // SYSTIMER_H





