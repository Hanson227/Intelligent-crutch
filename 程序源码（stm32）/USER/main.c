/**
  ******************************************************************************
  * @file    main.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-08
  * @brief   ���ܹ����������ļ�
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
#include "./sim800a/bsp_sim800a.h"
#include "./hcsr04/bsp_hcsr04.h"
#include "./gps/bsp_atgm336h.h"
#include "./adxl345/bsp_adxl345.h"

#if 0
 /**
  * @brief  ������
  * @param  ��
  * @retval int
  */
int main(void)
{
	uint32_t a=0;
	debug_uart_init(115200);//USART1���ܳ�ʼ����������115200
	Usart2_Init(9600);	
	Usart3_Init(115200);//USART3���ܳ�ʼ����������115200
	Uart4_Init(9600);
	TIM4_Init(300,7200);//TIM4��ʼ������ʱʱ��300*7200*1000/72000000 = 30ms	
	delay_init(84);//��ʱ������ʼ����84M
	sr04_init();//��ʼ��������ģ��
	IoT_Parameter_Init();//��ʼ����IoTƽ̨MQTT�������Ĳ���
	Init_ADXL345();
	
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
		parseGpsBuffer();//����gps����
		pushGPSdata();//�������ݴ�����
		ReadData();//�����Ż�����ʱ���ݴ���
		delay_ms(1000);
		
		
		
		
		/*-------------------------------------------------------------*/
		/*          �����������ܣ��������ϵ�˷���������λ������         */
		/*-------------------------------------------------------------*/
		if(temp_X<-THRESHOLD||temp_X>THRESHOLD||
			temp_Y<-THRESHOLD||temp_Y>THRESHOLD||
			temp_Z<-THRESHOLD||temp_Z>THRESHOLD)
		{
			if(Save_Data.isParseData)
			{
				SIM800C_Note_Edit(EMERGENCY_CALL_CMD);
				SIM800C_printf("help me!\r\n%s",gps_data);
				SIM800C_Note_Send(30);
			}
			else
			{
				SIM800C_Note_Edit(EMERGENCY_CALL_CMD);
				SIM800C_printf("help me!");
				SIM800C_Note_Send(30);
			}
		}
		
		
		
		
		/*-------------------------------------------------------------*/
		/*   ���ڷ������ݣ�����ģ�����ʶ����ʾǰ���ϰ����������     */
		/*-------------------------------------------------------------*/
		a=sr04_get_distance();
		if(a>0)
		{
			if(a>=20&&a<=3000)
			{
				u2_printf("3");
			}
		}
		
		
		
		
		
		
		if(Connect_flag==1)//�����ɹ�
		{
			/*-------------------------------------------------------------*/
			/*             ����GPD���ݣ����ص����ͻ���������                */
			/*-------------------------------------------------------------*/
			if(SubcribePack_flag==1)//������ĳɹ�
			{
				
				
				delay_ms(1000);//���ڸĳɶ�ʱ����ʱ����
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
			
			
			
			
			
			
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
			{                
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||
					((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
					//������ʾ��Ϣ
					printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);
					MQTT_TxData(MQTT_TxDataOutPtr);//��������
					MQTT_TxDataOutPtr += TBUFF_UNIT;//ָ������
					//���ָ�뵽������β����
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)
						//ָ���λ����������ͷ
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];
				}
				else
					printf("���ݣ�0x%x\r\n",MQTT_TxDataOutPtr[2]);
			}//�����ͻ��������ݵ�else if��֧��β
		
			



			
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if�����Ļ���˵�����ջ�������������														
				printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}				
				}			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("���ĳɹ�\r\n");//���������Ϣ
							        SubcribePack_flag = 1;//���ı��ĳɹ����������Ŀɷ���									  
									break;//������֧                                             
						default   : printf("����ʧ�ܣ�׼������\r\n");//���������Ϣ 
									Connect_flag = 0;//Connect_flag���㣬��������
									break;//������֧ 								
					}					
				}
								
				//if�жϣ����һ��������10���ֽڣ���һ���ֽ���0x0D���п����յ��� CLOSED ��ʾ���ӶϿ�
				//���ǽ���else if�������ж�
				else if((MQTT_RxDataOutPtr[1]==10)&&(MQTT_RxDataOutPtr[2]==0x0D)){
					MQTT_RxDataOutPtr[12] = '\0';//�����ַ���������
					if(strstr(SIM800C_RX_BUF,"CLOSED")){//�����������CLOSED����ʾ���ӶϿ���						
						Connect_flag = 0;//����״̬��0����ʾ�Ͽ�����Ҫ����������
					}			
				}
				//if�жϣ����һ��������15���ֽڣ���һ���ֽ���0x0D���п����յ��� +PDP: DEACT ��ʾGPRS�Ͽ�
				//���ǽ���else if�������ж�
				else if((MQTT_RxDataOutPtr[1]==15)&&(MQTT_RxDataOutPtr[1]==0x0D)){
					MQTT_RxDataOutPtr[17] = '\0';                   //�����ַ���������
					printf("GPRS�Ͽ�\r\n"); 		                //���������Ϣ
					if(strstr(SIM800C_RX_BUF,"+PDP: DEACT")){       //���������+PDP: DEACT����ʾGPRS�Ͽ���						
						Connect_flag = 0;                           //����״̬��0����ʾ�Ͽ�����Ҫ����������
					}			
				}	
				
				MQTT_RxDataOutPtr +=RBUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
			}//������ջ��������ݵ�else if��֧��β	
		}//Connect_flag=1��if��֧�Ľ�β
		
		
		
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("��Ҫ���ӷ�����\r\n");//���������Ϣ
			
			 
			SIM800C_RxCounter=0;//800C������������������
			//���800C���ջ�����                        
			memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);     
			
			/*���800C�����Ʒ�������������0����ʾ��ȷ������if*/
			if(SIM800C_Connect_IoTServer()==0)
			{   			 
				printf("����TCP���ӳɹ�\r\n");//���������Ϣ
				u2_printf("4");////���ڷ������ݣ�����ģ�����ʶ����ʾ�������ӳɹ�
				Connect_flag = 1;//��1����ʾ���ӳɹ�	
				SIM800C_RxCounter=0;//800C������������������    
				//���800C���ջ�����                    
				memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE); 
				MQTT_Buff_Init();//��ʼ��������                    
			}				
		}
	}
}

