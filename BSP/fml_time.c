#include "fml_time.h"

void fml_delay_ms(uint16_t ms)//10000 -- 1000ms
{
	TIM_TimeBaseInitTypeDef TIM_InitSruct = {0};
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InitSruct.TIM_ClockDivision = 0;
	TIM_InitSruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_InitSruct.TIM_Prescaler = 7199;
	TIM_InitSruct.TIM_Period = ms * 10;
	TIM_TimeBaseInit(TIM2,&TIM_InitSruct);
	TIM_GenerateEvent(TIM2,TIM_EventSource_Update);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_Cmd(TIM2,ENABLE);
	while(!TIM_GetFlagStatus(TIM2,TIM_FLAG_Update));
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_Cmd(TIM2,DISABLE);
}

