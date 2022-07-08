/**
  ******************************************************************************
  * @file    main.c
  * @author  Baochi
  * @version V1.0
  * @date    2022
  * @brief   ���ܹ���������
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./usart/bsp_usart3.h"
#include "./systick/bsp_SysTick.h"



/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	/*��ģ���ʼ��*/
	Debug_USART_Config();	//��ʼ��USART1�����Դ��� ����ģʽΪ 115200 8-N-1���жϽ���
	USART3_Config();		//��ʼ��USART3��SIM800����Э�鴫�䣬����ģʽΪ 115200 8-N-1���жϽ���
	SysTick_Init();			//ϵͳ��ʱ����ʼ��������ʵ����ʱ����
	
	
	printf("����һ�������жϽ��ջ���ʵ��\n");
	
	while(1)
	{	
		
	}	
}



/*********************************************END OF FILE**********************/

