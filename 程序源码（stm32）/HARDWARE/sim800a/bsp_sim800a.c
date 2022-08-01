/**
  ******************************************************************************
  * @file    bsp_sim800a.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-12
  * @brief   led应用函数接口
  ******************************************************************************
  */

#include "stm32f4xx.h"                 
#include "./sim800a/bsp_sim800a.h"	
#include "usart.h"
#include "delay.h"
#include "./mqtt/mqtt.h"

 /**
  * @brief  发送设置指令
  * @param  cmd：指令
			timeout：超时时间（100ms的倍数）
  * @retval 0：正确
			其他：错误
  */
char SIM800A_SendCmd(char *cmd, int timeout)
{
	SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
	SIM800A_printf("%s\r\n",cmd);                  //发送指令
	while(timeout--){                              //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(SIM800A_RX_BUF,"OK"))            //如果接收到OK表示指令成功
			break;       						   //主动跳出while循环
		printf("%d ",timeout);                  //串口输出现在的超时时间
	}
	printf("\r\n");                             //串口输出信息
	if(timeout<=0)return 1;                        //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;		         				   //反之，表示正确，说明收到OK，通过break主动跳出while
}

 /**
  * @brief  
  * @param  
  * @retval 
  */
char SIM800A_CMGS(int timeout,char *call_cmd)
{
	SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
	SIM800A_printf("%s\r\n",call_cmd); 
	while(timeout--){                              //等待超时时间到0
		delay_ms(1000);                             //延时1s
		if(strstr(SIM800A_RX_BUF,">"))            //如果接收到OK表示指令成功
			break;       						   //主动跳出while循环
		printf("%d ",timeout);                  //串口输出现在的超时时间
	}
	printf("\r\n");                             //串口输出信息
	if(timeout<=0)return 1;                        //如果timeout<=0，说明超时时间到了，也没能收到OK，返回1
	else return 0;	
}



 /**
  * @brief  
  * @param  
  * @retval 
  */
char SIM800A_Note_Edit(char *call_cmd)
{
	printf("等待设置字符集... ...\r\n");
	if(SIM800A_SendCmd("AT+CSCS=\"GSM\"",10)){
		printf("设置字符集失败\r\n");
		return 1;
	}else{
		printf("设置字符集成功\r\n");
		printf(SIM800A_RX_BUF);
		printf("等待设置文本模式... ...\r\n");
		
		if(SIM800A_SendCmd("AT+CMGF=1",10)){
			printf("设置文本模式失败\r\n");
			return 2;
		}else{
			printf("设置文本模式成功\r\n");
			printf(SIM800A_RX_BUF);
			printf("等待设置联系人... ...\r\n");
			if(SIM800A_CMGS(10,call_cmd)){
				printf("设置联系人失败\r\n");
				return 2;
			}else{
				printf("设置联系人成功\r\n");
				printf(SIM800A_RX_BUF);
				return 0;
			}
		}
	}	
}

char SIM800A_Note_Send(int timeout)
{
	SIM800A_RxCounter=0;//800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);//清空800C接收缓冲区 
	USART_SendData(USART3, 0x1a);
	
	while(timeout--)
	{
		delay_ms(1000);//延时1s
		//printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CMGS:"))//如果接收到 +CREG:
			break;//主动跳出while循环
		printf("%d ",timeout);  
	}
	printf("\r\n");                                       //串口输出信息
	if(timeout<=0)return 1;                                  //如果timeout<=0，说明超时时间到了，也没能收到+CSQ:，返回1
	else{
		printf("发送短信成功");
		return 0;
	}
}


 /**
  * @brief  CREG指令，查询注册上网络
  * @param  timeout：超时时间（1s的倍数）
  * @retval 0：正确  
			其他：错误
  */
char SIM800A_CREG(int timeout)
{
	while(timeout--){                                  //等待超时时间到0
		SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
		memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
		SIM800A_printf("AT+CREG?\r\n");                //发送指令，查看是否注册上
		delay_ms(1000);                                //延时1s
		printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CREG: 0,1"))        //如果接收到 +CREG: 0,1 表示注册上
			break;       						       //主动跳出while循环
		printf("%d ",timeout);                      //串口输出现在的超时时间
	}
	printf("\r\n");                                 //串口输出信息
	if(timeout<=0)return 1;                            //如果timeout<=0，说明超时时间到了，也没能收到+CREG: 0,1，返回1
	else return 0;		         				       //反之，表示正确，说明收到+CREG: 0,1，通过break主动跳出while
}

 /**
  * @brief  CSQ指令，查询信号
  * @param  timeout：超时时间（100ms的倍数）
  * @retval 0：正确   
			其他：错误
  */
