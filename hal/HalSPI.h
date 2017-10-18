#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "HalCtype.h"

typedef enum
{
    HAL_SPI_PORT_1 = 0,
    HAL_SPI_PORT_2,
}HalSPIPort_t;

void HalSPIInitialize(void);
void HalSPIPoll(void);
unsigned char HalSPIReadWrite(HalSPIPort_t port, unsigned char data);
void HalSPIDmaWrite(uint8_t *buff, uint16_t len);

#endif

