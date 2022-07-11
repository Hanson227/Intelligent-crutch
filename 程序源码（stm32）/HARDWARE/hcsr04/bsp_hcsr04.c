/**
  ******************************************************************************
  * @file    hcsr04.c
  * @author  chen
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   超声波测距
  ******************************************************************************
  */

#include "./hcsr04/bsp_hcsr04.h"
#include "delay.h"

 /**
  * @brief  超声波模块引脚初始化
  * @param  无
  * @retval 无
  */
void sr04_init(void)//配置PE2为触发信号引脚，PE3为回想信号引脚
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	//配置引脚为复用功能模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//第3根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;	//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//第3根引脚
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;	//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	//推挽输出，增加输出电流能力。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//高速响应
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//没有使能上下拉电阻
	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	PEout(2)=0;//配置PE2引脚初始化为低电平状态
}

 /**
  * @brief  超声波测距函数
  * @param  无
  * @retval 所测距离，单位cm
  */
uint32_t sr04_get_distance(void)
{
	uint32_t t=0;
	uint32_t d=0;
	//PE2持续最少10us的高电平时间
	PEout(2)=1;
	delay_us(20);
	PEout(2)=0;
	
	//回响信号判断高电平
	
	while(PEin(3)==0)
	{
			//超时处理
		t++;
		delay_us(1);
		if(t>=1000000)
		{
			return 1;//超时就退出循环并返回错误码
		}
	}
	
	//记录高电平的时间
	
	while(PEin(3)==1)
	{
		d++;
		delay_us(9);//9微秒为3mm的距离，为精度范围因此为一个循环
		while(d>=1000000)
		{
			return 2;//如果超时就返回-2
		}
		
	}
	
	d=d/2;
	return (d*3);
}
/*********************************************END OF FILE**********************/
