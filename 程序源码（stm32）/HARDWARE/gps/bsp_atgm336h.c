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


char gps_data[128];

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

 /**
  * @brief  mqtt发送的gps数据段
  * @param  无
  * @retval 无
  */
void GPS_data_Buff(void)
{
	//数据清空
	memset(gps_data,0,128);
	
	//数据写入
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
