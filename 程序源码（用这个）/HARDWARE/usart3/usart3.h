#ifndef __USART3_H
#define	__USART3_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"	     
#include "string.h"     

/*缓冲区大小宏定义和变量声明*/
#define USART3_TXBUFF_SIZE   1024   //定义串口3 发送缓冲区大小 1024字节
#define USART3_RXBUFF_SIZE   1024              //定义串口3 接收缓冲区大小 1024字节
extern char Usart3_RxCompleted ;               //外部声明，其他文件可以调用该变量
extern unsigned int Usart3_RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量

/*函数声明*/
void Usart3_Init(u32 bound);
void u3_printf(char*,...) ;

#endif /* __USART3_H */

