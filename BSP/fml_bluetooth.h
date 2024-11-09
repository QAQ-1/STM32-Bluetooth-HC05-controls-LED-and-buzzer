#ifndef FML_BLUEBOOTH_H
#define FML_BLUEBOOTH_H


#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "typedef.h"

#define BT_STATE_READ()		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)






extern bt_data_cache_t bt_usart;






void fml_bt_uart_init(void);
void fml_bt_state_init(void);
void fml_BtInit(void);
void fml_bt_uart_data_clear(void);
uint8_t fml_bt_data_st(void);
void fml_bt_uart_data_send(char *data);
uint8_t fml_bt_usart_data_handle(void);

#endif
