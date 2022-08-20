/**
  ******************************************************************************
  * @file    main.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-08
  * @brief   智能拐杖主函数文件
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "./usart2/usart2.h" 
#include "./usart3/usart3.h"
#include "./uart4/uart4.h"
#include "./mqtt/mqtt.h"
#include "./timer/timer4.h"
#include "./timer/timer6.h"
#include "./key/bsp_key.h"
#include "./sim800a/bsp_sim800a.h"
#include "./hcsr04/bsp_hcsr04.h"
#include "./gps/bsp_atgm336h.h"
#include "./adxl345/bsp_adxl345.h"

#if 1

 /**
  * @brief  主函数
  * @param  无
  * @retval int
  */
int main(void)
{
	debug_uart_init(115200);//USART1功能初始化，波特率115200
	Usart2_Init(9600);	
	Usart3_Init(115200);//USART3功能初始化，波特率115200
	Uart4_Init(9600);
	TIM4_Init(300,7200);//TIM4初始化，定时时间300*7200*1000/72000000 = 30ms
	TIM6_ENABLE_2S();//TIM6使能2s中断
	delay_init(84);//延时函数初始化，84M
	sr04_init();//初始化超声波模块
	IoT_Parameter_Init();//初始化云IoT平台MQTT服务器的参数
	Init_ADXL345();//加速度传感器初始化
	KEY_Init();
	
	while(1)
	{		


		
		/*-------------------------------------------------------------*/
		/*          跌倒求助功能，向紧急联系人发送求助和位置坐标         */
		/*-------------------------------------------------------------*/
		//printf("h:%d",help_time);

		if(help_time>5)
		{

			printf("报警！");
			u2_printf("5555555555");
			if(Save_Data.isParseData)
			{
				SIM800A_Note_Edit(EMERGENCY_CALL_CMD);
				SIM800A_printf("help me!\r\n%s",gps_data);
				SIM800A_Note_Send(30);
			}
			else
			{
				SIM800A_Note_Edit(EMERGENCY_CALL_CMD);
				SIM800A_printf("help me!");
				SIM800A_Note_Send(30);
			}
			help_flag = 0;
			help_time = 0;
		}

		

		
		/*-------------------------------------------------------------*/
		/*   串口发送数据，语音模块接收识别，提示前方障碍物，无需联网     */
		/*-------------------------------------------------------------*/
		//sr04_get_distance();

		
		if(Connect_flag==1)//联网成功
		{		
			
			
			
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
			{                
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||
					((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
					//串口提示信息
					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);
					MQTT_TxData(MQTT_TxDataOutPtr);//发送数据
					MQTT_TxDataOutPtr += TBUFF_UNIT;//指针下移
					//如果指针到缓冲区尾部了
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)
						//指针归位到缓冲区开头
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];
				}
				else
					printf("数据：0x%x\r\n",MQTT_TxDataOutPtr[2]);
			}//处理发送缓冲区数据的else if分支结尾


			
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if成立的话，说明接收缓冲区有数据了														
				printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}				
				}			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");//串口输出信息
							        SubcribePack_flag = 1;//订阅报文成功，其他报文可发送									  
									break;//跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");//串口输出信息 
									Connect_flag = 0;//Connect_flag置零，重启连接
									break;//跳出分支 								
					}					
				}
								
				//if判断，如果一共接收了10个字节，第一个字节是0x0D，有可能收到了 CLOSED 表示连接断开
				//我们进入else if，接着判断
				else if((MQTT_RxDataOutPtr[1]==10)&&(MQTT_RxDataOutPtr[2]==0x0D)){
					MQTT_RxDataOutPtr[12] = '\0';//加入字符串结束符
					if(strstr(SIM800A_RX_BUF,"CLOSED")){//如果搜索到了CLOSED，表示连接断开了						
						Connect_flag = 0;//连接状态置0，表示断开，需要重连服务器
					}			
				}
				//if判断，如果一共接收了15个字节，第一个字节是0x0D，有可能收到了 +PDP: DEACT 表示GPRS断开
				//我们进入else if，接着判断
				else if((MQTT_RxDataOutPtr[1]==15)&&(MQTT_RxDataOutPtr[1]==0x0D)){
					MQTT_RxDataOutPtr[17] = '\0';                   //加入字符串结束符
					printf("GPRS断开\r\n"); 		                //串口输出信息
					if(strstr(SIM800A_RX_BUF,"+PDP: DEACT")){       //如果搜索到+PDP: DEACT，表示GPRS断开了						
						Connect_flag = 0;                           //连接状态置0，表示断开，需要重连服务器
					}			
				}	
				
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾	
		}//Connect_flag=1的if分支的结尾
		
		
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("需要连接服务器\r\n");//串口输出信息
			
			 
			SIM800A_RxCounter=0;//800C接收数据量变量清零
			//清空800C接收缓冲区                        
			memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);     
			
			/*如果800C连接云服务器函数返回0，表示正确，进入if*/
			if(SIM800A_Connect_IoTServer()==0)
			{   			 
				printf("建立TCP连接成功\r\n");//串口输出信息
				u2_printf("6666666666");////串口发送数据，语音模块接收识别，提示网络连接成功
				Connect_flag = 1;//置1，表示连接成功	
				SIM800A_RxCounter=0;//800C接收数据量变量清零    
				//清空800C接收缓冲区                    
				memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE); 
				MQTT_Buff_Init();//初始化缓冲区                    
			}				
		}
	}
}

