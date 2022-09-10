/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "usart.h"
#include "./usart2/usart2.h"  
#include "./usart3/usart3.h"
#include "./uart4/uart4.h"
#include "./mqtt/mqtt.h"
#include "./timer/timer4.h"
#include "./timer/timer6.h"
#include "./gps/bsp_atgm336h.h"
#include "./adxl345/bsp_adxl345.h"
#include "./hcsr04/bsp_hcsr04.h"
#include "./key/bsp_key.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
	
	uint32_t r_sp ;
	r_sp = __get_PSP(); //获取SP的值
	printf("error:%d\r\n",r_sp);
	
	while (1)
	{
	}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
 
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

 /**
  * @brief  串口2接收中断函数
  * @param  无
  * @retval 无
  */
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus( USART2, USART_IT_RXNE ) != RESET)
	{		
		switch(USART_ReceiveData( USART2 ))
		{
			case 'z':P_cmd = 1;break;
			case 'L':P_cmd = 2;break;
		}
	}	
} 

 /**
  * @brief  串口3接收中断函数
  * @param  无
  * @retval 无
  */
void USART3_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){  //如果USART_IT_RXNE标志置位，表示有数据到了，进入if分支
		if(Connect_flag==0){                                //如果Connect_flag等于0，当前还没有连接服务器，处于指令配置状态
			if(USART3->DR){                                 //处于指令配置状态时，非零值才保存到缓冲区	
				Usart3_RxBuff[Usart3_RxCounter]=USART3->DR; //保存到缓冲区	
				Usart3_RxCounter ++;                        //每接收1个字节的数据，Usart3_RxCounter加1，表示接收的数据总量+1 
			}		
		}else{		                                        //反之Connect_flag等于1，连接上服务器了	
			Usart3_RxBuff[Usart3_RxCounter] = USART3->DR;   //把接收到的数据保存到Usart3_RxBuff中				
			if(Usart3_RxCounter == 0){    					//如果Usart3_RxCounter等于0，表示是接收的第1个数据，进入if分支				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else分支，表示果Usart3_RxCounter不等于0，不是接收的第一个数据
				TIM_SetCounter(TIM4,0);  
			}	
			Usart3_RxCounter ++;         				    //每接收1个字节的数据，Usart3_RxCounter加1，表示接收的数据总量+1 
		}
	}
}

 /**
  * @brief  串口4接收中断函数
  * @param  无
  * @retval 无
  */
void UART4_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(UART4);//读取接收到的数据
	
		if(Res == '$')
		{
			point1 = 0;	
		}
		
		UART4_RX_BUF[point1++] = Res;
	
		if(UART4_RX_BUF[0] == '$' && UART4_RX_BUF[4] == 'M' && UART4_RX_BUF[5] == 'C')			//确定是否收到"GPRMC/GNRMC"这一帧数据
		{
			if(Res == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
				memcpy(Save_Data.GPS_Buffer, UART4_RX_BUF, point1); 	//保存数据
				Save_Data.isGetData = true;
				point1 = 0;
				memset(UART4_RX_BUF, 0, UART4_REC_LEN);      //清空				
			}	
					
		}
		
		if(point1 >= UART4_REC_LEN)
		{
			point1 = UART4_REC_LEN;
		}	
		  		 
	} 
}

 /**
  * @brief  定时器4中断服务函数 
  * @param  无
  * @retval 无
  */
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart3_RxBuff,Usart3_RxCounter);  //拷贝数据到接收缓冲区
		MQTT_RxDataInPtr[0] = Usart3_RxCounter/256;                   //记录数据长度高字节
		MQTT_RxDataInPtr[1] = Usart3_RxCounter%256;                   //记录数据长度低字节
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                  //指针下移
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //如果指针到缓冲区尾部了
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //指针归位到缓冲区开头
		Usart3_RxCounter = 0;                                         //串口2接收数据量变量清零
		TIM_Cmd(TIM4, DISABLE);                        				  //关闭TIM4定时器
		TIM_SetCounter(TIM4, 0);                        			  //清零定时器4计数器
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}

 /**
  * @brief  定时器6中断服务函数 
  * @param  无
  * @retval 无
  */
void TIM6_DAC_IRQHandler(void)
{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){   //如果TIM_IT_Update置位，表示TIM6溢出中断，进入if	

//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);      //清除TIM6溢出中断标志 	
//	}
//	

	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){                //如果TIM_IT_Update置位，表示TIM4溢出中断，进入if	
		printf("\r\nTim6中断起始-----------------\r\n");
		parseGpsBuffer();//解析gps数据
		pushGPSdata();//发送数据处理换算
		ReadData();//后期优化，定时数据处理
		if(SubcribePack_flag==1)//如果订阅成功
		{
			if(Save_Data.isParseData)//如果解析成功
			{
				
				printf(gps_data);
				MQTT_PublishQs0(P_TOPIC_NAME,gps_data,strlen(gps_data)); //发送消息报文
			}
			else
			{
				MQTT_PingREQ();
			}
		}
		
		//	//路面检测
		if(distance>=20&&distance<=400)
		{
			switch(P_cmd)
			{
				case 1:u2_printf("11111");P_cmd = 0;break;
				case 2:u2_printf("22222");P_cmd = 0;break;
			}
		}
	
	
		//距离检测
		if(distance>0)
		{
			if(distance>=400&&distance<=1000)
			{
				u2_printf("33333");
			}
		}
		
		//摔倒检测
		if(help_flag>0)
		{
			help_time++;
		}
		if(help_flag==1)
		{
			if(KEY0)//如果没有按键按下
			{
							
			}			
			else
			{
				printf("按键");
				help_flag = 0;
				help_time = 0;
			}
		}
		
		printf("刷新状态：%lf\r\n",temp_X);
		if(temp_X!=255586.5)
		{
			if(temp_X<550||temp_X>10000	)
			{
				
				help_flag = 1;
				u2_printf("4444444444");
				printf("摔倒！");		
			}
		}
		else
		{
			printf("加速度模块未连接\r\n");
		}
		printf("Tim6中断结束-----------------\r\n");
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);     			  //清除TIM4溢出中断标志 	
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
