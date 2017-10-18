#include "HalGPIO.h"

#define IO_NUM_MAX 0x5f

static GPIO_TypeDef *ioToPort(uint8_t io)
{
    int n = io >> 4;

    switch(n)
    {
    case 0x0:
        return GPIOA;

    case 0x1:
        return GPIOB;

    case 0x2:
        return GPIOC;

    case 0x3:
        return GPIOD;

    case 0x4:
        return GPIOE;

    case 0x5:
        return GPIOF;

    default:
        return GPIOG;
    }
}

static uint16_t ioToPin(uint8_t io)
{
    int n = io & 0xf;
    return (1 << n);
}

uint8_t HalGPIOGetLevel(uint8_t io)
{
    return GPIO_ReadInputDataBit(ioToPort(io), ioToPin(io));
}

void HalGPIOSetLevel(uint8_t io, uint8_t level)
{
    GPIO_WriteBit(ioToPort(io), ioToPin(io), level);
}

void HalGPIOConfig(uint8_t io, HalGPIODirect_t dir)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if(io > IO_NUM_MAX)
    {
        return;
    }
    
    GPIO_InitStruct.GPIO_Pin = ioToPin(io);
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    if(dir == HAL_IO_OUTPUT)
    {
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    }
    else
    {
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    }
    
    GPIO_Init(ioToPort(io), &GPIO_InitStruct);
}

void HalGPIOInitialize(void)
{
}

void HalGPIOPoll(void)
{
}