char SIM800A_CSQ(int timeout)
{
	char  temp1[10],temp2[10] ,temp3[10];                    //定义3个临时缓冲区  
	
	SIM800A_RxCounter=0;                          			 //800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE); 			 //清空800C接收缓冲区 
	SIM800A_printf("AT+CSQ\r\n");            	    		 //发送指令,查询信号质量
	while(timeout--){                                        //等待超时时间到0
		delay_ms(100);                            			 //延时100ms
		if(strstr(SIM800A_RX_BUF,"+CSQ:")){		             //接收到 +CSQ: 表示指令成功
			sscanf(SIM800A_RX_BUF,"%[^ ] %[^,],%[^,]",temp1,temp2,temp3);   //格式化搜索，信号质量
			printf("信号强度范围0~31,99表示无信号\r\n");  //串口输出信息
			printf("信号强度：%s\r\n",temp2);             //串口输出信息
			break;       						             //主动跳出while循环
		}
		printf("%d ",timeout);                            //串口输出现在的超时时间
	}
	printf("\r\n");                                       //串口输出信息
	if(timeout<=0)return 1;                                  //如果timeout<=0，说明超时时间到了，也没能收到+CSQ:，返回1
	else return 0;		         				             //反之，表示正确，说明收到+CSQ:，通过break主动跳出while
}

 /**
  * @brief  CGATT指令，查询是否附着GPRS
  * @param  timeout：超时时间（1s的倍数）
  * @retval 0：正确  
			其他：错误
  */
char SIM800A_CGATT(int timeout)
{
	while(timeout--){                                  //等待超时时间到0
		SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
		memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
		SIM800A_printf("AT+CGATT?\r\n");               //发送指令，查询是否附着GPRS
		delay_ms(1000);                                //延时1s
		printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CGATT: 1"))		   //接收到 +CGATT: 1 表示指令成功
			break;       						       //主动跳出while循环
		printf("%d ",timeout);                      //串口输出现在的超时时间
	}
	printf("\r\n");                                 //串口输出信息
	if(timeout<=0)return 1;                            //如果timeout<=0，说明超时时间到了，也没能收到+CGATT: 1，返回1
	else return 0;		         				       //反之，表示正确，说明收到+CGATT: 1，通过break主动跳出while
}

 /**
  * @brief  激活数据网络 
  * @param  无 
  * @retval 0：正确   
			其他：错误
  */
char SIM800A_ActivateNetwork(void)
{	                                                              //反之如果time小于timeout，表示正确，通过break主动跳出while
	printf("等待激活GPRS... ...\r\n");                         //串口输出信息
	if(SIM800A_SendCmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",10)){  //发送激活GPRS命令，超时时间10s
		printf("激活GPRS失败\r\n");                            //串口输出信息
		return 1;                                                 //如果if成立，表示超时了，返回1
	}else{                                                        //反之，表示指令正确
		printf("激活GPRS成功\r\n");                            //串口输出信息
		printf("等待设置接入点... ...\r\n");                   //串口输出信息
		if(SIM800A_SendCmd("AT+SAPBR=3,1,\"APN\",\"CMIOT\"",10)){ //发送设置接入点命令，超时时间10s
			printf("设置接入点失败\r\n");                      //串口输出信息
			return 2;                                             //如果if成立，表示超时了，返回2
		}else{													  //反之，表示指令正确
			printf("设置接入点成功\r\n");                      //串口输出信息
			printf("等待打开GPRS网络... ...\r\n");             //串口输出信息
			if(SIM800A_SendCmd("AT+SAPBR=1,1",10)){               //发送打开网络命令，超时时间10s
				printf("打开GPRS网络失败\r\n");                //串口输出信息
				return 3;                                         //如果if成立，表示超时了，返回3
			}else printf("打开GPRS网络成功\r\n");              //串口输出信息	
		}
	}	
	return 0;
}

 /**
  * @brief  同服务器建立TCP连接
  * @param  timeout：超时时间（1s的倍数） 
  * @retval 0：正确
			其他：错误
  */
