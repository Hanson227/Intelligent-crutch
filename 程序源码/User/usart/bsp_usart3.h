#ifndef __USART_H
#define	__USART_H

#include "stm32f4xx.h"
#include "stdio.h" 
#include "stdarg.h"
#include "string.h"


//���Ŷ���
/*******************************************************/
#define USARTx                             USART3

/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ������ֲʱҪע�� 
 * ����1��6��      RCC_APB2PeriphClockCmd
 * ����2/3/4/5/7�� RCC_APB1PeriphClockCmd
 */
#define USART3_CLK                         RCC_APB1Periph_USART3
#define USART3_CLOCKCMD                    RCC_APB1PeriphClockCmd
#define USART3_BAUDRATE                    115200  //���ڲ�����

#define USART3_RXBUFF_SIZE   1024              //���崮��3 ���ջ�������С 1024�ֽ�
extern char Usart3_RxCompleted ;               //�ⲿ�����������ļ����Ե��øñ���
extern unsigned int Usart3_RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���

#define USART3_RX_GPIO_PORT                GPIOB
#define USART3_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define USART3_RX_PIN                      GPIO_Pin_11
#define USART3_RX_AF                       GPIO_AF_USART3
#define USART3_RX_SOURCE                   GPIO_PinSource11

#define USART3_TXBUFF_SIZE   			   1024   //���崮��3 ���ͻ�������С 1024�ֽ�

#define USART3_TX_GPIO_PORT                GPIOB
#define USART3_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define USART3_TX_PIN                      GPIO_Pin_10
#define USART3_TX_AF                       GPIO_AF_USART3
#define USART3_TX_SOURCE                   GPIO_PinSource10

#define SIM800_USART_IRQHandler			   USART3_IRQHandler
/************************************************************/

void USARTx_Config(void);
void u3_printf(char*,...) ;          
void u3_TxData(unsigned char *data);

#endif /* __USART_H */
