#include "app_bluetooth.h"
#include "fml_bluetooth.h"
#include "fml_time.h"
#include "fml_gpio.h"

void BluetoothInit(void)
{
	fml_bt_uart_init();
	fml_bt_uart_data_clear();
	fml_bt_state_init();//蓝牙状态管脚初始化
	bt_usart.comm_step = PERIPH_STEP_CFG;		//外设配置步骤
}
void BluetoothCtrlTask(void)
{	
	uint8_t cmd = 0;
	if (fml_bt_data_st() != 1)
		return;
	
	cmd = fml_bt_usart_data_handle();
	
	switch(cmd)
	{
		case LED_ON:
			LED_PutVal(Bit_RESET);
			printf("led on\r\n");
		break;
		case LED_OFF:
			LED_PutVal(Bit_SET);
			printf("led off\r\n");
		break;		
		case BUZZER_ON:
			BUZZER_PutVal(Bit_RESET);
			printf("buzzer on\r\n");
		break;		
		case BUZZER_OFF:
			BUZZER_PutVal(Bit_SET);
			printf("buzzer off\r\n");
		break;	
	}
	fml_bt_uart_data_clear();	
}
/*
 *	蓝牙底层串口初始化完成后，进行参数配置
 *
*/
void BluetoothConfig(void)
{
	static uint8_t step = 0;
	static uint8_t temp = 0;
	
	if (BT_STATE_READ() == 1)
	{
		bt_usart.comm_step = PERIPH_STEP_TXM;		//外设配置步骤
		printf("> bluetooth connect success\r\n");
		return;
	}
	while(step != 5)
	{
		switch(step)
		{
			case 0:
				temp = BluetoothCmdSend("AT\r\n","OK\r\n",NULL,500);//测试AT指令
				if (temp)
					step = 1;
			break;
			case 1:
				BluetoothCmdSend("AT+NAME=HC-05\r\n","OK\r\n",NULL,500);//修改名称
				temp = BluetoothCmdSend("AT+NAME?\r\n","+NAME:HC-05\r\n","OK\r\n",500);  //读取修改的名称
				if (temp)
					step = 2;
			break;
			case 2:
				BluetoothCmdSend("AT+PSWD=\"1234\"\r\n","OK\r\n",NULL,500);//修改配对码
				temp = BluetoothCmdSend("AT+PSWD?\r\n","+PSWD:1234\r\n","OK\r\n",500);
				if (temp)
					step = 3;
			break;
			case 3:				
				BluetoothCmdSend("AT+ROLE=0\r\n","+ROLE:0\r\n","OK\r\n",500);
				temp =  BluetoothCmdSend("AT+ROLE?\r\n","+ROLE:0\r\n","OK\r\n",500);
				if (temp)
					step = 4;
			case 4:
				BluetoothCmdSend("AT+UART=38400,0,0\r\n","OK\r\n",NULL,500);
				temp = BluetoothCmdSend("AT+UART?\r\n","+UART:38400,0,0\r\n","OK\r\n",500);
				if (temp)
					BluetoothCmdSend("AT+RESET\r\n","OK\r\n",NULL,100);//复位
				while(BT_STATE_READ() != 1);
				step = 5;
				bt_usart.comm_step = PERIPH_STEP_TXM;		//外设配置步骤
				printf("> bluetooth connect success\r\n");
			break;

		}
		
	}
	
	step = 0;
}

/**
　* @brief  蓝牙AT指令发送函数
　* @param  cmd:指令
  * @param  ack1:该指令的响应1
  * @param  ack2:该指令的响应2
  * @param  timeout:该指令的超时上限
　* @retval uint8_t 1 NO_ERR 0:ERR
  */
uint8_t BluetoothCmdSend(char *cmd,char *ack1,char *ack2,u16 timeout)
{
	u8 result = 0;
	
	fml_bt_uart_data_send(cmd);
	fml_delay_ms(timeout);

	printf("> %s\r\n",bt_usart.recv_data);
	if ( (ack1 != NULL) && (ack2 != NULL) )                     //该指令有两条应答
	{
		result = strstr((const char *)bt_usart.recv_data,ack1) ||      //是否包含第一条应答 
						 strstr((const char *)bt_usart.recv_data,ack2);        //是否包含第二条应答 
	}
	else if (ack1 != NULL)                                      //该指令只有ack1应答
	{
		result = strstr((const char *)bt_usart.recv_data,ack1) != NULL;
	}
	else if (ack2 != NULL)                                                       //该指令只有ack2应答 
	{
		result = strstr((const char *)bt_usart.recv_data,ack2) != NULL;
	}	
	fml_bt_uart_data_clear();	
	return result;
}




















