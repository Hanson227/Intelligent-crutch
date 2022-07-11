/**
  ******************************************************************************
  * @file    timer6.c
  * @author  chen
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   ʵ�ֶ�ʱ��6����
  ******************************************************************************
  */

#include "stm32f4xx.h"  

 /**
  * @brief  ��ʱ��6ʹ��30s��ʱ
  * @param  ��
  * @retval ��
  */
void TIM6_ENABLE_30S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);           //ʹ��TIM6ʱ��	
	TIM_DeInit(TIM6);                                             //��ʱ��6�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);            //����TIM6
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                      //ʹ��TIM6����ж�    
	TIM_Cmd(TIM6,ENABLE);                                         //��TIM6                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;                 //����TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}

 /**
  * @brief  ��ʱ��6ʹ��2s��ʱ
  * @param  ��
  * @retval ��
  */
void TIM6_ENABLE_2S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //����һ�����ö�ʱ���ı���
	NVIC_InitTypeDef NVIC_InitStructure;                          //����һ�������жϵı���
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //�����ж��������飺��2�� �������ȼ���0 1 2 3 �����ȼ���0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);           //ʹ��TIM6ʱ��
	TIM_DeInit(TIM6);                                             //��ʱ��6�Ĵ����ָ�Ĭ��ֵ	
	TIM_TimeBaseInitStructure.TIM_Period = 20000-1; 	          //�����Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;               //���ö�ʱ��Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1��Ƶ
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);            //����TIM6
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                    //�������жϱ�־λ
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                      //ʹ��TIM6����ж�    
	TIM_Cmd(TIM6,ENABLE);                                         //��TIM6                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;                 //����TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //�ж�ͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                               //�����ж�
}

/*********************************************END OF FILE**********************/
