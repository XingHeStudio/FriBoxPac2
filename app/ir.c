#include "ir.h"
#include "ili9341.h"

#define IR_INPUT_PIN  0x0f

void IRInitialize(void)
{
    HalGPIOConfig(IR_INPUT_PIN, HAL_IO_INPUT);
}

void IRPoll(void)
{
    static uint8_t oldStatus = 0;
    char buff[10];

    if(HalGPIOGetLevel(IR_INPUT_PIN) != oldStatus)
    {
        oldStatus = HalGPIOGetLevel(IR_INPUT_PIN);
        //SysLog("ir level %d", oldStatus);
        sprintf(buff, "IR pin:%d", oldStatus);
        Ili9341LCDDisplayString(0, 50, 24, 48, buff, LCD_COLOR_GRAY);
    }
}

