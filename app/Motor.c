#include "Sys.h"
#include "Motor.h"

void MotorSpeedSet(uint8_t ch, MotorSpeedLevel_t level)
{
    uint16_t pluse = level * 20;
    
    HalPwmPluseSet(HAL_PWM_NUM_1, ch, pluse);
}

void MotorInitialize(void)
{
    HalPwmConfig(HAL_PWM_NUM_1, 20);
}

void MotorPoll(void)
{
}

