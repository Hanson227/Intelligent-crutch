/**
  ******************************************************************************
  * @file    bsp_adxl345.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-21
  * @brief   加速度模块摔倒检测
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "delay.h"
#include "./adxl345/bsp_adxl345.h"


#define SDA_RCC		  	RCC_APB2Periph_GPIOA
#define SDA_GPIO		GPIOA
#define SDA_GPIO_PIN	GPIO_Pin_5

#define SCL_RCC			RCC_APB2Periph_GPIOA   //时钟号
#define SCL_GPIO		GPIOA                  //端口号
#define SCL_GPIO_PIN	GPIO_Pin_4             //引脚号

#define SCL_OUT() SCL_Set_Output()                       //设置时钟线为输出模式
#define SET_SCL() GPIO_SetBits(SCL_GPIO, SCL_GPIO_PIN)   //拉高时钟线
#define CLE_SCL() GPIO_ResetBits(SCL_GPIO, SCL_GPIO_PIN) //拉低时钟线
                    
#define SDA_OUT() SDA_Set_Output()                      //设置数据线为输出模式
#define SDA_INT() SDA_Set_Input()                       //设置数据线为输入模式
#define SET_SDA() GPIO_SetBits(SDA_GPIO, SDA_GPIO_PIN)  //拉高数据线
#define CLE_SDA() GPIO_ResetBits(SDA_GPIO, SDA_GPIO_PIN)//拉低数据线
#define SDA_VAL() GPIO_ReadInputDataBit(SDA_GPIO, SDA_GPIO_PIN)     //读取数据线状态

#define	SlaveAddress   0xA6	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改         //ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A\



unsigned char BUF[8];                        //接收数据缓存区      	
unsigned char ge,shi,bai,qian,wan;           //显示变量
unsigned char err;							 //错误编码变量
double temp_X,temp_Y,temp_Z;				 //三轴加速度
uint32_t help_time = 0;				 		 //摔倒时间计时
unsigned char help_flag = 0;				 //摔倒判断

 /**
  * @brief  时钟信号输出初始化
  * @param  无
  * @retval 无
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
  * @brief  数据信号输出初始化
  * @param  无
  * @retval 无
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
  * @brief  数据信号输入初始化
  * @param  无
  * @retval 无
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
  * @brief  起始信号
  * @param  无
  * @retval 无
  */
void ADXL345_Start(void)
{
    SCL_OUT();
    SDA_OUT();
    SET_SDA();//SDA = 1;                    //拉高数据线
    SET_SCL();//SCL = 1;                    //拉高时钟线
    delay_us(2);//Delay5us();               //延时
    CLE_SDA();//SDA = 0;                    //拉低数据线
    delay_us(2);//Delay5us();               //延时
    CLE_SCL();//SCL = 0;                    //拉低时钟线
}

 /**
  * @brief  停止信号
  * @param  无
  * @retval 无
  */
void ADXL345_Stop(void)
{
    SCL_OUT();
    SDA_OUT();
    CLE_SDA();//SDA = 0;                    //拉低数据线
    SET_SCL();//SCL = 1;                    //拉高时钟线
    delay_us(2);//Delay5us();                 //延时
    SET_SDA();//SDA = 1;                    //产生上升沿
    delay_us(2);//Delay5us();                 //延时
    CLE_SCL();
}

 /**
  * @brief  发送应答信号
  * @param  ack (0:ACK 1:NAK)
  * @retval 
  */
void ADXL345_SendACK(uchar ack)
{   
    SCL_OUT();
    SDA_OUT();
    if(ack==0)//SDA = ack;                  //写应答信号
    {
      CLE_SDA();
    }
    else
    {
      SET_SDA();
    }
    SET_SCL();			                    //拉高时钟线
    delay_us(2);                 			//延时
    CLE_SCL();			                    //拉低时钟线
    delay_us(5);			               	//延时
}

 /**
  * @brief  接收应答信号
  * @param  无
  * @retval 无
  */
uchar ADXL345_RecvACK(void)
{
    SDA_INT();
    SCL_OUT();
    SET_SCL();				                 //拉高时钟线
    delay_us(2);				             //延时
    SET_SCL();
    if(SDA_VAL()== Bit_SET)					 //读应答信号
    {
      err = 1;
    }
    else
    {
      err = 0;
    }
 
    CLE_SCL() ;			                    //拉低时钟线
    delay_us(5);			                //延时
    SDA_OUT();
    return err;
}

 /**
  * @brief  向IIC总线发送一个字节数据
  * @param  dat：发送数据
  * @retval 无
  */