#endif 


#if 0

 /**
  * @brief  超声波模块调试函数
  * @param  无
  * @retval int
  */
int main(void)
{	
	delay_init(84);//延时函数初始化，84M
	debug_uart_init(115200);//获取前方障碍物距离	
	Usart2_Init(9600);//USART2初始化，用于向语音识别模块的STC单片机发送数据
	sr04_init();//初始化超声波模块
	printf("超声波模块测试\r\n");
	while(1)
	{	
		sr04_get_distance();//获取前方障碍物距离
		if(distance>0)
		{
			if(distance>=20&&distance<=1000)//如果20mm-1000mm范围内有物体
			{
				u2_printf("3");//串口发送命令3，语音模块接收识别，提示前方有障碍物
			}
		}
		printf("%d cm\r\n",distance/10);//调试串口数据输出
		delay_ms(1000);//延时一秒
	}
}

#endif

#if 0

 /**
  * @brief  GPS定位模块调试函数
  * @param  无
  * @retval int
  */
int main(void)
{	
	delay_init(84);
	debug_uart_init(115200);	 //串口初始化
	Uart4_Init(9600);
		
	clrStruct();
	
	printf("GPS模块测试\r\n");
	while(1)
	{
		parseGpsBuffer();  
		//GPS_data_Buff();
		//printf(gps_data);
		//printf("\r\n");
	}
}

#endif

#if 0

 /**
  * @brief  语音模块调试函数
  * @param  无
  * @retval int
  */
int main(void)
{
	delay_init(84);
	debug_uart_init(115200);	 //串口初始化
	Usart2_Init(9600);
	u2_printf("语音模块测试\r\n");
	while(1)
	{
	}	
}

#endif



#if 0

 /**
  * @brief  ADXL345模块调试函数
  * @param  无
  * @retval int
  */
int main()
{
	delay_init(84);
	debug_uart_init(115200);
	Usart3_Init(115200);//USART3功能初始化，波特率115200
	Init_ADXL345();
	printf("ADXL345模块测试\r\n");
	if(Single_Read_ADXL345(0X00)==0xe5)	
	{
		delay_ms(5);
	}
	else
	{
		delay_ms(3);
	}
	while(1)
	{
		
		ReadData();
		delay_ms(1000);
		
		printf("x=%lf,y=%lf,z=%lf\r\n",temp_X,temp_Y,temp_Z);
	}
}

#endif

#if 0

int main()
{
	int key;
	debug_uart_init(115200);
	KEY_Init();
	printf("按键测试\r\n");
	delay_init(84);//延时函数初始化，84M
	while(1)
	{
		key = KEY_Scan(0);
		if(!KEY0)
		printf("%d",key);
	}
}

#endif

/*********************************************END OF FILE**********************/