#endif 


#if 0
 /**
  * @brief  ���������Ժ���
  * @param  ��
  * @retval int
  */
int main(void)
{	
	uint32_t a=0;
	//����1������:115200bps
	delay_init(84);//��ʱ������ʼ����84M
	debug_uart_init(115200);	
	Usart2_Init(9600);
	sr04_init();//��ʼ��������ģ��
	printf("������ģ�����");
	while(1)
	{
		
		a=sr04_get_distance();
		if(a>0)
		{
			if(a>=20&&a<=3000)
			{
				u2_printf("3");//���ڷ������ݣ�����ģ�����ʶ��
			}
		}
		printf("%d cm\r\n",a/10);
		delay_ms(1000);
	}
}

#endif

#if 0
 /**
  * @brief  GPS���Ժ���
  * @param  ��
  * @retval int
  */
int main(void)
{	
	delay_init(84);
	debug_uart_init(115200);	 //���ڳ�ʼ��
	Uart4_Init(9600);
		
	clrStruct();
	
	printf("GPSģ�����\r\n");
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
  * @brief  �������Ժ���
  * @param  ��
  * @retval int
  */
int main(void)
{
	delay_init(84);
	debug_uart_init(115200);	 //���ڳ�ʼ��
	Usart2_Init(9600);
	u2_printf("����ģ�����\r\n");
	while(1)
	{
	}	
}

#endif



#if 1
 /**
  * @brief  ADXL345ģ�����
  * @param  ��
  * @retval int
  */
int main()
{
	delay_init(84);
	debug_uart_init(115200);
	Usart3_Init(115200);//USART3���ܳ�ʼ����������115200
	Init_ADXL345();
	printf("ADXL345ģ�����\r\n");
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
		
		if(temp_X<-THRESHOLD||temp_X>THRESHOLD||
			temp_Y<-THRESHOLD||temp_Y>THRESHOLD||
			temp_Z<-THRESHOLD||temp_Z>THRESHOLD)
		{
			SIM800C_Note_Edit(EMERGENCY_CALL_CMD);
			SIM800C_printf("help me!");
			SIM800C_Note_Send(30);
		}
	}
}

#endif

/*********************************************END OF FILE**********************/

