#ifndef TYPEDEF_H
#define TYPEDEF_H

#include "stm32f10x.h"

typedef struct{
	uint8_t recvBuff[10];	//���ݽ��ջ�����
	uint8_t recvflag;		//���ݽ��ձ�־λ
	uint8_t recvLen;		//���ݽ��ճ���
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
	PERIPH_STEP_CFG = 0,		//�������ý׶�
	PERIPH_STEP_TXM,		//���ݴ���׶�
}periph_comm_step_e;

typedef struct{
	uint8_t recv_data[128];
	uint8_t recv_len;
	uint8_t recv_flag;
	uint8_t comm_step;		//��Χ������������ �������á�����͸��
	uint8_t send_data[128];
	uint8_t send_len;
}bt_data_cache_t;


























#endif
