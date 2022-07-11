/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  chen
  * @version V1.0
  * @date    2022-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  */


#include "stm32f4xx.h"                 
#include "./sim800a/bsp_sim800a.h"	
#include "usart.h"
#include "delay.h"
#include "./mqtt/mqtt.h"

// /**
//  * @brief  ��ʼ��SIM800C���ص�GPIO
//  * @param  ��
//  * @retval ��
//  */
//void SIM800C_GPIO_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;                      //����һ������IO�˿ڲ����Ľṹ��
//	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC , ENABLE);   //ʹ��PC�˿�ʱ��
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                 //׼������PC0
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   		  //�����ʽ
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			  //����ģʽ
//	GPIO_Init(GPIOC, &GPIO_InitStructure);            		  //����PC0
//	
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;                //׼������PC1
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //����50Mhz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   		  	  //����ģʽ
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;				  //����ģʽ
//	GPIO_Init(GPIOC, &GPIO_InitStructure);            		  //����PC1
//}

// /**
//  * @brief  SIM800C����
//  * @param  ��
//  * @retval 0:�����ɹ�
//			1:����ʧ��
//  */
//char SIM800C_Power(void)
//{
//	if(POWER_STA==0){   							           //���PC1�ǵ͵�ƽ����ʾĿǰ���ڹػ�״̬		
//		printf("\r\nĿǰ���ڹػ�״̬��׼������\r\n");       //���������Ϣ
//		POWER_KEY(1);                                          //������PC0
//		delay_ms(1500);										   //��ʱ
//		POWER_KEY(0);										   //������PC0������
//	}else{                                                     //��֮PC1�Ǹߵ�ƽ����ʾĿǰ���ڿ���״̬
//		printf("\r\nĿǰ���ڿ���״̬��׼������\r\n");       //���������Ϣ		
//		POWER_KEY(1);                                          //������PC0
//		delay_ms(1500);										   //��ʱ
//		POWER_KEY(0);										   //������PC0���ػ�
//		delay_ms(1200);                                        //���
//		POWER_KEY(1);                                          //������PC0
//		delay_ms(1500);										   //��ʱ
//		POWER_KEY(0);										   //������PC0���������������
//	}	
//	delay_ms(2000);			                                   //��ʱ���ȴ�״̬�ȶ�
//	if(POWER_STA){                                             //���PC1�ǵ͵�ƽ����ʾ�������������ɹ�
//		printf("�����ɹ�\r\n");                             //���������Ϣ
//	}else{												       //��֮PC1�Ǹߵ�ƽ����ʾ��ʼ��������ʧ��
//		printf("����ʧ��\r\n"); 							   //���������Ϣ
//		return 1;                                              //����1����ʾʧ��
//	}	
//	return 0;                                                  //����0����ʾ�ɹ�
//}

 /**
  * @brief  ��������ָ��
  * @param  cmd��ָ��
			timeout����ʱʱ�䣨100ms�ı�����
  * @retval 0����ȷ
			����������
  */
