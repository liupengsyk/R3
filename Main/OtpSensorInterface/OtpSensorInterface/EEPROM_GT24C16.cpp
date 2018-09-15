#include "StdAfx.h"
#include "EEPROM_GT24C16.h"


EEPROM_GT24C16::EEPROM_GT24C16(void)
{

} 

EEPROM_GT24C16::~EEPROM_GT24C16(void)
{
}

int EEPROM_GT24C16::Init()
{	
	return 0;
}

int EEPROM_GT24C16::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{ 
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;  
	for (i =startAddr;i<=endAddr;i++)
	{
		if(I2cRead( i,&value_,0) != 1) return 0;
		if (SendWaitAck() != 1) return 0;

		buf[i-startAddr] = value_;
	}

	return 1;
}

int EEPROM_GT24C16::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0; 
	BYTE value_ = 0;
	for (i =startAddr;i<=endAddr;i++)
	{ 
		value_ = buf[i-startAddr] ; 
		if(I2cWrite(i,value_,0) != 1) return 0; 
		if (SendWaitAck() != 1) return 0;
	}
	return 1;
}

int EEPROM_GT24C16::GT24C16_WriteByte(USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	if ( ((address%16)!=0) && ( length > 16 ) )
	{
		int modbyte = 16 - address%16;
		for (int k=0; k<modbyte; k++)
		{
			if(I2cWrite(  address+k, (USHORT)value[k], 3) != 1) return 0;	
			if (SendWaitAck() != 1) return 0;
		}

		length = length - modbyte;
		if ( length > 16 )
		{
			int group = length/16;
			int num = length%16;
			int i = 0;
			for (i = 0; i < group; i++)
			{
				for (int k=0; k<16; k++)
				{
					if(I2cWrite(address+modbyte+i*16+k, (USHORT)value[modbyte+i*16+k], 3) != 1) return 0;
					if (SendWaitAck() != 1) return 0;
				}
			}
			for (int k=0; k<16; k++)
			{
				if(I2cWrite(address+modbyte+i*16+k, (USHORT)value[modbyte+i*16+k], 3) != 1) return 0;
				if (SendWaitAck() != 1) return 0;
			}
		}
		else
		{
			for (int k=0; k<length; k++)
			{
				if(I2cWrite(address+modbyte+k, (USHORT)value[modbyte+k], 3) != 1) return 0;
				if (SendWaitAck() != 1) return 0;
			}
		}
	}
	else
	{
		if ( length > 16 )
		{
			int group = length/16;
			int num = length%16;
			int i = 0;
			for (i = 0; i < group; i++)
			{
				for (int k=0; k<16; k++)
				{
					if(I2cWrite(address+i*16+k, (USHORT)value[i*16+k], 3) != 1) return 0;
					if (SendWaitAck() != 1) return 0;
				}
			}
			for (int k=0; k<num; k++)
			{
				if(I2cWrite(address+i*16+k, (USHORT)value[i*16+k], 3) != 1) return 0;
				if (SendWaitAck() != 1) return 0;
			}
		}
		else
		{
			for (int k=0; k<length; k++)
			{
				if(I2cWrite(address+k, (USHORT)value[k], 3) != 1) return 0;
				if (SendWaitAck() != 1) return 0;
			}			
		}
	}
	return 1;
}

BYTE EEPROM_GT24C16::GT24C16_ReadByte( USHORT address, BYTE value)
{ 
	I2cRead(  address, (USHORT *)&value, 3); 
	return value; 
}
