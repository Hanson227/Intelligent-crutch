/**
  ******************************************************************************
  * @file    usart2.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-14
  * @brief   ʹ�ô���2��K210���ݽ��գ�������ģ�鷢������
  ******************************************************************************
  */
  
#include "./usart2/usart2.h"  
#include "stm32f4xx.h"
#include "./timer/timer4.h"



 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}


 /**
  * @brief  USART2 GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Usart2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
			
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* ʹ�� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);
	
	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
			
	/* ���ô��� USART2 ģʽ */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	
	NVIC_Configuration();
	/*���ô��ڽ����ж�*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART2, ENABLE);
}

 /**
  * @brief  �����������
  * @param  fmt�������ַ���
  * @retval ��
  */
__align(8) char Usart2_TxBuff[USART2_TXBUFF_SIZE];
void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = Usart2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}

 /**
  * @brief  ����2���ͻ������е�����
  * @param  data������
  * @retval ��
  */
void u2_TxData(unsigned char *data)
{
	int	i;	
	while((USART2->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){		
		USART2->DR = data[i+1];
		while((USART2->SR&0X40)==0);	
	}
}
  
/*********************************************END OF FILE**********************/

