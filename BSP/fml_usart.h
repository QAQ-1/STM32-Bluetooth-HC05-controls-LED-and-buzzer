#ifndef HDL_USART_H
#define HDL_USART_H


#include "stm32f10x.h"
#include "stdio.h"
#include "typedef.h"



extern usart_data_t fml_uartRecv;




void fml_usart_init(void);
uint8_t fml_usart_data_st(void);
void fml_usart_data_clear(void);
uint8_t fml_usart_data_handle(void);
void UsartCtrlTask(void);



#endif
