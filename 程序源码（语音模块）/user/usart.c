#include "config.h"
#define FOSC 22118400L      //System frequency
uint32_t baud=9600;           //UART baudrate

uint8_t RX_BUF[50];
uint8_t NUM_1=0;

 /**
  * @brief  ���ڳ�ʼ��,STC10L08XE ��Ƭ�����ڳ�ʼ������
  * @param  ��
  * @retval ��
  */
void UartIni(void)
{
    SCON = 0x50;            //8-bit variable UART
    TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/baud); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}

 /**
  * @brief  ���ڷ���һ�ֽ�����
  * @param  DAT:�����͵�����
  * @retval ��
  */
void UARTSendByte(uint8_t DAT)
{
	ES  =  0;
	TI=0;
	SBUF = DAT;
	while(TI==0);
	TI=0;
	ES = 1;
}

 /**
  * @brief  ���ڷ����ַ�������
  * @param  *DAT���ַ���ָ��
  * @retval ��
  */
void PrintCom(uint8_t *DAT)
{
	while(*DAT)
	{
	 	UARTSendByte(*DAT++);
	}	
}

 /**
  * @brief  �����жϺ���
  * @param  ��
  * @retval ��
  */
void Uart_Isr(void) interrupt 4 using 1
{
    if (RI)
    {
			RI=0;
			RX_BUF[NUM_1]=SBUF;
			NUM_1++;
			if(NUM_1>=49)
				NUM_1=0;
			if(NUM_1>=3)
			{
				if(RX_BUF[NUM_1]==0xF8&&RX_BUF[NUM_1-1]==0xF8&&RX_BUF[NUM_1-2]==0xF8)
					IAP_CONTR=0x60;		
			}		
		}
}
