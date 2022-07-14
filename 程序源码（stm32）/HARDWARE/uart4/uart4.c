/**
  ******************************************************************************
  * @file    uart4.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-14
  * @brief   使用串口4，gps模块数据接收
  ******************************************************************************
  */

#include "./uart4/uart4.h"

/*全局变量*/
char rxdatabufer;
u16 point1 = 0;
_SaveData Save_Data;

//注意,读取USARTx->SR能避免莫名其妙的错误   	
char UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大UART4_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA=0;       //接收状态标记	



/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 嵌套向量中断控制器组选择 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* 配置UART为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	/* 抢断优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	/* 子优先级为1 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* 初始化配置NVIC */
	NVIC_Init(&NVIC_InitStructure);
}


 /**
  * @brief  串口4配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
  */
void UART4_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	/* 使能 USART 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	/* GPIO初始化 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* 配置Tx引脚为复用功能  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10  ;  
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* 配置Rx引脚为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
  
	/* 连接 PXx 到 USARTx_Tx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	/*  连接 PXx 到 USARTx__Rx*/
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	
	/* 配置串口模式 */
	/* 波特率设置：bound */
	USART_InitStructure.USART_BaudRate = bound;
	/* 字长(数据位+校验位)：8 */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* 停止位：1个停止位 */
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* 校验位选择：不使用校验 */
	USART_InitStructure.USART_Parity = USART_Parity_No;
	/* 硬件流控制：不使用硬件流 */
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART模式控制：同时使能接收和发送 */
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 完成USART初始化配置 */
	USART_Init(UART4, &USART_InitStructure); 
	
	/* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
  
	/* 使能串口接收中断 */
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	
	/* 使能串口 */
	USART_Cmd(UART4, ENABLE);
  
	CLR_Buf();//清空缓存
}

 /**
  * @brief  串口命令识别函数
  * @param  a：识别的命令
  * @retval 0：无命令
			1：有命令
  */
u8 Hand(char *a)                   // 
{ 
    if(strstr(UART4_RX_BUF,a)!=NULL)
	    return 1;
	else
		return 0;
}

 /**
  * @brief  串口缓存清理
  * @param  无
  * @retval 无
  */
void CLR_Buf(void)
{
	memset(UART4_RX_BUF, 0, UART4_REC_LEN);      //清空
	point1 = 0;                    
}

 /**
  * @brief  清除GPS结构体数据
  * @param  无
  * @retval 无
  */
void clrStruct(void)
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	memset(Save_Data.E_W, 0, E_W_Length);
	
}







/*********************************************END OF FILE**********************/

