/**
  ******************************************************************************
  * @file    bsp_atgm336h.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-13
  * @brief   gps实现定位功能
  ******************************************************************************
  */

#include "./gps/bsp_atgm336h.h"
#include "usart.h"
#include "./uart4/uart4.h"
#include "stdlib.h"





 /**
  * @brief  错误检测函数
  * @param  num：错误参数
  * @retval 无
  */
void errorLog(int num)
{
	
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

 /**
  * @brief  GPS定位数据分析获取函数
  * @param  无
  * @retval 无
  */
void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i=0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		//printf("**************\r\n");
		//printf(Save_Data.GPS_Buffer);
		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//判断定位信息是否成功获取
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

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
					errorLog(2);	//解析错误
				}
			}
		}
	}
}

char gps_data[128];
void pushGPSdata(void)
{
	int num_temp = 0;
	int i;
	char str_temp[12] = {0};
	int len = 0;
	
	memset(gps_data,0,128);//清除
	
	if(Save_Data.isUsefull)
	{	
		
		
		//UTCTime_Length 11 hhmmss.sss
		memcpy(gps_data,"UTCTime",strlen("UTCTime"));
		len += strlen("UTCTime");
		memcpy(&gps_data[len],Save_Data.UTCTime,strlen(Save_Data.UTCTime));
		len += strlen(Save_Data.UTCTime);
		
		
		//latitude_Length 11 ddmm.mmmm
		for(i = 2; i<9; i++)
		{
			if (i != 4)
			{
				num_temp *= 10;
			}
			if ((i>=2)&&(i != 4))
				num_temp += (Save_Data.latitude[i] - '0');
		}
		num_temp =(int)(((double)num_temp) / 0.6);
		memcpy(str_temp, Save_Data.latitude, 3);
		str_temp[2] = '.';
		for (i = 1; i <= 6; i++)
		{
			str_temp[9 - i] = num_temp % 10 + '0';
			num_temp /= 10;
		}
		memcpy(&gps_data[len],",N",2);
		len += 2;
		memcpy(&gps_data[len],str_temp,strlen(str_temp));
		len += strlen(str_temp);
		
		
		//longitude_Length 12 dddmm.mmmm
		memset(str_temp,0,12);
		num_temp=0;
		for(i = 3; i<10; i++)
		{
			if (i != 5)
			{
				num_temp *= 10;
			}
			if ((i>=3)&&(i != 5))
				num_temp += (Save_Data.longitude[i] - '0');
		}
		num_temp =(int)(((double)num_temp) / 0.6);
		memcpy(str_temp, Save_Data.longitude, 2);
		str_temp[3] = '.';
		for (i = 1; i <= 6; i++)
		{
			str_temp[10 - i] = num_temp % 10 + '0';
			num_temp /= 10;
		}
		memcpy(&gps_data[len],",E",2);
		len += 2;
		memcpy(&gps_data[len],str_temp,strlen(str_temp));
		len += strlen(str_temp);
	}
	else
	{
		//UTCTime_Length 11 hhmmss.sss
		memcpy(gps_data,"UTCTime",strlen("UTCTime"));
		len += strlen("UTCTime");
		memcpy(&gps_data[len],Save_Data.UTCTime,strlen(Save_Data.UTCTime));
		len += strlen(Save_Data.UTCTime);
		
		memcpy(gps_data,",null",strlen(",null"));
		len += strlen(",null");
	}
}

/*********************************************END OF FILE**********************/
