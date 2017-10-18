#ifndef HAL_COMMON_H
#define HAL_COMMON_H

#include "HalCtype.h"
#include "HalUart.h"
#include "HalGPIO.h"
#include "HalTimer.h"
#include "HalFlash.h"
#include "HalPwm.h"
#include "HalSPI.h"

void HalCommonStatusLedSet(uint8_t blink);
uint32_t HalGetSysTimeCount(void);
void HalInterruptSet(bool enable);
void HalTimerPast1ms(void);
void HalCommonInitialize(void);
void HalCommonPoll(void);

#endif

