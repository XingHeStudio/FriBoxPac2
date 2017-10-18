#include "HalUart.h"

HalUartConfig_t g_uartConfig[HAL_UART_COUNT];

void HalUartConfig(HalUartPort_t uart, HalUartConfig_t *config)
{   
    USART_InitTypeDef uartcfg;
    USART_TypeDef *uartNo;
    
    USART_StructInit(&uartcfg);
    if(HAL_UART_PORT_1 == uart)
    {
        uartNo = USART1;
    }
    else if(HAL_UART_PORT_2 == uart)
    {
        uartNo = USART2;
    }
    else if(HAL_UART_PORT_3 == uart)
    {
        uartNo = USART3;
    }
    else
    {
        return;
    }
    g_uartConfig[uart] = *config;

    USART_Cmd(uartNo, DISABLE);
    USART_ITConfig(uartNo, USART_IT_RXNE, DISABLE);
    
    uartcfg.USART_BaudRate = config->baudrate;
    uartcfg.USART_WordLength = config->wordLength;
    uartcfg.USART_Parity = config->parity;
    uartcfg.USART_StopBits = USART_StopBits_1;
    uartcfg.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uartcfg.USART_HardwareFlowControl = config->flowControl;

    USART_Init(uartNo, &uartcfg);
    USART_ClearITPendingBit(uartNo, USART_IT_RXNE);
    USART_ITConfig(uartNo, USART_IT_RXNE, ENABLE);
    USART_Cmd(uartNo, ENABLE);
}

void HalUartInitialize(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    GPIO_InitTypeDef GPIO_InitStructure;

    //uart1 io
    //GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //uart2 io
    //GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;  //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //uart3 io
    //GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;  //TX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
     
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;  //RX
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void HalUartPoll(void)
{
}

void HalUartWrite(HalUartPort_t uart, const uint8_t *data, uint16_t len)
{
    uint16_t i;
    USART_TypeDef *uartNo;
    
    if(HAL_UART_PORT_1 == uart)
    {
        uartNo = USART1;
    }
    else if(HAL_UART_PORT_2 == uart)
    {
        uartNo = USART2;
    }
    else
    {
        uartNo = USART3;
    }

    for(i = 0; i < len; i++)
    {
        USART_SendData(uartNo, (uint16_t)data[i]);
        while(USART_GetFlagStatus(uartNo, USART_FLAG_TC) == RESET);
    }
    
}

void USART1_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        data = USART_ReceiveData(USART1);
        if(g_uartConfig[HAL_UART_PORT_1].recvCb != NULL)
        {
            g_uartConfig[HAL_UART_PORT_1].recvCb(&data, 1);
        }
    }
}

void USART2_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        data = USART_ReceiveData(USART2);
        if(g_uartConfig[HAL_UART_PORT_2].recvCb != NULL)
        {
            g_uartConfig[HAL_UART_PORT_2].recvCb(&data, 1);
        }
    }
}

void USART3_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        data = USART_ReceiveData(USART3);
        if(g_uartConfig[HAL_UART_PORT_3].recvCb != NULL)
        {
            g_uartConfig[HAL_UART_PORT_3].recvCb(&data, 1);
        }
    }
}


