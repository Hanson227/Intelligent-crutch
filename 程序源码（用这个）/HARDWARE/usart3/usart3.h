#ifndef __USART3_H
#define	__USART3_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"	     
#include "string.h"     

/*��������С�궨��ͱ�������*/
#define USART3_TXBUFF_SIZE   1024   //���崮��3 ���ͻ�������С 1024�ֽ�
#define USART3_RXBUFF_SIZE   1024              //���崮��3 ���ջ�������С 1024�ֽ�
extern char Usart3_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart3_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���

/*��������*/
void Usart3_Init(u32 bound);
void u3_printf(char*,...) ;

#endif /* __USART3_H */