char SIM800C_SendCmd(char *cmd, int timeout)
{
	SIM800C_RxCounter=0;                           //800C������������������                        
	memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800C_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(100);                             //��ʱ100ms
		if(strstr(SIM800C_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						   //��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                        //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				   //��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}

 /**
  * @brief  CREGָ���ѯע��������
  * @param  timeout����ʱʱ�䣨1s�ı�����
  * @retval 0����ȷ  
			����������
  */
char SIM800C_CREG(int timeout)
{
	while(timeout--){                                  //�ȴ���ʱʱ�䵽0
		SIM800C_RxCounter=0;                           //800C������������������                        
		memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);  //���800C���ջ����� 
		SIM800C_printf("AT+CREG?\r\n");                //����ָ��鿴�Ƿ�ע����
		delay_ms(1000);                                //��ʱ1s
		if(strstr(SIM800C_RX_BUF,"+CREG: 0,1"))        //������յ� +CREG: 0,1 ��ʾע����
			break;       						       //��������whileѭ��
		printf("%d ",timeout);                      //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                                 //���������Ϣ
	if(timeout<=0)return 1;                            //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+CREG: 0,1������1
	else return 0;		         				       //��֮����ʾ��ȷ��˵���յ�+CREG: 0,1��ͨ��break��������while
}

 /**
  * @brief  CSQָ���ѯ�ź�
  * @param  timeout����ʱʱ�䣨100ms�ı�����
  * @retval 0����ȷ   
			����������
  */
char SIM800C_CSQ(int timeout)
{
	char  temp1[10],temp2[10] ,temp3[10];                    //����3����ʱ������  
	
	SIM800C_RxCounter=0;                          			 //800C������������������                        
	memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE); 			 //���800C���ջ����� 
	SIM800C_printf("AT+CSQ\r\n");            	    		 //����ָ��,��ѯ�ź�����
	while(timeout--){                                        //�ȴ���ʱʱ�䵽0
		delay_ms(100);                            			 //��ʱ100ms
		if(strstr(SIM800C_RX_BUF,"+CSQ:")){		             //���յ� +CSQ: ��ʾָ��ɹ�
			sscanf(SIM800C_RX_BUF,"%[^ ] %[^,],%[^,]",temp1,temp2,temp3);   //��ʽ���������ź�����
			printf("�ź�ǿ�ȷ�Χ0~31,99��ʾ���ź�\r\n");  //���������Ϣ
			printf("�ź�ǿ�ȣ�%s\r\n",temp2);             //���������Ϣ
			break;       						             //��������whileѭ��
		}
		printf("%d ",timeout);                            //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                                       //���������Ϣ
	if(timeout<=0)return 1;                                  //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+CSQ:������1
	else return 0;		         				             //��֮����ʾ��ȷ��˵���յ�+CSQ:��ͨ��break��������while
}

 /**
  * @brief  CGATTָ���ѯ�Ƿ���GPRS
  * @param  timeout����ʱʱ�䣨1s�ı�����
  * @retval 0����ȷ  
			����������
  */
char SIM800C_CGATT(int timeout)
{
	while(timeout--){                                  //�ȴ���ʱʱ�䵽0
		SIM800C_RxCounter=0;                           //800C������������������                        
		memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);  //���800C���ջ����� 
		SIM800C_printf("AT+CGATT?\r\n");               //����ָ���ѯ�Ƿ���GPRS
		delay_ms(1000);                                //��ʱ1s
		if(strstr(SIM800C_RX_BUF,"+CGATT: 1"))		   //���յ� +CGATT: 1 ��ʾָ��ɹ�
			break;       						       //��������whileѭ��
		printf("%d ",timeout);                      //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                                 //���������Ϣ
	if(timeout<=0)return 1;                            //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+CGATT: 1������1
	else return 0;		         				       //��֮����ʾ��ȷ��˵���յ�+CGATT: 1��ͨ��break��������while
}

 /**
  * @brief  ������������ 
  * @param  �� 
  * @retval 0����ȷ   
			����������
  */
char SIM800C_ActivateNetwork(void)
{	                                                              //��֮���timeС��timeout����ʾ��ȷ��ͨ��break��������while
	printf("�ȴ�����GPRS... ...\r\n");                         //���������Ϣ
	if(SIM800C_SendCmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",60)){  //���ͼ���GPRS�����ʱʱ��6s
		printf("����GPRSʧ��\r\n");                            //���������Ϣ
		return 1;                                                 //���if��������ʾ��ʱ�ˣ�����1
	}else{                                                        //��֮����ʾָ����ȷ
		printf("����GPRS�ɹ�\r\n");                            //���������Ϣ
		printf("�ȴ����ý����... ...\r\n");                   //���������Ϣ
		if(SIM800C_SendCmd("AT+SAPBR=3,1,\"APN\",\"CMIOT\"",60)){ //�������ý���������ʱʱ��6s
			printf("���ý����ʧ��\r\n");                      //���������Ϣ
			return 2;                                             //���if��������ʾ��ʱ�ˣ�����2
		}else{													  //��֮����ʾָ����ȷ
			printf("���ý����ɹ�\r\n");                      //���������Ϣ
			printf("�ȴ���GPRS����... ...\r\n");             //���������Ϣ
			if(SIM800C_SendCmd("AT+SAPBR=1,1",60)){               //���ʹ����������ʱʱ��6s
				printf("��GPRS����ʧ��\r\n");                //���������Ϣ
				return 3;                                         //���if��������ʾ��ʱ�ˣ�����3
			}else printf("��GPRS����ɹ�\r\n");              //���������Ϣ	
		}
	}	
	return 0;
}

 /**
  * @brief  ͬ����������TCP����
  * @param  timeout����ʱʱ�䣨1s�ı����� 
  * @retval 0����ȷ
			����������
  */
char SIM800C_TCPConnect(int timeout)
{
	SIM800C_RxCounter=0;                           //800C������������������                        
	memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800C_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n",ServerIP,ServerPort); //����ָ����ӷ�����
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                            //��ʱ1s
		if(strstr(SIM800C_RX_BUF,"CONNECT"))       //������յ�CONNECT��ʾ���ӳɹ�
			break;       						   //��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                        //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�CONNECT������1
	else return 0;		         				   //��֮����ʾ��ȷ��˵���յ�CONNECT��ͨ��break��������while
}

 /**
  * @brief  ͬ�������Ͽ�TCP����
  * @param  timeout����ʱʱ�䣨1s�ı�����
  * @retval 0����ȷ   
			����������
  */
char SIM800C_TCPClose(int timeout)
{
	SIM800C_RxCounter=0;                           //800C������������������                        
	memset(SIM800C_RX_BUF,0,SIM800C_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800C_printf("AT+CIPCLOSE\r\n");             //����ָ��Ͽ�����
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                            //��ʱ1s
		if(strstr(SIM800C_RX_BUF,"CLOSE OK"))      //������յ�CLOSE OK��ʾ���ӳɹ�
			break;       						   //��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                        //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�CLOSE OK������1
	else return 0;		         				   //��֮����ʾ��ȷ��˵���յ�CLOSE OK��ͨ��break��������while
}

 /**
  * @brief  800C�����������Ʒ�����
  * @param  data������
  * @retval ��
  */
char SIM800C_Connect_IoTServer(void)
{
	char i;                         //����һ������������forѭ��
	
	//SIM800C_GPIO_Init();            //����800C��IO��ʼ��
	//if(SIM800C_Power()){            //����800C����������,�������1��˵����������ʧ�ܣ�׼������
	//	return 1;                   //����1
	//}

	printf("��ȴ�ע��������... ...\r\n");        //���������Ϣ
	if(SIM800C_CREG(30)){                            //�ȴ�ע�������磬��ʱ��λ1s����ʱʱ��30s
		printf("ע�����糬ʱ��׼������\r\n");     //���������Ϣ
		return 2;                                    //����2
	}else printf("ע��������\r\n");               //���������Ϣ
	
	if(SIM800C_CSQ(60)){                             //��ѯ�ź�ǿ�ȣ���ʱ��λ100ms����ʱʱ��6s
		printf("��ѯ�ź�ǿ�ȳ�ʱ��׼������\r\n"); //���������Ϣ
		return 3;                                    //����3	
	}
	
	printf("��ȴ�������GPRS... ...\r\n");        //���������Ϣ
	if(SIM800C_CGATT(30)){                           //��ѯ����GPRS����ʱ��λ1s����ʱʱ��30s
		printf("��ѯ����GPRS��ʱ��׼������\r\n"); //���������Ϣ
		return 4;                                    //����4	
	}else printf("������GPRS\r\n");               //���������Ϣ
	
	printf("��ȴ����õ�����... ...\r\n");        //���������Ϣ
	if(SIM800C_SendCmd("AT+CIPMUX=0",60)){           //���õ�����ģʽ����ʱ��λ100ms����ʱʱ��6s	
		printf("���õ�����ʧ�ܣ�׼������\r\n");   //���������Ϣ
		return 5;                                    //����5	
	}else printf("���õ�����ģʽ�ɹ�\r\n");       //���������Ϣ

	printf("��ȴ�����͸��... ...\r\n");         //���������Ϣ
	if(SIM800C_SendCmd("AT+CIPMODE=1",60)){         //����͸��ģʽ����ʱ��λ100ms����ʱʱ��6s		
		printf("����͸��ʧ�ܣ�׼������\r\n");    //���������Ϣ
		return 6;                                   //����6
	}else printf("����͸���ɹ�\r\n");            //���������Ϣ   
	
	if(SIM800C_ActivateNetwork()){                   //׼����������
		printf("��������ʧ�ܣ�׼������\r\n");     //���������Ϣ
		return 7;                                    //����7
	}
	delay_ms(500);                                   //�ʵ���ʱ
	for(i=0;i<3;i++)                                 //����3��                                   
	{
		printf("��ȴ������Ϸ�����... ...\r\n");  //���������Ϣ
		if(SIM800C_TCPConnect(10)){                  //ͬ����������TCP���ӣ���ʱ��λ1s����ʱʱ��10s 
			printf("����ʧ�ܣ�׼���ٴ�����\r\n"); //���������Ϣ
			delay_ms(500);                           //�ʵ���ʱ
			if(SIM800C_TCPClose(10)){                //׼���ٴ�����ǰҪ�ȷ��͹ر�ָ���ʱ��λ1s����ʱʱ��10s 
				printf("�����쳣��׼������\r\n"); //���������Ϣ
				delay_ms(500);                       //�ʵ���ʱ
				return 8;                            //����7
			}
		}else return 0;                              //��ȷ������0
	}
	printf("���Ӳ��Ϸ�������׼������\r\n");       //���������Ϣ
	return 9;                                        //����9
}



/*********************************************END OF FILE**********************/
