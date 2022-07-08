#ifndef __USART_H
#define	__USART_H

#include "stm32f4xx.h"
#include "stdio.h" 
#include "stdarg.h"
#include "string.h"


//引脚定义
/*******************************************************/
#define USARTx                             USART3

/* 不同的串口挂载的总线不一样，时钟使能函数也不一样，移植时要注意 
 * 串口1和6是      RCC_APB2PeriphClockCmd
 * 串口2/3/4/5/7是 RCC_APB1PeriphClockCmd
 */
#define USART3_CLK                         RCC_APB1Periph_USART3
#define USART3_CLOCKCMD                    RCC_APB1PeriphClockCmd
#define USART3_BAUDRATE                    115200  //串口波特率

#define USART3_RXBUFF_SIZE   1024              //定义串口3 接收缓冲区大小 1024字节
extern char Usart3_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart3_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量

#define USART3_RX_GPIO_PORT                GPIOB
#define USART3_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define USART3_RX_PIN                      GPIO_Pin_11
#define USART3_RX_AF                       GPIO_AF_USART3
#define USART3_RX_SOURCE                   GPIO_PinSource11

#define USART3_TXBUFF_SIZE   			   1024   //定义串口3 发送缓冲区大小 1024字节

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
