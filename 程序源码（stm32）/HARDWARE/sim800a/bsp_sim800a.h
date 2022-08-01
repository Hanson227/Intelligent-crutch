#ifndef __BSP_SIM800A_H
#define __BSP_SIM800A_H

#include "./usart3/usart3.h"	    
	    	
#define POWER_STA       GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)        //PC1�ĵ�ƽ�������ж�800C�ǿ������ǹػ�
#define POWER_KEY(x)    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (BitAction)x)  //PC0����800C�Ŀ��ػ�

#define SIM800A_printf       u3_printf           //����2���� 800C
#define SIM800A_RxCounter    Usart3_RxCounter    //����2���� 800C
#define SIM800A_RX_BUF       Usart3_RxBuff       //����2���� 800C
#define SIM800A_RXBUFF_SIZE  USART3_RXBUFF_SIZE  //����2���� 800C
#define EMERGENCY_CALL_CMD		 "AT+CMGS=\"+8613106944560\""

char SIM800A_SendCmd(char *, int);
char SIM800A_Note_Edit(char *call_cmd);
char SIM800A_Note_Send(int timeout);
char SIM800A_CREG(int);
char SIM800A_CSQ(int timeout);
char SIM800A_CGATT(int timeout);
char SIM800A_ActivateNetwork(void);
char SIM800A_TCPConnect(int);
void SIM800A_TxData(unsigned char *);
char SIM800A_Connect_IoTServer(void);

#endif /*__BSP_SIM800A_H*/


