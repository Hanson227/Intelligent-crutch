/**
  ******************************************************************************
  * @file    bsp_adxl345.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-21
  * @brief   ���ٶ�ģ��ˤ�����
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "delay.h"
#include "./adxl345/bsp_adxl345.h"


#define SDA_RCC		  	RCC_APB2Periph_GPIOA
#define SDA_GPIO		GPIOA
#define SDA_GPIO_PIN	GPIO_Pin_5

#define SCL_RCC			RCC_APB2Periph_GPIOA   //ʱ�Ӻ�
#define SCL_GPIO		GPIOA                  //�˿ں�
#define SCL_GPIO_PIN	GPIO_Pin_4             //���ź�

#define SCL_OUT() SCL_Set_Output()                       //����ʱ����Ϊ���ģʽ
#define SET_SCL() GPIO_SetBits(SCL_GPIO, SCL_GPIO_PIN)   //����ʱ����
#define CLE_SCL() GPIO_ResetBits(SCL_GPIO, SCL_GPIO_PIN) //����ʱ����
                    
#define SDA_OUT() SDA_Set_Output()                      //����������Ϊ���ģʽ
#define SDA_INT() SDA_Set_Input()                       //����������Ϊ����ģʽ
#define SET_SDA() GPIO_SetBits(SDA_GPIO, SDA_GPIO_PIN)  //����������
#define CLE_SDA() GPIO_ResetBits(SDA_GPIO, SDA_GPIO_PIN)//����������
#define SDA_VAL() GPIO_ReadInputDataBit(SDA_GPIO, SDA_GPIO_PIN)     //��ȡ������״̬

#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�         //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A\



unsigned char BUF[8];                        //�������ݻ�����      	
unsigned char ge,shi,bai,qian,wan;           //��ʾ����
unsigned char err;							 //����������
double temp_X,temp_Y,temp_Z;				 //������ٶ�
uint32_t help_time = 0;				 		 //ˤ��ʱ���ʱ
unsigned char help_flag = 0;				 //ˤ���ж�

 /**
  * @brief  ʱ���ź������ʼ��
  * @param  ��
  * @retval ��
  */
void SCL_Set_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SCL_GPIO, &GPIO_InitStructure);					 					
}	

 /**
  * @brief  �����ź������ʼ��
  * @param  ��
  * @retval ��
  */
void SDA_Set_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SDA_GPIO, &GPIO_InitStructure);					 					
}	

 /**
  * @brief  �����ź������ʼ��
  * @param  ��
  * @retval ��
  */
void SDA_Set_Input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SDA_GPIO, &GPIO_InitStructure);					 
}

 /**
  * @brief  ��ʼ�ź�
  * @param  ��
  * @retval ��
  */
void ADXL345_Start(void)
{
    SCL_OUT();
    SDA_OUT();
    SET_SDA();//SDA = 1;                    //����������
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();               //��ʱ
    CLE_SDA();//SDA = 0;                    //����������
    delay_us(2);//Delay5us();               //��ʱ
    CLE_SCL();//SCL = 0;                    //����ʱ����
}

 /**
  * @brief  ֹͣ�ź�
  * @param  ��
  * @retval ��
  */
void ADXL345_Stop(void)
{
    SCL_OUT();
    SDA_OUT();
    CLE_SDA();//SDA = 0;                    //����������
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    SET_SDA();//SDA = 1;                    //����������
    delay_us(2);//Delay5us();                 //��ʱ
    CLE_SCL();
}

 /**
  * @brief  ����Ӧ���ź�
  * @param  ack (0:ACK 1:NAK)
  * @retval 
  */
void ADXL345_SendACK(uchar ack)
{   
    SCL_OUT();
    SDA_OUT();
    if(ack==0)//SDA = ack;                  //дӦ���ź�
    {
      CLE_SDA();
    }
    else
    {
      SET_SDA();
    }
    SET_SCL();			                    //����ʱ����
    delay_us(2);                 			//��ʱ
    CLE_SCL();			                    //����ʱ����
    delay_us(5);			               	//��ʱ
}

 /**
  * @brief  ����Ӧ���ź�
  * @param  ��
  * @retval ��
  */
