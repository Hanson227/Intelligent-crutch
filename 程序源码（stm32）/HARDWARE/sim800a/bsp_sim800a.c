/**
  ******************************************************************************
  * @file    bsp_sim800a.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-12
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  */

#include "stm32f4xx.h"                 
#include "./sim800a/bsp_sim800a.h"	
#include "usart.h"
#include "delay.h"
#include "./mqtt/mqtt.h"

 /**
  * @brief  ��������ָ��
  * @param  cmd��ָ��
			timeout����ʱʱ�䣨100ms�ı�����
  * @retval 0����ȷ
			����������
  */
char SIM800A_SendCmd(char *cmd, int timeout)
{
	SIM800A_RxCounter=0;                           //800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800A_printf("%s\r\n",cmd);                  //����ָ��
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(SIM800A_RX_BUF,"OK"))            //������յ�OK��ʾָ��ɹ�
			break;       						   //��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                        //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;		         				   //��֮����ʾ��ȷ��˵���յ�OK��ͨ��break��������while
}

 /**
  * @brief  
  * @param  
  * @retval 
  */
char SIM800A_CMGS(int timeout,char *call_cmd)
{
	SIM800A_RxCounter=0;                           //800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800A_printf("%s\r\n",call_cmd); 
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                             //��ʱ1s
		if(strstr(SIM800A_RX_BUF,">"))            //������յ�OK��ʾָ��ɹ�
			break;       						   //��������whileѭ��
		printf("%d ",timeout);                  //����������ڵĳ�ʱʱ��
	}
	printf("\r\n");                             //���������Ϣ
	if(timeout<=0)return 1;                        //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�OK������1
	else return 0;	
}



 /**
  * @brief  
  * @param  
  * @retval 
  */
char SIM800A_Note_Edit(char *call_cmd)
{
	printf("�ȴ������ַ���... ...\r\n");
	if(SIM800A_SendCmd("AT+CSCS=\"GSM\"",10)){
		printf("�����ַ���ʧ��\r\n");
		return 1;
	}else{
		printf("�����ַ����ɹ�\r\n");
		printf(SIM800A_RX_BUF);
		printf("�ȴ������ı�ģʽ... ...\r\n");
		
		if(SIM800A_SendCmd("AT+CMGF=1",10)){
			printf("�����ı�ģʽʧ��\r\n");
			return 2;
		}else{
			printf("�����ı�ģʽ�ɹ�\r\n");
			printf(SIM800A_RX_BUF);
			printf("�ȴ�������ϵ��... ...\r\n");
			if(SIM800A_CMGS(10,call_cmd)){
				printf("������ϵ��ʧ��\r\n");
				return 2;
			}else{
				printf("������ϵ�˳ɹ�\r\n");
				printf(SIM800A_RX_BUF);
				return 0;
			}
		}
	}	
}

char SIM800A_Note_Send(int timeout)
{
	SIM800A_RxCounter=0;//800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);//���800C���ջ����� 
	USART_SendData(USART3, 0x1a);
	
	while(timeout--)
	{
		delay_ms(1000);//��ʱ1s
		//printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CMGS:"))//������յ� +CREG:
			break;//��������whileѭ��
		printf("%d ",timeout);  
	}
	printf("\r\n");                                       //���������Ϣ
	if(timeout<=0)return 1;                                  //���timeout<=0��˵����ʱʱ�䵽�ˣ�Ҳû���յ�+CSQ:������1
	else{
		printf("���Ͷ��ųɹ�");
		return 0;
	}
}


 /**
  * @brief  CREGָ���ѯע��������
  * @param  timeout����ʱʱ�䣨1s�ı�����
  * @retval 0����ȷ  
			����������
  */
