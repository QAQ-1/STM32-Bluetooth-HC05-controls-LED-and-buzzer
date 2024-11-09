#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "stm32f10x.h"

typedef struct{
	uint8_t recvBuff[10];	//数据接收缓冲区
	uint8_t recvflag;		//数据接收标志位
	uint8_t recvLen;		//数据接收长度
}usart_data_t;

typedef enum{
	ID_ERR = 1,
	DATA_ERR,
	LED_ON,
	LED_OFF,
	BUZZER_ON,
	BUZZER_OFF,
}usart_data_result_e;

typedef enum{
	PERIPH_STEP_CFG = 0,		//参数配置阶段
	PERIPH_STEP_TXM,		//数据传输阶段
}periph_comm_step_e;

typedef struct{
	uint8_t recv_data[128];
	uint8_t recv_len;
	uint8_t recv_flag;
	uint8_t comm_step;		//外围器件交互步骤 参数配置、数据透传
	uint8_t send_data[128];
	uint8_t send_len;
}bt_data_cache_t;


























#endif
