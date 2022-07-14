/**
  ******************************************************************************
  * @file    bsp_atgm336h.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-13
  * @brief   gpsʵ�ֶ�λ����
  ******************************************************************************
  */

#include "./gps/bsp_atgm336h.h"
#include "usart.h"
#include "./uart4/uart4.h"


char gps_data[128];

 /**
  * @brief  �����⺯��
  * @param  num���������
  * @retval ��
  */
void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

 /**
  * @brief  GPS��λ���ݷ�����ȡ����
  * @param  ��
  * @retval ��
  */
void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);

		
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//��ȡUTCʱ��
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//�ж϶�λ��Ϣ�Ƿ�ɹ���ȡ
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//��ȡγ����Ϣ
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//��ȡN/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//��ȡ������Ϣ
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//��ȡE/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//��������
				}
			}
		}
	}
}

 /**
  * @brief  GPS���ݷ�����ӡ����
  * @param  ��
  * @retval ��
  */
void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}


/*typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
} _SaveData;*/

void GPS_data_Buff(void)
{
	//�������
	memset(gps_data,0,128);
	
	//����д��
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		if(Save_Data.isUsefull)
		{
			memcpy(gps_data,Save_Data.latitude,strlen(Save_Data.latitude));
			memcpy(gps_data,Save_Data.N_S,strlen(Save_Data.N_S));		
			memcpy(gps_data,Save_Data.longitude,strlen(Save_Data.longitude));
			memcpy(gps_data,Save_Data.E_W,strlen(Save_Data.E_W));
		}
		else
		{
			memcpy(gps_data,"null",strlen("null"));
		}
	}
}


/*********************************************END OF FILE**********************/
