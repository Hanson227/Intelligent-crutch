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
		}
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