void ADXL345_SendByte(unsigned char dat)
{
    unsigned char i;
    SCL_OUT();
    SDA_OUT();
    for (i=0; i<8; i++)         				//8位计数器
    {
        delay_us(5);             				//延时
        if(dat&0x80)  			             	//送数据口     
        {SET_SDA();}
        else
        {CLE_SDA();}       
        delay_us(5);             				//延时
        SET_SCL();				                //拉高时钟线
        delay_us(5);  					       	//延时
        CLE_SCL();				                //拉低时钟线
        dat <<= 1;              				//移出数据的最高位
    }
    ADXL345_RecvACK();
}

 /**
  * @brief  从IIC总线接收一个字节数据
  * @param  无
  * @retval 无
  */
unsigned char ADXL345_RecvByte(void)
{
    unsigned char i;
    unsigned char Mid;
    unsigned char dat = 0;
    SDA_INT();
    SCL_OUT();

    for (i=0; i<8; i++)         				//8位计数器
    {
        dat <<= 1;
        delay_us(5);            				//延时
        SET_SCL();

			if(SDA_VAL()== Bit_SET)         	//读应答信号
			{
				Mid = 1;
			}
			else
			{
				Mid = 0;
			}
        if(Mid) dat++;
        delay_us(5);     
        CLE_SCL();			                	//拉低时钟线
    }
    return dat;
}

 /**
  * @brief  单字节写入
  * @param  REG_Address：写入地址
			REG_data：写入数据
  * @retval 无
  */
void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //起始信号
    ADXL345_SendByte(SlaveAddress);   //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);    //内部寄存器地址 
    ADXL345_SendByte(REG_data);       //内部寄存器数据
    ADXL345_Stop();                   //发送停止信号
}

 /**
  * @brief  单字节读取
  * @param  REG_Address：读取地址
  * @retval 无
  */
uchar Single_Read_ADXL345(uchar REG_Address)
{  
	uchar REG_data;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(REG_Address);            //发送存储单元地址，从0开始	
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=ADXL345_RecvByte();              //读出寄存器数据
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //停止信号
    return REG_data; 
}

 /**
  * @brief  连续读出ADXL345内部加速度数据，地址范围0x32~0x37
  * @param  无
  * @retval 无
  */
void Multiple_Read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress);           //发送设备地址+写信号
    ADXL345_SendByte(0x32);                   //发送存储单元地址，从0x32开始	
    ADXL345_Start();                          //起始信号
    ADXL345_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
	{
        BUF[i] = ADXL345_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 5)
        {
           ADXL345_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //回应ACK
       }
	}
    ADXL345_Stop();                          //停止信号
    delay_us(5);
}

 /**
  * @brief  初始化ADXL345
  * @param  无
  * @retval 无
  */
void Init_ADXL345(void)
{                       
	Single_Write_ADXL345(0x31,0x0B);   //测量范围,正负16g，13位模式
	Single_Write_ADXL345(0x2C,0x08);   //速率设定为25
	Single_Write_ADXL345(0x2D,0x08);   //选择电源模式
	Single_Write_ADXL345(0x2E,0x80);   //使能 DATA_READY 中断
	Single_Write_ADXL345(0x1E,0x00);   //X 偏移量
	Single_Write_ADXL345(0x1F,0x00);   //Y 偏移量
	Single_Write_ADXL345(0x20,0x05);   //Z 偏移量
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
  * @brief  加速度xyz数据读取
  * @param  无
  * @retval 无
  */
void ReadData(void)
{   
	int  dis_data;                       //变量
	Multiple_Read_ADXL345();       	//连续读出数据，存储在BUF中
	dis_data=(BUF[1]<<8)+BUF[0];  //合成数据   
	
	temp_X=(double)dis_data*3.9;  //计算数据和显示
	dis_data=(BUF[3]<<8)+BUF[2];  //合成数据   
	
	temp_Y=(double)dis_data*3.9;  //计算数据和显示
	dis_data=(BUF[5]<<8)+BUF[4];    //合成数据   
	
	temp_Z=(double)dis_data*3.9;  //计算数据和显示
	
	if(temp_X>200000.0)
			temp_X-=255586.5;
	if(temp_Y>200000.0)
			temp_Y-=255586.5;
	if(temp_Z>200000.0)
			temp_Z-=255586.5;
}


