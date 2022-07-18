/**
  ******************************************************************************
  * @file    bsp_usart3.c
  * @author  fire
  * @version V1.0
  * @date    2022-07-10
  * @brief  ʹ�ô���3��sim800�������ݣ��жϽ���ģʽ
  ******************************************************************************
  */ 

#include "stm32f4xx.h"
#include "./usart3/usart3.h"
#include "./timer/timer4.h"


/*ȫ�ֱ�������*/
char Usart3_RxCompleted = 0;            //����һ������ 0����ʾ����δ��� 1����ʾ������� 
unsigned int Usart3_RxCounter = 0;      //����һ����������¼����3�ܹ������˶����ֽڵ�����
char Usart3_RxBuff[USART3_RXBUFF_SIZE]; //����һ�����飬���ڱ��洮��3���յ�������   	



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
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
}


 /**
  * @brief  RS232_USART GPIO ����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void Usart3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
			
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* ʹ�� UART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_USART3);
	
	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_USART3);
	
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	/* ���ô��� USART3 ģʽ */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure); 
	
	NVIC_Configuration();
	/*���ô��ڽ����ж�*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART3, ENABLE);
}

 /**
  * @brief  �����������
  * @param  fmt�������ַ���
  * @retval ��
  */
__align(8) char Usart3_TxBuff[USART3_TXBUFF_SIZE];
void u3_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart3_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart3_TxBuff);		
	while((USART3->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART3->DR = Usart3_TxBuff[i];
		while((USART3->SR&0X40)==0);	
	}	
}

 /**
  * @brief  ����3���ͻ������е�����
  * @param  data������
  * @retval ��
  */
void u3_TxData(unsigned char *data)
{
	int	i;	
	while((USART3->SR&0X40)==0);
	for(i = 1;i <= (data[0]*256+data[1]);i ++){		
		USART3->DR = data[i+1];
		while((USART3->SR&0X40)==0);	
	}
}



/*********************************************END OF FILE**********************/


