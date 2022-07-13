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
  * @brief  ������
  * @param  ��
  * @retval int
  */
int main(void)
{
	uart_init(115200);//USART1���ܳ�ʼ����������115200				
	Usart3_Init(115200);//USART3���ܳ�ʼ����������115200
	TIM4_Init(300,7200);//TIM4��ʼ������ʱʱ��300*7200*1000/72000000 = 30ms	
	delay_init(84);//��ʱ������ʼ����84M
	IoT_Parameter_Init();//��ʼ����IoTƽ̨MQTT�������Ĳ���
	
	while(1)
	{
		if(Connect_flag==1)
		{
			//if�����Ļ���˵�����ͻ�������������
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
						case 0x01 : printf("���ĳɹ�\r\n");            //���������Ϣ
							        SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
									Ping_flag = 0;                        //Ping_flag����
   								    TIM6_ENABLE_30S();                    //����30s��PING��ʱ��
									//void MQTT_PublishQs0(char *topic, char *data, int data_len)
									MQTT_PublishQs0(P_TOPIC_NAME,"1234",4); //�������ݸ�������  
									break;                                //������֧                                             
						default   : printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
									Connect_flag = 0;                     //Connect_flag���㣬��������
									break;                                //������֧ 								
					}					
				}
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2]==0xD0){ 
					printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						TIM6_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2]==0x30)){ 
					printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
				}				
				//if�жϣ����һ��������10���ֽڣ���һ���ֽ���0x0D���п����յ��� CLOSED ��ʾ���ӶϿ�
				//���ǽ���else if�������ж�
				else if((MQTT_RxDataOutPtr[1]==10)&&(MQTT_RxDataOutPtr[2]==0x0D)){
					MQTT_RxDataOutPtr[12] = '\0';                   //�����ַ���������
					if(strstr(SIM800C_RX_BUF,"CLOSED")){            //�����������CLOSED����ʾ���ӶϿ���						
						Connect_flag = 0;                           //����״̬��0����ʾ�Ͽ�����Ҫ����������
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
			
			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/
//			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                             //if�����Ļ���˵�����������������			       
//				printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
//				if(!memcmp(&MQTT_CMDOutPtr[2],CMD1,strlen(CMD1))){           //�ж�ָ������CMD1
//					LED1_OUT(!LED1_IN_STA);                                  //����1״̬��ת ���������Ϩ�𣬷�֮���Ϩ��͵���
//					LED1_State();                                            //�жϿ��غ͵���ʱ״̬����������������
//				}else if(!memcmp(&MQTT_CMDOutPtr[2],CMD2,strlen(CMD2))){     //�ж�ָ������CMD2���ظ�����״̬
//					LED1_State();                                            //�жϿ��غ͵���ʱ״̬����������������				
//				}else printf("δָ֪��\r\n");                             //���������Ϣ
//				
//				MQTT_CMDOutPtr += CBUFF_UNIT;                             	 //ָ������
//				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
//					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
//			}//��������������ݵ�else if��֧��β		
		}//Connect_flag=1��if��֧�Ľ�β
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("��Ҫ���ӷ�����\r\n");//���������Ϣ
			TIM_Cmd(TIM4,DISABLE);//�ر�TIM4 
			TIM_Cmd(TIM6,DISABLE);//�ر�TIM6  
			SIM800C_RxCounter=0;//800C������������������
			//���800C���ջ�����                        
			memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);     
			
			/*���800C�����Ʒ�������������0����ʾ��ȷ������if*/
			if(SIM800C_Connect_IoTServer()==0)
			{   			 
				printf("����TCP���ӳɹ�\r\n");//���������Ϣ
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
	uart_init(115200);	
	sr04_init();//��ʼ��������ģ��
	printf("������ģ�����");
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
	uart_init(9600);	 //���ڳ�ʼ��
	UART4_Init(9600);
		
	clrStruct();
	
	printf("GPSģ�����");
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();   
	}
}

#endif

/*********************************************END OF FILE**********************/

