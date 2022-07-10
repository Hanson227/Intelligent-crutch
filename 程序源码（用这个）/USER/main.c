#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "./sim800/bsp_sim800.h"
#include "./mqtt/mqtt.h"
#include "./usart3/usart3.h"
#include "./tim/timer6.h"
#include "./tim/timer4.h"




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
		}
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