uchar ADXL345_RecvACK(void)
{
    SDA_INT();
    SCL_OUT();
    SET_SCL();				                 //����ʱ����
    delay_us(2);				             //��ʱ
    SET_SCL();
    if(SDA_VAL()== Bit_SET)					 //��Ӧ���ź�
    {
      err = 1;
    }
    else
    {
      err = 0;
    }
 
    CLE_SCL() ;			                    //����ʱ����
    delay_us(5);			                //��ʱ
    SDA_OUT();
    return err;
}

 /**
  * @brief  ��IIC���߷���һ���ֽ�����
  * @param  dat����������
  * @retval ��
  */
void ADXL345_SendByte(unsigned char dat)
{
    unsigned char i;
    SCL_OUT();
    SDA_OUT();
    for (i=0; i<8; i++)         				//8λ������
    {
        delay_us(5);             				//��ʱ
        if(dat&0x80)  			             	//�����ݿ�     
        {SET_SDA();}
        else
        {CLE_SDA();}       
        delay_us(5);             				//��ʱ
        SET_SCL();				                //����ʱ����
        delay_us(5);  					       	//��ʱ
        CLE_SCL();				                //����ʱ����
        dat <<= 1;              				//�Ƴ����ݵ����λ
    }
    ADXL345_RecvACK();
}

 /**
  * @brief  ��IIC���߽���һ���ֽ�����
  * @param  ��
  * @retval ��
  */
unsigned char ADXL345_RecvByte(void)
{
    unsigned char i;
    unsigned char Mid;
    unsigned char dat = 0;
    SDA_INT();
    SCL_OUT();

    for (i=0; i<8; i++)         				//8λ������
    {
        dat <<= 1;
        delay_us(5);            				//��ʱ
        SET_SCL();

			if(SDA_VAL()== Bit_SET)         	//��Ӧ���ź�
			{
				Mid = 1;
			}
			else
			{
				Mid = 0;
			}
        if(Mid) dat++;
        delay_us(5);     
        CLE_SCL();			                	//����ʱ����
    }
    return dat;
}

 /**
  * @brief  ���ֽ�д��
  * @param  REG_Address��д���ַ
			REG_data��д������
  * @retval ��
  */
void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ 
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ�������
    ADXL345_Stop();                   //����ֹͣ�ź�
}

 /**
  * @brief  ���ֽڶ�ȡ
  * @param  REG_Address����ȡ��ַ
  * @retval ��
  */
uchar Single_Read_ADXL345(uchar REG_Address)
{  
	uchar REG_data;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}

 /**
  * @brief  ��������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
  * @param  ��
  * @retval ��
  */
void Multiple_Read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
	{
        BUF[i] = ADXL345_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //��ӦACK
       }
	}
    ADXL345_Stop();                          //ֹͣ�ź�
    delay_us(5);
}

 /**
  * @brief  ��ʼ��ADXL345
  * @param  ��
  * @retval ��
  */
void Init_ADXL345(void)
{                       
	Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
	Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ25
	Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ
	Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
	Single_Write_ADXL345(0x1E,0x00);   //X ƫ����
	Single_Write_ADXL345(0x1F,0x00);   //Y ƫ����
	Single_Write_ADXL345(0x20,0x05);   //Z ƫ����
	if(Single_Read_ADXL345(0X00)==0xe5)	
	{
		delay_ms(5);
	}
	else
	{
		delay_ms(3);
	}
}
//***********************************************************************
 /**
  * @brief  ���ٶ�xyz���ݶ�ȡ
  * @param  ��
  * @retval ��
  */
void ReadData(void)
{   
	int  dis_data;                       //����
	Multiple_Read_ADXL345();       	//�����������ݣ��洢��BUF��
	dis_data=(BUF[1]<<8)+BUF[0];  //�ϳ�����   
	
	temp_X=(double)dis_data*3.9;  //�������ݺ���ʾ
	dis_data=(BUF[3]<<8)+BUF[2];  //�ϳ�����   
	
	temp_Y=(double)dis_data*3.9;  //�������ݺ���ʾ
	dis_data=(BUF[5]<<8)+BUF[4];    //�ϳ�����   
	
	temp_Z=(double)dis_data*3.9;  //�������ݺ���ʾ
	
	if(temp_X>200000.0)
			temp_X-=255586.5;
	if(temp_Y>200000.0)
			temp_Y-=255586.5;
	if(temp_Z>200000.0)
			temp_Z-=255586.5;
}


