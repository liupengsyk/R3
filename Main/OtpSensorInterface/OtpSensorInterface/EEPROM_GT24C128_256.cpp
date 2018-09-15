#include "StdAfx.h"
#include "EEPROM_GT24C128_256.h"


EEPROM_GT24C128_256::EEPROM_GT24C128_256(void)
{
}


EEPROM_GT24C128_256::~EEPROM_GT24C128_256(void)
{
}

//thanks for John
bool fSleep(unsigned short uiMillsecond)

{
	DWORD uiStartTick =   GetTickCount();
	while(true)	
	{
		DWORD uiNowTick =   GetTickCount();	
		if ((uiNowTick-uiStartTick) >= uiMillsecond)	
		{
			break;		
		}
		MSG msg;	
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	
		{
			TranslateMessage(&msg);		
			DispatchMessage(&msg);		
		}	
	}	
	return true;	
}

int EEPROM_GT24C128_256::Init()
{
	return 0;
}

int EEPROM_GT24C128_256::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page /* = 0*/ )
{
	//USHORT usYear = 0;
	USHORT i = 0,value_ = 0;
	int flg = 0;

	int len = endAddr - startAddr +1;
	BYTE* data =  new BYTE[len];
	if(NULL == data) return 0;

	flg = GT24C128_256_ReadByte(startAddr,data,len);
	for(int i=0;i< len;i++)
	{
		buf[i] =  data[i];
	}
	SAFE_DELETE_ARRAY(data);
	return flg;

}

int EEPROM_GT24C128_256::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT /*page  = 0*/ )
{
	if(NULL == buf || endAddr < startAddr) return 0;

	USHORT i = 0,value_ = 0;
	int  flg = 0;
	int len = endAddr - startAddr +1;
	BYTE* data = new BYTE[len];
	if(NULL == data) return 0;

	for(int i=0;i< len;i++)
	{
		data[i] = buf[i];
	}
	flg = GT24C128_256_WriteByte(startAddr,data,len);
	SAFE_DELETE_ARRAY(data);
	return flg;
}

int EEPROM_GT24C128_256::GT24C128_256_ReadByte(USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	USHORT PageSize=128;            //GT24C64 的PageSize为32
	USHORT group = 0;  
	int i=0;
	USHORT ByteLast =0;
	USHORT ByteFirst =0;

	if((address%PageSize) != 0)
	{
		ByteFirst  = PageSize - address%PageSize;
	}
	else
	{
		ByteFirst = 0;
	}
	if(length <= ByteFirst)
	{
		ByteFirst = length;
	}

	group = (length-ByteFirst)/PageSize;
	ByteLast = length-ByteFirst-group*PageSize;
	if(ByteFirst!=0)
	{
		if(I2cReadBlock(address,2,value,ByteFirst) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cReadBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}

	return 1;
}

int EEPROM_GT24C128_256::GT24C128_256_WriteByte(USHORT address, BYTE *value, int length)
{
	if(NULL == value) return 0;

	USHORT PageSize=64;            //GT24C64 的PageSize为32
	USHORT group = 0;  
	int i=0;
	USHORT ByteLast =0;
	USHORT ByteFirst =0;
	if((address%PageSize) != 0)
	{
		ByteFirst  = PageSize - address%PageSize;
	}
	else
	{
		ByteFirst = 0;
	}
	if(length <= ByteFirst)
	{
		ByteFirst = length;
	}

	group = (length-ByteFirst)/PageSize;
	ByteLast = length-ByteFirst-group*PageSize;
	if(ByteFirst!=0)
	{
		if(I2cWriteBlock(address,2,value,ByteFirst) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}

	for ( i = 0; i < group; i++)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,PageSize) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	if(ByteLast!=0)
	{
		if(I2cWriteBlock(address+ByteFirst+i*PageSize,2,value+ByteFirst+i*PageSize,ByteLast) != 1) return 0;
		if (SendWaitAck() != 1) return 0;
	}
	return 1;
}
