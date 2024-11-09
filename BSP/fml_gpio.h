#ifndef HDL_GPIO_H
#define HDL_GPIO_H

#include "stm32f10x.h"

#define LED_CONTROL_PIN		(GPIO_Pin_0 | GPIO_Pin_1)
#define LED_PutVal(val)		(GPIO_WriteBit(GPIOA,LED_CONTROL_PIN,val))
#define BUZZER_CONTROL_PIN		(GPIO_Pin_12)
#define BUZZER_PutVal(val)		(GPIO_WriteBit(GPIOB,BUZZER_CONTROL_PIN,val))




void fml_led_init(void);
void fml_buzzer_init(void);

#endif
