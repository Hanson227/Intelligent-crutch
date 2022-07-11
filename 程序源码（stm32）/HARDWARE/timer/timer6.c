/**
  ******************************************************************************
  * @file    timer6.c
  * @author  chen
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   实现定时器6功能
  ******************************************************************************
  */

#include "stm32f4xx.h"  

 /**
  * @brief  定时器6使能30s定时
  * @param  无
  * @retval 无
  */
void TIM6_ENABLE_30S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);           //使能TIM6时钟	
	TIM_DeInit(TIM6);                                             //定时器6寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 60000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=36000-1;              //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);            //设置TIM6
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                      //使能TIM6溢出中断    
	TIM_Cmd(TIM6,ENABLE);                                         //开TIM6                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;                 //设置TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;       //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

 /**
  * @brief  定时器6使能2s定时
  * @param  无
  * @retval 无
  */
void TIM6_ENABLE_2S(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;            //定义一个设置定时器的变量
	NVIC_InitTypeDef NVIC_InitStructure;                          //定义一个设置中断的变量
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               //设置中断向量分组：第2组 抢先优先级：0 1 2 3 子优先级：0 1 2 3		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);           //使能TIM6时钟
	TIM_DeInit(TIM6);                                             //定时器6寄存器恢复默认值	
	TIM_TimeBaseInitStructure.TIM_Period = 20000-1; 	          //设置自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=7200-1;               //设置定时器预分频数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;     //1分频
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);            //设置TIM6
	
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);                    //清除溢出中断标志位
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);                      //使能TIM6溢出中断    
	TIM_Cmd(TIM6,ENABLE);                                         //开TIM6                          
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;                 //设置TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;       //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;              //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;                 //中断通道使能
	NVIC_Init(&NVIC_InitStructure);                               //设置中断
}

/*********************************************END OF FILE**********************/
