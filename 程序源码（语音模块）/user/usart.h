#ifndef __USART_H
#define __USART_H

extern uint8_t RX_BUF[20];
extern uint8_t NUM_1;

void UartIni(void);//���ڳ�ʼ��
void UARTSendByte(uint8_t DAT);	//���ڷ���һ�ֽ�����
void PrintCom(uint8_t *DAT); //��ӡ�����ַ�������

#endif /*__USART_H*/
