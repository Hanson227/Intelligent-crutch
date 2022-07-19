/**
 * @file bsp_buzzer.c
 * @author Hanson 
 * @brief 扬声器随着超声波模块距离增加而提高响度
 * @version 0.1
 * @date 2022-07-19
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "./hcsr04/bsp_hcsr04.h"
#include "usart.h"
#include "delay.h"


uint32_t tim13_cnt=0;


/**
 * @brief 定时器13初始化
 * 
 */
void tim13_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/* 定时器13的时钟使能*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	

	/*定时器的基本配置，用于配置定时器的输出脉冲的频率为100Hz */
	TIM_TimeBaseStructure.TIM_Period = (40000/100)-1;					//设置定时脉冲的频率
	TIM_TimeBaseStructure.TIM_Prescaler = 2100-1;						//第一次分频，简称为预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//第二次分频,当前实现1分频，也就是不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	tim13_cnt=TIM_TimeBaseStructure.TIM_Period;
	
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

	/* 配置PF8 引脚为复用模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					//第8根引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//设置复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽模式，增加驱动电流
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//设置IO的速度为100MHz，频率越高性能越好，频率越低，功耗越低
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//不需要上拉电阻
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //允许输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//有效的时候， 输出高电平
	TIM_OC1Init(TIM13, &TIM_OCInitStructure);
	
	/*  使能定时器 13 工作 */
	TIM_Cmd(TIM13, ENABLE);
}

/**
 * @brief 设置定时器13的频率
 * 
 * @param freq 频率大小
 */
void tim13_set_freq(uint32_t freq)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	/*定时器的基本配置，用于配置定时器的输出脉冲的频率为freq Hz */
	TIM_TimeBaseStructure.TIM_Period = (40000/freq)-1;					//设置定时脉冲的频率
	TIM_TimeBaseStructure.TIM_Prescaler = 2100-1;						//第一次分频，简称为预分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				//第二次分频,当前实现1分频，也就是不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	tim13_cnt= TIM_TimeBaseStructure.TIM_Period;
	
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);

}

/**
 * @brief 设置定时器13的PWM占空比0%~100%
 * 
 * @param duty 占空比大小0%~100%
 */
void tim13_set_duty(uint32_t duty)
{
	uint32_t cmp=0;
	
	cmp = (tim13_cnt+1) * duty/100;

	TIM_SetCompare1(TIM13,cmp);
}

/**
 * @brief 蜂鸣器初始化
 * 
 */
void beep_init(void)
{
	tim13_init();
	
	//蜂鸣器禁鸣
	tim13_set_duty(0);
}

/**
 * @brief 根据距离进行蜂鸣器响应
 * 
 */
void buzzer(void)
{
    int distance=0;
	uint32_t duty=50;
	uint32_t distance_safe=1;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置中断分组

    sr04_init();

    beep_init();

    tim13_set_duty(duty);//设置音量范围1-100
    while (1)
    {
        distance=sr04_get_distance();//注意：距离单位为毫米
        if(distance < 0)
		{
			//模块有可能没有连接好，连接错引脚或杜邦线松动
			printf("sr04 is error\r\n");
		}
		else if(distance>=20 && distance<=400)
		{
			//数据是正确的
			printf("distance=%dmm\r\n",distance);		
		
			if(distance>=250 && distance<350)
			{	
				tim13_set_freq(2);
                distance_safe=1;
			}				
			else if(distance>=150 && distance<250)
			{
				tim13_set_freq(4);
                distance_safe=1;
			}	

			else if(distance<150)
			{			
				tim13_set_freq(10);
                distance_safe=1;
			}	

			//若是安全距离，则蜂鸣器禁鸣
			if(distance_safe==1)
            {
                tim13_set_duty(0);
            }
			//若不是安全距离，则蜂鸣器根据对应的占空比进行鸣响
			else
            {
                tim13_set_duty(duty);
            }	
		}
		else
		{
			//数据是异常的
			printf("sr04 get distance is error\r\n");
		}
		//延时500ms
		delay_ms(50);
    }    
}
