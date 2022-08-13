#ifndef __USART_H
#define __USART_H

extern uint8_t RX_BUF[20];
extern uint8_t NUM_1;

void UartIni(void);//串口初始化
void UARTSendByte(uint8_t DAT);	//串口发送一字节数据
void PrintCom(uint8_t *DAT); //打印串口字符串数据

#endif /*__USART_H*/
