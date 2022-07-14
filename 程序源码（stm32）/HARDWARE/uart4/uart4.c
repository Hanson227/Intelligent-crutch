/**
  ******************************************************************************
  * @file    uart4.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-14
  * @brief   ʹ�ô���4��gpsģ�����ݽ���
  ******************************************************************************
  */

#include "./uart4/uart4.h"

/*ȫ�ֱ���*/
char rxdatabufer;
u16 point1 = 0;
_SaveData Save_Data;

//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
char UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���UART4_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART4_RX_STA=0;       //����״̬���	



/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* ����UARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* �������ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* �����ȼ�Ϊ1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  ����4����,����ģʽ���á�115200 8-N-1 ���жϽ���ģʽ
  * @param  ��
  * @retval ��
  */
void UART4_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	/* ʹ�� USART ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* GPIO��ʼ�� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  
	/* ���� PXx �� USARTx_Tx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	/*  ���� PXx �� USARTx__Rx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	
	/* ���ô���ģʽ */
	/* ���������ã�bound */
	USART_InitStructure.USART_BaudRate = bound;
	/* �ֳ�(����λ+У��λ)��8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* ֹͣλ��1��ֹͣλ */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* У��λѡ�񣺲�ʹ��У�� */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* Ӳ�������ƣ���ʹ��Ӳ���� */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* ���USART��ʼ������ */
	USART_Init(UART4, &USART_InitStructure); 
	
	/* Ƕ�������жϿ�����NVIC���� */
	NVIC_Configuration();
  
	/* ʹ�ܴ��ڽ����ж� */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	/* ʹ�ܴ��� */
	USART_Cmd(UART4, ENABLE);
  
	CLR_Buf();//��ջ���
}

 /**
  * @brief  ��������ʶ����
  * @param  a��ʶ�������
  * @retval 0��������
			1��������
  */
u8 Hand(char *a)                   // 
{ 
    if(strstr(UART4_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

 /**
  * @brief  ���ڻ�������
  * @param  ��
  * @retval ��
  */
void CLR_Buf(void)
{
	memset(UART4_RX_BUF, 0, UART4_REC_LEN);      //���
	point1 = 0;                    
}

 /**
  * @brief  ���GPS�ṹ������
  * @param  ��
  * @retval ��
  */
void clrStruct(void)
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}







/*********************************************END OF FILE**********************/

