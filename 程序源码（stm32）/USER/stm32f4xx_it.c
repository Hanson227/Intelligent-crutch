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
	r_sp = __get_PSP(); //��ȡSP��ֵ
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
  * @brief  ����2�����жϺ���
  * @param  ��
  * @retval ��
  */
void USART2_IRQHandler(void)                	//����1�жϷ������
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
  * @brief  ����3�����жϺ���
  * @param  ��
  * @retval ��
  */
void USART3_IRQHandler(void)   
{                      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){  //���USART_IT_RXNE��־��λ����ʾ�����ݵ��ˣ�����if��֧
		if(Connect_flag==0){                                //���Connect_flag����0����ǰ��û�����ӷ�����������ָ������״̬
			if(USART3->DR){                                 //����ָ������״̬ʱ������ֵ�ű��浽������	
				Usart3_RxBuff[Usart3_RxCounter]=USART3->DR; //���浽������	
				Usart3_RxCounter ++;                        //ÿ����1���ֽڵ����ݣ�Usart3_RxCounter��1����ʾ���յ���������+1 
			}		
		}else{		                                        //��֮Connect_flag����1�������Ϸ�������	
			Usart3_RxBuff[Usart3_RxCounter] = USART3->DR;   //�ѽ��յ������ݱ��浽Usart3_RxBuff��				
			if(Usart3_RxCounter == 0){    					//���Usart3_RxCounter����0����ʾ�ǽ��յĵ�1�����ݣ�����if��֧				
				TIM_Cmd(TIM4,ENABLE); 
			}else{                        					//else��֧����ʾ��Usart3_RxCounter������0�����ǽ��յĵ�һ������
				TIM_SetCounter(TIM4,0);  
			}	
			Usart3_RxCounter ++;         				    //ÿ����1���ֽڵ����ݣ�Usart3_RxCounter��1����ʾ���յ���������+1 
		}
	}
}

 /**
  * @brief  ����4�����жϺ���
  * @param  ��
  * @retval ��
  */
void UART4_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(UART4);//��ȡ���յ�������
	
		if(Res == '$')
		{
			point1 = 0;	
		}
		
		UART4_RX_BUF[point1++] = Res;
	
		if(UART4_RX_BUF[0] == '$' && UART4_RX_BUF[4] == 'M' && UART4_RX_BUF[5] == 'C')			//ȷ���Ƿ��յ�"GPRMC/GNRMC"��һ֡����
		{
			if(Res == '\n')									   
			{
				memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //���
				memcpy(Save_Data.GPS_Buffer, UART4_RX_BUF, point1); 	//��������
				Save_Data.isGetData = true;
				point1 = 0;
				memset(UART4_RX_BUF, 0, UART4_REC_LEN);      //���				
			}	
					
		}
		
		if(point1 >= UART4_REC_LEN)
		{
			point1 = UART4_REC_LEN;
		}	
		  		 
	} 
}

 /**
  * @brief  ��ʱ��4�жϷ����� 
  * @param  ��
  * @retval ��
  */
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){                //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
		
		memcpy(&MQTT_RxDataInPtr[2],Usart3_RxBuff,Usart3_RxCounter);  //�������ݵ����ջ�����
		MQTT_RxDataInPtr[0] = Usart3_RxCounter/256;                   //��¼���ݳ��ȸ��ֽ�
		MQTT_RxDataInPtr[1] = Usart3_RxCounter%256;                   //��¼���ݳ��ȵ��ֽ�
		MQTT_RxDataInPtr+=RBUFF_UNIT;                                  //ָ������
		if(MQTT_RxDataInPtr==MQTT_RxDataEndPtr)                       //���ָ�뵽������β����
			MQTT_RxDataInPtr = MQTT_RxDataBuf[0];                     //ָ���λ����������ͷ
		Usart3_RxCounter = 0;                                         //����2������������������
		TIM_Cmd(TIM4, DISABLE);                        				  //�ر�TIM4��ʱ��
		TIM_SetCounter(TIM4, 0);                        			  //���㶨ʱ��4������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);     			  //���TIM4����жϱ�־ 	
	}
}

 /**
  * @brief  ��ʱ��6�жϷ����� 
  * @param  ��
  * @retval ��
  */
void TIM6_DAC_IRQHandler(void)
{
//	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){   //���TIM_IT_Update��λ����ʾTIM6����жϣ�����if	

//		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);      //���TIM6����жϱ�־ 	
//	}
//	

	if(TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET){                //���TIM_IT_Update��λ����ʾTIM4����жϣ�����if	
		printf("\r\nTim6�ж���ʼ-----------------\r\n");
		parseGpsBuffer();//����gps����
		pushGPSdata();//�������ݴ�����
		ReadData();//�����Ż�����ʱ���ݴ���
		if(SubcribePack_flag==1)//������ĳɹ�
		{
			if(Save_Data.isParseData)//��������ɹ�
			{
				
				printf(gps_data);
				MQTT_PublishQs0(P_TOPIC_NAME,gps_data,strlen(gps_data)); //������Ϣ����
			}
			else
			{
				MQTT_PingREQ();
			}
		}
		
		//	//·����
		if(distance>=20&&distance<=400)
		{
			switch(P_cmd)
			{
				case 1:u2_printf("11111");P_cmd = 0;break;
				case 2:u2_printf("22222");P_cmd = 0;break;
			}
		}
	
	
		//������
		if(distance>0)
		{
			if(distance>=400&&distance<=1000)
			{
				u2_printf("33333");
			}
		}
		
		//ˤ�����
		if(help_flag>0)
		{
			help_time++;
		}
		if(help_flag==1)
		{
			if(KEY0)//���û�а�������
			{
							
			}			
			else
			{
				printf("����");
				help_flag = 0;
				help_time = 0;
			}
		}
		
		printf("ˢ��״̬��%lf\r\n",temp_X);
		if(temp_X!=255586.5)
		{
			if(temp_X<550||temp_X>10000	)
			{
				
				help_flag = 1;
				u2_printf("4444444444");
				printf("ˤ����");		
			}
		}
		else
		{
			printf("���ٶ�ģ��δ����\r\n");
		}
		printf("Tim6�жϽ���-----------------\r\n");
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);     			  //���TIM4����жϱ�־ 	
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