char SIM800A_CREG(int timeout)
{
	while(timeout--){                                  //�ȴ���ʱʱ�䵽0
		SIM800A_RxCounter=0;                           //800C������������������                        
		memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
		SIM800A_printf("AT+CREG?\r\n");                //����ָ��鿴�Ƿ�ע����
		delay_ms(1000);                                //��ʱ1s
		printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CREG: 0,1"))        //������յ� +CREG: 0,1 ��ʾע����
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
char SIM800A_CSQ(int timeout)
{
	char  temp1[10],temp2[10] ,temp3[10];                    //����3����ʱ������  
	
	SIM800A_RxCounter=0;                          			 //800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE); 			 //���800C���ջ����� 
	SIM800A_printf("AT+CSQ\r\n");            	    		 //����ָ��,��ѯ�ź�����
	while(timeout--){                                        //�ȴ���ʱʱ�䵽0
		delay_ms(100);                            			 //��ʱ100ms
		if(strstr(SIM800A_RX_BUF,"+CSQ:")){		             //���յ� +CSQ: ��ʾָ��ɹ�
			sscanf(SIM800A_RX_BUF,"%[^ ] %[^,],%[^,]",temp1,temp2,temp3);   //��ʽ���������ź�����
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
char SIM800A_CGATT(int timeout)
{
	while(timeout--){                                  //�ȴ���ʱʱ�䵽0
		SIM800A_RxCounter=0;                           //800C������������������                        
		memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
		SIM800A_printf("AT+CGATT?\r\n");               //����ָ���ѯ�Ƿ���GPRS
		delay_ms(1000);                                //��ʱ1s
		printf(SIM800A_RX_BUF);
		if(strstr(SIM800A_RX_BUF,"+CGATT: 1"))		   //���յ� +CGATT: 1 ��ʾָ��ɹ�
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
char SIM800A_ActivateNetwork(void)
{	                                                              //��֮���timeС��timeout����ʾ��ȷ��ͨ��break��������while
	printf("�ȴ�����GPRS... ...\r\n");                         //���������Ϣ
	if(SIM800A_SendCmd("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"",10)){  //���ͼ���GPRS�����ʱʱ��10s
		printf("����GPRSʧ��\r\n");                            //���������Ϣ
		return 1;                                                 //���if��������ʾ��ʱ�ˣ�����1
	}else{                                                        //��֮����ʾָ����ȷ
		printf("����GPRS�ɹ�\r\n");                            //���������Ϣ
		printf("�ȴ����ý����... ...\r\n");                   //���������Ϣ
		if(SIM800A_SendCmd("AT+SAPBR=3,1,\"APN\",\"CMIOT\"",10)){ //�������ý���������ʱʱ��10s
			printf("���ý����ʧ��\r\n");                      //���������Ϣ
			return 2;                                             //���if��������ʾ��ʱ�ˣ�����2
		}else{													  //��֮����ʾָ����ȷ
			printf("���ý����ɹ�\r\n");                      //���������Ϣ
			printf("�ȴ���GPRS����... ...\r\n");             //���������Ϣ
			if(SIM800A_SendCmd("AT+SAPBR=1,1",10)){               //���ʹ����������ʱʱ��10s
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
char SIM800A_TCPConnect(int timeout)
{
	SIM800A_RxCounter=0;                           //800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800A_printf("AT+CIPSTART=\"TCP\",\"%s\",\"%d\"\r\n",ServerIP,ServerPort); //����ָ����ӷ�����
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                            //��ʱ1s
		if(strstr(SIM800A_RX_BUF,"CONNECT"))       //������յ�CONNECT��ʾ���ӳɹ�
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
char SIM800A_TCPClose(int timeout)
{
	SIM800A_RxCounter=0;                           //800C������������������                        
	memset(SIM800A_RX_BUF,0,SIM800A_RXBUFF_SIZE);  //���800C���ջ����� 
	SIM800A_printf("AT+CIPCLOSE\r\n");             //����ָ��Ͽ�����
	while(timeout--){                              //�ȴ���ʱʱ�䵽0
		delay_ms(1000);                            //��ʱ1s
		if(strstr(SIM800A_RX_BUF,"CLOSE OK"))      //������յ�CLOSE OK��ʾ���ӳɹ�
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
char SIM800A_Connect_IoTServer(void)
{
	char i;                         //����һ������������forѭ��

	printf("��ȴ�ע��������... ...\r\n");        //���������Ϣ
	if(SIM800A_CREG(10)){                            //�ȴ�ע�������磬��ʱ��λ1s����ʱʱ��10s
		printf("ע�����糬ʱ��׼������\r\n");     //���������Ϣ
		return 2;                                    //����2
	}else printf("ע��������\r\n");               //���������Ϣ
	
	if(SIM800A_CSQ(10)){                             //��ѯ�ź�ǿ�ȣ���ʱ��λ100ms����ʱʱ��1s
		printf("��ѯ�ź�ǿ�ȳ�ʱ��׼������\r\n"); //���������Ϣ
		return 3;                                    //����3	
	}
	
	printf("��ȴ�������GPRS... ...\r\n");        //���������Ϣ
	if(SIM800A_CGATT(10)){                           //��ѯ����GPRS����ʱ��λ1s����ʱʱ��10s
		printf("��ѯ����GPRS��ʱ��׼������\r\n"); //���������Ϣ
		return 4;                                    //����4	
	}else printf("������GPRS\r\n");               //���������Ϣ
	
	printf("��ȴ����õ�����... ...\r\n");        //���������Ϣ
	if(SIM800A_SendCmd("AT+CIPMUX=0",10)){           //���õ�����ģʽ����ʱ��λ1s����ʱʱ��10s
		printf(SIM800A_RX_BUF);
		printf("���õ�����ʧ�ܣ�׼������\r\n");   //���������Ϣ
		return 5;                                    //����5	
	}else printf("���õ�����ģʽ�ɹ�\r\n");       //���������Ϣ

	printf("��ȴ�����͸��... ...\r\n");         //���������Ϣ
	if(SIM800A_SendCmd("AT+CIPMODE=1",10)){         //����͸��ģʽ����ʱ��λ1s����ʱʱ��10s		
		printf("����͸��ʧ�ܣ�׼������\r\n");    //���������Ϣ
		return 6;                                   //����6
	}else printf("����͸���ɹ�\r\n");            //���������Ϣ   
	
	if(SIM800A_ActivateNetwork()){                   //׼����������
		printf("��������ʧ�ܣ�׼������\r\n");     //���������Ϣ
		return 7;                                    //����7
	}
	delay_ms(500);                                   //�ʵ���ʱ
	for(i=0;i<3;i++)                                 //����3��                                   
	{
		printf("��ȴ������Ϸ�����... ...\r\n");  //���������Ϣ
		if(SIM800A_TCPConnect(10)){                  //ͬ����������TCP���ӣ���ʱ��λ1s����ʱʱ��10s 
			printf("����ʧ�ܣ�׼���ٴ�����\r\n"); //���������Ϣ
			delay_ms(500);                           //�ʵ���ʱ
			if(SIM800A_TCPClose(10)){                //׼���ٴ�����ǰҪ�ȷ��͹ر�ָ���ʱ��λ1s����ʱʱ��10s 
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
