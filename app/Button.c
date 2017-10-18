#include "Sys.h"
#include "SysButton.h"
#include "ili9341.h"
#include "Motor.h"

#define BUTTON_POWER_PIN 0x2a
#define BUTTON_SPEED_PIN 0x2b

static SysButton_t g_powerButton;
static SysButton_t g_gearButton;

static bool g_poweron = false;
static MotorSpeedLevel_t g_speedlevel = 1;

static SysButtonState_t getButtonStatus(SysButton_t *button)
{
    if(button == &g_powerButton)
    {
        if(HalGPIOGetLevel(BUTTON_POWER_PIN))
        {
            return SYS_BUTTON_STATE_RELEASED;
        }
        else
        {
            return SYS_BUTTON_STATE_PRESSED;
        }
    }
    else if(button == &g_gearButton)
    {
        if(HalGPIOGetLevel(BUTTON_SPEED_PIN))
        {
            return SYS_BUTTON_STATE_RELEASED;
        }
        else
        {
            return SYS_BUTTON_STATE_PRESSED;
        }
    }
    return SYS_BUTTON_STATE_RELEASED;
}

static uint8_t buttonHandle(SysButton_t *button, SysTime_t pressTime, SysButtonState_t state)
{
    char buff[16] = "";
    if(state == SYS_BUTTON_STATE_RELEASED && pressTime > 20)
    {
        if(button == &g_powerButton)
        {
            g_poweron = !g_poweron;
            if(g_poweron)
            {
                Ili9341LCDDisplayString(0, 200, 24, 48, "POWER:ON ", LCD_COLOR_GRAY);
            }
            else
            {
                Ili9341LCDDisplayString(0, 200, 24, 48, "POWER:OFF", LCD_COLOR_GRAY);
            }
        }
        else if(button == &g_gearButton)
        {
            g_speedlevel++;
            if(g_speedlevel > 3)
            {
                g_speedlevel = 1;
            }
            MotorSpeedSet(1, g_speedlevel);
            MotorSpeedSet(2, g_speedlevel);
            MotorSpeedSet(3, g_speedlevel);
            MotorSpeedSet(4, g_speedlevel);
            sprintf(buff, "SPEED:%02d", g_speedlevel);
            Ili9341LCDDisplayString(0, 250, 24, 48, buff, LCD_COLOR_GRAY);
        }
        return 1;
    }
    return 0;
}

void ButtonInitialize(void)
{
    HalGPIOConfig(BUTTON_POWER_PIN, HAL_IO_INPUT);
    HalGPIOConfig(BUTTON_SPEED_PIN, HAL_IO_INPUT);

    SysButtonInitialize();
    
    SysButtonRegister(&g_powerButton, buttonHandle, getButtonStatus);
    SysButtonRegister(&g_gearButton, buttonHandle, getButtonStatus);
    Ili9341LCDDisplayString(0, 200, 24, 48, "POWER:OFF", LCD_COLOR_GRAY);
    Ili9341LCDDisplayString(0, 250, 24, 48, "SPEED:01", LCD_COLOR_GRAY);
}

void ButtonPoll(void)
{
    SysButtonPoll();
}

