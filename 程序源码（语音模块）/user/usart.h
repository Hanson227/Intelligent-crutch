#ifndef __USART_H
#define __USART_H

extern uint8_t RX_BUF[50];
extern uint8_t NUM_1;

void UartIni(void);//串口初始化
void UARTSendByte(uint8_t DAT);	//串口发送一字节数据
uint8_t UARTReceiveByte(void);
void PrintCom(uint8_t *DAT); //打印串口字符串数据

#endif /*__USART_H*/
