#ifndef __USART_H
#define __USART_H

void UartIni(void);//���ڳ�ʼ��
void UARTSendByte(uint8_t DAT);	//���ڷ���һ�ֽ�����
uint8_t UARTReceiveByte(void);
void PrintCom(uint8_t *DAT); //��ӡ�����ַ�������

#endif /*__USART_H*/
