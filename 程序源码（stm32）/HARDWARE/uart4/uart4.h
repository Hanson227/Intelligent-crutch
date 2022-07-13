#ifndef __UART4_H
#define __UART4_H

#include "stdio.h"	
#include "sys.h"
#include "string.h"

#define UART4_REC_LEN  			200  	//�����������ֽ��� 200
	  	
extern char  UART4_RX_BUF[UART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART4_RX_STA;         		//����״̬���	



#define false 0
#define true 1
	
//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 

typedef struct SaveData 
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
} _SaveData;

extern char rxdatabufer;
extern u16 point1;
extern _SaveData Save_Data;

void UART4_Init(u32 bound);

void CLR_Buf(void);
u8 Hand(char *a);
void clrStruct(void);

#endif /*__UART4_H*/
