#include "SysButton.h"

#define SYS_CFG_BUTTON_LIMIT 5

static void handleButton(SysButton_t *button);

static SysButton_t *g_buttons[SYS_CFG_BUTTON_LIMIT] = {NULL};

void SysButtonInitialize(void)
{
    
}

void SysButtonPoll(void)
{
    int i;
    for(i = 0; i < SYS_CFG_BUTTON_LIMIT; i++)
    {
        if(g_buttons[i])
        {
            handleButton(g_buttons[i]);
        }
    }
}

uint8_t SysButtonRegister(SysButton_t *button, SysButtonHandler_t handler, SysButtonGetState_t getState)
{
    int i;
    for(i = 0; i < SYS_CFG_BUTTON_LIMIT; i++)
    {
        if(g_buttons[i] == NULL)
        {
            g_buttons[i] = button;
            button->handled = 0;
            button->lastState = SYS_BUTTON_STATE_RELEASED;
            button->pressedTime = 0;
            button->getState = getState;
            button->handler = handler;
            return 1;
        }
    }
    return 0;
}

void SysButtonUnregister(SysButton_t *button)
{
    int i;
    for(i = 0; i < SYS_CFG_BUTTON_LIMIT; i++)
    {
        if(g_buttons[i] == button)
        {
            g_buttons[i] = NULL;
        }
    }
}

static void handleButton(SysButton_t *button)
{
    SysTime_t curTime = SysTime();
    
    if(button->lastState == SYS_BUTTON_STATE_PRESSED)
    {
        button->lastState = button->getState(button);
        if(button->handler(button, curTime - button->pressedTime, button->lastState))
        {
            button->handled = 0;
            button->pressedTime = curTime;
        }
    }
    else
    {
        button->lastState = button->getState(button);
        if(button->lastState == SYS_BUTTON_STATE_PRESSED)
        {
            button->handled = 0;
            button->pressedTime = curTime;
        }
    }
}
