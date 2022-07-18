/*******************************************************
**	CPU: STC11L08XE
**	¾§Õñ£º22.1184MHZ
**	²¨ÌØÂÊ£º9600 bit/S
/*********************************************************/
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

