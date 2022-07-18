#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"	     
#include "string.h"     

/*��������С�궨��ͱ�������*/
#define USART2_TXBUFF_SIZE   1024   		   //���崮��2 ���ͻ�������С 1024�ֽ�
#define USART2_RXBUFF_SIZE   1024              //���崮��2 ���ջ�������С 1024�ֽ�

/*��������*/
void Usart2_Init(u32 bound);
void u2_printf(char*,...) ;
void u2_TxData(unsigned char *data);

#endif /*__USART2_H*/