char SIM800A_TCPConnect(int timeout)
{
	SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
	SIM800A_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n",ServerIP,ServerPort); //发送指令，连接服务器
	while(timeout--){                              //等待超时时间到0
		delay_ms(1000);                            //延时1s
		if(strstr(SIM800A_RX_BUF,"CONNECT"))       //如果接收到CONNECT表示连接成功
			break;       						   //主动跳出while循环
		printf("%d ",timeout);                  //串口输出现在的超时时间
	}
	printf("\r\n");                             //串口输出信息
	if(timeout<=0)return 1;                        //如果timeout<=0，说明超时时间到了，也没能收到CONNECT，返回1
	else return 0;		         				   //反之，表示正确，说明收到CONNECT，通过break主动跳出while
}

 /**
  * @brief  同服务器断开TCP连接
  * @param  timeout：超时时间（1s的倍数）
  * @retval 0：正确   
			其他：错误
  */
char SIM800A_TCPClose(int timeout)
{
	SIM800A_RxCounter=0;                           //800C接收数据量变量清零                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //清空800C接收缓冲区 
	SIM800A_printf("AT+CIPCLOSE\r\n");             //发送指令，断开连接
	while(timeout--){                              //等待超时时间到0
		delay_ms(1000);                            //延时1s
		if(strstr(SIM800A_RX_BUF,"CLOSE OK"))      //如果接收到CLOSE OK表示连接成功
			break;       						   //主动跳出while循环
		printf("%d ",timeout);                  //串口输出现在的超时时间
	}
	printf("\r\n");                             //串口输出信息
	if(timeout<=0)return 1;                        //如果timeout<=0，说明超时时间到了，也没能收到CLOSE OK，返回1
	else return 0;		         				   //反之，表示正确，说明收到CLOSE OK，通过break主动跳出while
}

 /**
  * @brief  800C连接物联网云服务器
  * @param  data：数据
  * @retval 无
  */
char SIM800A_Connect_IoTServer(void)
{
	char i;                         //定义一个变量，用于for循环

	printf("请等待注册上网络... ...\r\n");        //串口输出信息
	if(SIM800A_CREG(10)){                            //等待注册上网络，超时单位1s，超时时间10s
		printf("注册网络超时，准备重启\r\n");     //串口输出信息
		return 2;                                    //返回2
	}else printf("注册上网络\r\n");               //串口输出信息
	
	if(SIM800A_CSQ(10)){                             //查询信号强度，超时单位100ms，超时时间1s
		printf("查询信号强度超时，准备重启\r\n"); //串口输出信息
		return 3;                                    //返回3	
	}
	
	printf("请等待附着上GPRS... ...\r\n");        //串口输出信息
	if(SIM800A_CGATT(10)){                           //查询附着GPRS，超时单位1s，超时时间10s
		printf("查询附着GPRS超时，准备重启\r\n"); //串口输出信息
		return 4;                                    //返回4	
	}else printf("附着上GPRS\r\n");               //串口输出信息
	
	printf("请等待设置单链接... ...\r\n");        //串口输出信息
	if(SIM800A_SendCmd("AT+CIPMUX=0",10)){           //设置单链接模式，超时单位1s，超时时间10s
		printf(SIM800A_RX_BUF);
		printf("设置单链接失败，准备重启\r\n");   //串口输出信息
		return 5;                                    //返回5	
	}else printf("设置单链接模式成功\r\n");       //串口输出信息

	printf("请等待设置透传... ...\r\n");         //串口输出信息
	if(SIM800A_SendCmd("AT+CIPMODE=1",10)){         //设置透传模式，超时单位1s，超时时间10s		
		printf("设置透传失败，准备重启\r\n");    //串口输出信息
		return 6;                                   //返回6
	}else printf("设置透传成功\r\n");            //串口输出信息   
	
	if(SIM800A_ActivateNetwork()){                   //准备激活网络
		printf("激活网络失败，准备重启\r\n");     //串口输出信息
		return 7;                                    //返回7
	}
	delay_ms(500);                                   //适当延时
	for(i=0;i<3;i++)                                 //重试3次                                   
	{
		printf("请等待连接上服务器... ...\r\n");  //串口输出信息
		if(SIM800A_TCPConnect(10)){                  //同服务器建立TCP连接，超时单位1s，超时时间10s 
			printf("连接失败，准备再次连接\r\n"); //串口输出信息
			delay_ms(500);                           //适当延时
			if(SIM800A_TCPClose(10)){                //准备再次连接前要先发送关闭指令，超时单位1s，超时时间10s 
				printf("连接异常，准备重启\r\n"); //串口输出信息
				delay_ms(500);                       //适当延时
				return 8;                            //返回7
			}
		}else return 0;                              //正确，返回0
	}
	printf("连接不上服务器，准备重启\r\n");       //串口输出信息
	return 9;                                        //返回9
}



/*********************************************END OF FILE**********************/
