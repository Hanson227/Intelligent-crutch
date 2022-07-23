#ifndef __BSP_ADXL345_H
#define __BSP_ADXL345_H

#define uint unsigned int
#define uchar unsigned char
	
#define THRESHOLD 7500.0 //Ë¤µ¹¼ì²âµÄãÐÖµ
	
extern double temp_X,temp_Y,temp_Z;


void SCL_Set_Output(void);
void SDA_Set_Output(void);
void SDA_Set_Input(void);
void Init_ADXL345(void);    
void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   
uchar Single_Read_ADXL345(uchar REG_Address);                   
void  Multiple_Read_ADXL345(void);                                 
void ADXL345_Start(void);
void ADXL345_Stop(void);
void ADXL345_SendACK(uchar ack);
uchar  ADXL345_RecvACK(void);
void ADXL345_SendByte(uchar dat);
uchar ADXL345_RecvByte(void);
void ADXL345_ReadPage(void);
void ADXL345_WritePage(void);
void ReadData(void);

#endif /*__BSP_ADXL345_H*/

