/**
  ******************************************************************************
  * @file    main.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-11
  * @brief   ������
  *			 CPU: STC11L08XE
  *      	 ����22.1184MHZ
  *		     �����ʣ�9600 bit/S
  ******************************************************************************
  */

#include "config.h"

uint8 idata nAsrStatus = 0;
void MCU_init();
void ProcessInt0(); //ʶ������
void delay(unsigned long uldata);
void User_handle(uint8 dat);//�û�ִ�в�������
void Delay200ms();
void Led_test(void);//��Ƭ������ָʾ
uint8_t G0_flag = DISABLE; //���б�־��ENABLE:���С�DISABLE:��ֹ����
sbit LED = P4 ^ 2; //�ź�ָʾ��

sbit SRD1 = P1 ^ 7;
sbit SRD2 = P1 ^ 6;
sbit SRD3 = P1 ^ 5;
sbit SRD4 = P1 ^ 4;

char P_cmd_arr[50] = {0};

uint8 P_cmd_num = 100;

 /**
  * @brief  ������	�������
  * @param  ��
  * @retval ��
  */
void  main(void)
{
	char time_count = 0;
	uint8 idata nAsrRes;
	char cmd = 0;
	uint8 i = 0;
	P1M0 = 0xFF;
	P1M1 = 0x00;
	SRD1 = SRD2 = SRD3 = SRD4 = 0;
	Led_test();
	MCU_init();
	LD_Reset();
	UartIni(); /*���ڳ�ʼ��*/
	nAsrStatus = LD_ASR_NONE;		//	��ʼ״̬��û������ASR
	PrintCom("<G>����С�����ܸ���Ϊ������");
	while(1)
	{
		switch(RX_BUF[NUM_1])
		{
			case '1':
				memset(P_cmd_arr,0,20);
				memcpy(P_cmd_arr,"<G>ǰ��������",strlen("<G>ǰ��������"));
				break;
			case '2':
				memset(P_cmd_arr,0,20);
				memcpy(P_cmd_arr,"<G>ǰ��¥��",strlen("<G>ǰ��¥��"));
				break;
			case '3':
				memset(P_cmd_arr,0,20);
				memcpy(P_cmd_arr,"<G>ǰ�����ϰ���",strlen("<G>ǰ�����ϰ���"));
				break;
			case '4':
				memset(P_cmd_arr,0,20);
				memcpy(P_cmd_arr,"<G>��⵽����ˤ�������°���ȡ��",strlen("<G>��⵽����ˤ�������°���ȡ��"));
				break;
			default:
				break;
		}
		switch(nAsrStatus)
		{
		case LD_ASR_RUNING:
		case LD_ASR_ERROR:
			break;
		case LD_ASR_NONE:
		{
			nAsrStatus = LD_ASR_RUNING;
			if (RunASR() == 0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
			{
				nAsrStatus = LD_ASR_ERROR;
			}
			break;
		}
		case LD_ASR_FOUNDOK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
		{
			nAsrRes = LD_GetResult();		/*��ȡ���*/
			User_handle(nAsrRes);//�û�ִ�к���
			nAsrStatus = LD_ASR_NONE;
			break;
		}
		case LD_ASR_FOUNDZERO:
		default:
		{
			nAsrStatus = LD_ASR_NONE;
			break;
		}
		}
	}

}

 /**
  * @brief  ����LED�Ʋ���
  * @param  ��
  * @retval ��
  */
void Led_test(void)
{
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
	Delay200ms();
	LED = ~ LED;
}

 /**
  * @brief  ��Ƭ����ʼ��
  * @param  ��
  * @retval ��
  */
void MCU_init()
{
	P0 = 0xff;
	P1 = 0x00;
	P2 = 0xff;
	P3 = 0xff;
	P4 = 0xff;


	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD |= 0x01;		//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x28;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0 = 1;

	LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
	IE0 = 1;
	EX0 = 1;
	EA = 1;
	WDT_CONTR = 0x3D;
}

 /**
  * @brief  ��ʱ����
  * @param  ��
  * @retval ��
  */
void Delay200us()		//@22.1184MHz
{
	unsigned char i, j;
	_nop_();
	_nop_();
	i = 5;
	j = 73;
	do
	{
		while (--j);
	}
	while (--i);
}

 /**
  * @brief  ��ʱ����
  * @param  uldata����λ200us
  * @retval ��
  */
void  delay(unsigned long uldata)
{
	unsigned int j  =  0;
	unsigned int g  =  0;
	while(uldata--)
		Delay200us();
}

 /**
  * @brief  ��ʱ����
  * @param  ��
  * @retval ��
  */
void Delay200ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	i = 17;
	j = 208;
	k = 27;
	do
	{
		do
		{
			while (--k);
		}
		while (--j);
	}
	while (--i);
}

 /**
  * @brief  �жϴ�����
  * @param  ��
  * @retval ��
  */
void ExtInt0Handler(void) interrupt 0
{
	ProcessInt0();
}

 /**
  * @brief  �û�ִ�к�����ʶ��ɹ���ִ�ж������ڴ˽����޸�
  * @param  ��
  * @retval ��
  */
void User_handle(uint8 dat)
{
	switch(dat)
	{
	case CODE_1:	 	/*�����*/
		SRD1 = 1;
		PrintCom("<G>С���ڵ�\r\n");
		break;
	case CODE_2:	 	/*�����*/
		SRD1 = 0;
		PrintCom("<G>�ڵ�\r\n");
		break;
	case CODE_3:		/*�����*/
		SRD2 = 1;
		PrintCom("<G>����\r\n");
		break;
	default:/*text.....*/
		break;
	}
}

 /**
  * @brief  ��ʱ���жϺ���
  * @param  ��
  * @retval ��
  */
void tm0_isr() interrupt 1
{
	TL0 = 0x00;		//���ö�ʱ��ֵ,���40ms
	TH0 = 0x28;		//���ö�ʱ��ֵ
	WDT_CONTR = 0x3D;
	P_cmd_num--;
	if(P_cmd_num==0)
	{
		PrintCom(P_cmd_arr);
		memset(P_cmd_arr,0,20);
		P_cmd_num = 100;
	}
}
