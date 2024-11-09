#include "fml_bluetooth.h"

void fml_bt_state_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
}
#define USART2_IT_ENABLE	(1)
void fml_bt_uart_init(void)
{
	//结构体变量定义 
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	USART_InitTypeDef USART_InitStruct = {0};
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitStruct.USART_BaudRate            = 38400;
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;
	USART_InitStruct.USART_Parity              = USART_Parity_No ;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStruct);
	//中断配置
	#if USART2_IT_ENABLE
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	NVIC_SetPriorityGrouping(5);
	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(5,0,0));
	#endif
	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	
	/* Configure USART2 Rx (PA3) as input floating                           */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

  	/* Configure USART2 Tx (PA2) as alternate function push-pull              */
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


}

void fml_BtInit(void)
{
	fml_bt_uart_init();
	fml_bt_uart_data_clear();
	bt_usart.comm_step = PERIPH_STEP_CFG;		//外设配置步骤
	fml_bt_state_init();//蓝牙状态管脚初始化
}

void fml_bt_clear_st(void)
{
	bt_usart.recv_flag = 0;
}
uint8_t fml_bt_data_st(void)
{
	return bt_usart.recv_flag;
}

void fml_bt_uart_data_clear(void)
{
	memset(bt_usart.recv_data,0,sizeof(bt_usart.recv_data));
	bt_usart.recv_flag = 0;
	bt_usart.recv_len = 0;
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
}
bt_data_cache_t bt_usart;

void fml_bt_uart_data_send(char *data)
{
	while(*data != '\0')
	{
		USART_SendData(USART2,*data++);
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	}
	
}
uint8_t fml_bt_usart_data_handle(void)
{
	uint8_t cmd = 0;
	
	if (bt_usart.recv_data[1] != 0x02)
		return ID_ERR;
	
	switch(bt_usart.recv_data[2])
	{
		case 0x01://led
			if (bt_usart.recv_data[3] == 0x01)
				cmd = LED_ON;
			else if (bt_usart.recv_data[3] == 0x00)
				cmd = LED_OFF;
			else
				cmd = DATA_ERR;
		break;
		case 0x02://蜂鸣器
			if (bt_usart.recv_data[3] == 0x01)
				cmd = BUZZER_ON;
			else if (bt_usart.recv_data[3] == 0x00)
				cmd = BUZZER_OFF;
			else
				cmd = DATA_ERR;
		break;
		default:
			cmd = DATA_ERR;
		break;
	}
	return cmd;	
}

void USART2_IRQHandler(void)
{
	uint8_t temp = 0;
	
	if (USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		temp = USART_ReceiveData(USART2);
		switch(bt_usart.comm_step)
		{
			case PERIPH_STEP_CFG:
				bt_usart.recv_data[bt_usart.recv_len++] = temp;
			break;
			case PERIPH_STEP_TXM:
				if((bt_usart.recv_len == 0) && (temp != 0x55))
				{
					fml_bt_uart_data_clear();
					return;
				}
				else
				{
					USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
					bt_usart.recv_flag = 1;
				}
				bt_usart.recv_data[bt_usart.recv_len++] = temp;
			break;
		}
		
	}
}









