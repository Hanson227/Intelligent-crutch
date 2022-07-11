#ifndef __BSP_SIM800A_H
#define __BSP_SIM800A_H

#include "./usart3/usart3.h"	    
	    	
#define POWER_STA       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)        //PC1的电平，可以判断800C是开机还是关机
#define POWER_KEY(x)    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)x)  //PC0控制800C的开关机

#define SIM800C_printf       u3_printf           //串口2控制 800C
#define SIM800C_RxCounter    Usart3_RxCounter    //串口2控制 800C
#define SIM800C_RX_BUF       Usart3_RxBuff       //串口2控制 800C
#define SIM800C_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //串口2控制 800C
 
void SIM800C_GPIO_Init(void);
char SIM800C_Power(void);
char SIM800C_SendCmd(char *, int);
char SIM800C_CREG(int);
char SIM800C_CSQ(int timeout);
char SIM800C_CGATT(int timeout);
char SIM800C_ActivateNetwork(void);
char SIM800C_TCPConnect(int);
void SIM800C_TxData(unsigned char *);
char SIM800C_Connect_IoTServer(void);

#endif /*__BSP_SIM800A_H*/


