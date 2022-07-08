/**
  ******************************************************************************
  * @file    main.c
  * @author  Baochi
  * @version V1.0
  * @date    2022
  * @brief   智能拐杖主函数
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./usart/bsp_usart3.h"
#include "./systick/bsp_SysTick.h"



/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
	/*各模块初始化*/
	Debug_USART_Config();	//初始化USART1，调试串口 配置模式为 115200 8-N-1，中断接收
	USART3_Config();		//初始化USART3，SIM800串口协议传输，配置模式为 115200 8-N-1，中断接收
	SysTick_Init();			//系统定时器初始化，用于实现延时函数
	
	
	printf("这是一个串口中断接收回显实验\n");
	
	while(1)
	{	
		
	}	
}



/*********************************************END OF FILE**********************/

