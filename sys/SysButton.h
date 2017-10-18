#ifndef SYSBUTTON_H
#define SYSBUTTON_H
#include "Sys.h"

void SysButtonInitialize(void);
void SysButtonPoll(void);

//按键状态
typedef enum
{
    SYS_BUTTON_STATE_RELEASED, //未按下
    SYS_BUTTON_STATE_PRESSED,  //按下
}SysButtonState_t;

struct SysButton_st;
typedef struct SysButton_st SysButton_t;

//返回1 表示按住重复触发按键
typedef uint8_t (*SysButtonHandler_t)(SysButton_t *button, SysTime_t pressTime, SysButtonState_t state);
typedef SysButtonState_t (*SysButtonGetState_t)(SysButton_t *button);

struct SysButton_st
{
    SysButtonState_t lastState;
    uint8_t handled;
    SysTime_t pressedTime;
    SysButtonHandler_t handler;
    SysButtonGetState_t getState;
};

//注册按键
uint8_t SysButtonRegister(SysButton_t *button, SysButtonHandler_t handler, SysButtonGetState_t getState);


#endif // SYSBUTTON_H
