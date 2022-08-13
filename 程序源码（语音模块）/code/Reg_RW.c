/**
  ******************************************************************************
  * @file    Reg_RW.c
  * @author  chen
  * @version V1.0
  * @date    2022-07-11
  * @brief   读取写入文件
  ******************************************************************************
  */
  
#include "config.h"

#define LD_INDEX_PORT		(*((volatile unsigned char xdata*)(0x8100)))
#define LD_DATA_PORT		(*((volatile unsigned char xdata*)(0x8000)))

void LD_WriteReg( unsigned char address, unsigned char dataout )
{
  LD_INDEX_PORT  = address;
  LD_DATA_PORT = dataout;
}

unsigned char LD_ReadReg( unsigned char address )
{
  LD_INDEX_PORT = address;
  return (unsigned char)LD_DATA_PORT;
}

/*********************************************END OF FILE**********************/
