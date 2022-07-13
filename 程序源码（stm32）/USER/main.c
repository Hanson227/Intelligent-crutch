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
#include "./usart3/usart3.h"
#include "./uart4/uart4.h"
#include "./mqtt/mqtt.h"
#include "./timer/timer4.h"
#include "./timer/timer6.h"
#include "./sim800a/bsp_sim800a.h"
#include "./hcsr04/bsp_hcsr04.h"
#include "./gps/bsp_atgm336h.h"

#if 0
 /**
  * @brief  主函数
  * @param  无
  * @retval int
  */
int main(void)
{
	uart_init(115200);//USART1功能初始化，波特率115200				
	Usart3_Init(115200);//USART3功能初始化，波特率115200
	TIM4_Init(300,7200);//TIM4初始化，定时时间300*7200*1000/72000000 = 30ms	
	delay_init(84);//延时函数初始化，84M
	IoT_Parameter_Init();//初始化云IoT平台MQTT服务器的参数
	
	while(1)
	{
		if(Connect_flag==1)
		{
			//if成立的话，说明发送缓冲区有数据了
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
						case 0x01 : printf("订阅成功\r\n");            //串口输出信息
							        SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
									Ping_flag = 0;                        //Ping_flag清零
   								    TIM6_ENABLE_30S();                    //启动30s的PING定时器
									//void MQTT_PublishQs0(char *topic, char *data, int data_len)
									MQTT_PublishQs0(P_TOPIC_NAME,"1234",4); //发布数据给服务器  
									break;                                //跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");  //串口输出信息 
									Connect_flag = 0;                     //Connect_flag置零，重启连接
									break;                                //跳出分支 								
					}					
				}
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING报文回复\r\n"); 		  //串口输出信息 
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						TIM6_ENABLE_30S(); 				  //PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
				//if判断，如果一共接收了10个字节，第一个字节是0x0D，有可能收到了 CLOSED 表示连接断开
				//我们进入else if，接着判断
				else if((MQTT_RxDataOutPtr[1]==10)&&(MQTT_RxDataOutPtr[2]==0x0D)){
					MQTT_RxDataOutPtr[12] = '\0';                   //加入字符串结束符
					if(strstr(SIM800C_RX_BUF,"CLOSED")){            //如果搜索到了CLOSED，表示连接断开了						
						Connect_flag = 0;                           //连接状态置0，表示断开，需要重连服务器
					}			
				}
				//if判断，如果一共接收了15个字节，第一个字节是0x0D，有可能收到了 +PDP: DEACT 表示GPRS断开
				//我们进入else if，接着判断
				else if((MQTT_RxDataOutPtr[1]==15)&&(MQTT_RxDataOutPtr[1]==0x0D)){
					MQTT_RxDataOutPtr[17] = '\0';                   //加入字符串结束符
					printf("GPRS断开\r\n"); 		                //串口输出信息
					if(strstr(SIM800C_RX_BUF,"+PDP: DEACT")){       //如果搜索到+PDP: DEACT，表示GPRS断开了						
						Connect_flag = 0;                           //连接状态置0，表示断开，需要重连服务器
					}			
				}	
				
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/
//			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if成立的话，说明命令缓冲区有数据了			       
//				printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
//				if(!memcmp(&MQTT_CMDOutPtr[2],CMD1,strlen(CMD1))){           //判断指令，如果是CMD1
//					LED1_OUT(!LED1_IN_STA);                                  //开关1状态翻转 如果点亮就熄灭，反之如果熄灭就点亮
//					LED1_State();                                            //判断开关和倒计时状态，并发布给服务器
//				}else if(!memcmp(&MQTT_CMDOutPtr[2],CMD2,strlen(CMD2))){     //判断指令，如果是CMD2，回复开关状态
//					LED1_State();                                            //判断开关和倒计时状态，并发布给服务器				
//				}else printf("未知指令\r\n");                             //串口输出信息
//				
//				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //指针下移
//				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
//					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
//			}//处理命令缓冲区数据的else if分支结尾		
		}//Connect_flag=1的if分支的结尾
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("需要连接服务器\r\n");//串口输出信息
			TIM_Cmd(TIM4,DISABLE);//关闭TIM4 
			TIM_Cmd(TIM6,DISABLE);//关闭TIM6  
			SIM800C_RxCounter=0;//800C接收数据量变量清零
			//清空800C接收缓冲区                        
			memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);     
			
			/*如果800C连接云服务器函数返回0，表示正确，进入if*/
			if(SIM800C_Connect_IoTServer()==0)
			{   			 
				printf("建立TCP连接成功\r\n");//串口输出信息
				Connect_flag = 1;//置1，表示连接成功	
				SIM800C_RxCounter=0;//800C接收数据量变量清零    
				//清空800C接收缓冲区                    
				memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE); 
				MQTT_Buff_Init();//初始化缓冲区                    
			}				
		}
	}
}

#endif 


#if 0
 /**
  * @brief  超声波测试函数
  * @param  无
  * @retval int
  */
int main(void)
{	
	uint32_t a=0;
	//串口1波特率:115200bps
	delay_init(84);//延时函数初始化，84M
	uart_init(115200);	
	sr04_init();//初始化超声波模块
	printf("超声波模块测试");
	while(1)
	{
		
		a=sr04_get_distance();
		if(a>0)
		{
			if(a>=20&&a<=4000)
			{
				
			}
		}
		printf("%d cm\r\n",a/10);
		delay_ms(1000);
	}
}

#endif

#if 1

int main(void)
{	
	delay_init(84);
	uart_init(9600);	 //串口初始化
	UART4_Init(9600);
		
	clrStruct();
	
	printf("GPS模块测试");
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();   
	}
}

#endif

/*********************************************END OF FILE**********************/

