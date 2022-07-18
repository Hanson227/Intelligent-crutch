#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include "stdio.h"
#include "stdarg.h"	     
#include "string.h"     

/*缓冲区大小宏定义和变量声明*/
#define USART2_TXBUFF_SIZE   1024   		   //定义串口2 发送缓冲区大小 1024字节
#define USART2_RXBUFF_SIZE   1024              //定义串口2 接收缓冲区大小 1024字节

/*函数声明*/
void Usart2_Init(u32 bound);
void u2_printf(char*,...) ;
void u2_TxData(unsigned char *data);

#endif /*__USART2_H*/
