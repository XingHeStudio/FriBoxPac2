#ifndef HAL_WAIT_H
#define HAL_WAIT_H

#include "HalCtype.h"

void HalWaitMs(uint16_t ms);
void HalWaitUs(uint16_t us);
void HalWaitInitialize(void);
void HalWaitPoll(void);
#endif 

