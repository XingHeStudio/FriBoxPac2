#ifndef HAL_FLASH_H
#define HAL_FLASH_H

#include "HalCtype.h"

#define HAL_FLASH_BLOCK_SIZE 2048

void HalFlashInitialize(void);
void HalFlashPoll(void);

void HalFlashErase(uint32_t addr);
void HalFlashWrite(uint32_t addr, const void *data, uint32_t len);
void HalFlashRead(uint32_t addr, void *buf, uint32_t bufSize);

#endif // HAL_FLASH_H
