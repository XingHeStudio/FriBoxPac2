#include "HalTimer.h"
#include "stm32f10x_tim.h"


extern void HalTimerPast1ms(void);

void TIM1_UP_IRQHandler(void)
{
    if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
        HalTimerPast1ms();
    }
}

void HalTimerInitialize(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

    TIM_TimeBaseInitStruct.TIM_Prescaler = 1000 - 5;
    TIM_TimeBaseInitStruct.TIM_Period = 72 - 1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

void HalTimerPoll(void)
{
}

