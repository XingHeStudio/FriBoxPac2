#include "SysTimer.h"

static SysTimer_t g_timers[SYS_TIMER_MAX_NUM];

void SysTimerInitialize()
{
    
}

static void timerExecute(SysTimer_t *timer)
{
    //not repeat
    if(!(timer->flag & SYS_TIMER_REPEAT))
    {
        //not need unset
        if(!(timer->flag & SYS_TIMER_NEED_UNSET))
        {
            timer->alloced = 0;
        }
        else
        {
            //set span to max, not execute again.
            timer->span = ~((uint16_t)0);
        }
    }
    else
    {
        timer->startTime = SysTime();
    }
    timer->run(timer->arg);
}

void SysTimerUnSet(SysTimer_t *timer)
{
    if(timer)
    {
        timer->alloced = 0;
    }
}

SysTimer_t *SysTimerSet(SysTimerRun_t run, uint32_t span, uint8_t flag, void *arg)
{
    int i;
    uint8_t istate;
    SysTimer_t *timer = NULL;
    
    //SYS_ENTER_CRITICAL(istate);
    for(i = 0; i < SYS_TIMER_MAX_NUM; i++)
    {
        if(g_timers[i].alloced)
        {
            continue;
        }
        timer = &g_timers[i];
        timer->alloced = 1;
        break;
    }
    //SYS_EXIT_CRITICAL(istate);
    
    if(!timer)
    {
        return NULL;
    }
    
    timer->arg = arg;
    timer->run = run;
    timer->span = span;
    timer->startTime = SysTime();
    timer->flag = flag;
    
    if(timer->flag & SYS_TIMER_IMME)
    {
        timerExecute(timer);
    }
    
    return timer;
}

void SysTimerPoll()
{
    int i;
    for(i = 0; i < SYS_TIMER_MAX_NUM; i++)
    {
        if(!g_timers[i].alloced)
        {
            continue;
        }
        
        if(SysTimeHasPast(g_timers[i].startTime, g_timers[i].span))
        {
            timerExecute(&g_timers[i]);
        }
    }
}

