#ifndef HAL_PWM_H
#define HAL_PWM_H

#include "HalCtype.h"

#define HAL_PWM_NUM_1 1
#define HAL_PWM_NUM_2 2

void HalPwmPluseSet(uint8_t pwmNo, uint8_t chn, uint16_t pluse);
void HalPwmConfig(uint8_t pwmNo, uint16_t defPluse);
void HalPwmInitialize(void);
void HalPwmPoll(void);

#endif

