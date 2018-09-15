#include "StdAfx.h"
#include "SP2509.h"


SP2509::SP2509(void)
{
	flag=0;
}

SP2509::~SP2509(void)
{
}
int SP2509:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int SP2509::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int SP2509::GetTemperature(USHORT &temperature)
{
	return 1;
}

int SP2509::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	return 1;
}
int SP2509::Init()
{
	return 1;
}

int SP2509::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}


int SP2509::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	
	return 1;
}

int SP2509::GetLsc(BYTE* P10Buff,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	
	return 1;
}

int SP2509::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int SP2509::ApplySpc(short *pSPC)
{
	return 1;
}


int SP2509::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x3,&ExpHigh,0) != 1) return 0;
	if(I2cRead(0x4,&ExpLow,0) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int SP2509::WriteExp( int exp)
{
	
	USHORT ExpHigh=(exp>>8)&0xFF;
	USHORT ExpLow=exp&0xFF;


	if(I2cWrite(0xfd, 0x1,0) != 1) return 0;
	if(I2cWrite(0x3, ExpHigh, 0) != 1) return 0;
	if(I2cWrite(0x4,ExpLow, 0) != 1) return 0;
	if(I2cWrite(0x1, 0x1, 0) != 1) return 0;

	return 1;
} 

int SP2509::ReadGain( USHORT &gain)
{
	USHORT GainLow;
	if(I2cRead(0x24,&GainLow,0) != 1) return 0;

	gain=GainLow;

	return 1;
} 
int SP2509::WriteGain( USHORT gain)
{

	USHORT GainLow=gain&0xFF;

	if(I2cWrite(0xfd, 0x1,0) != 1) return 0;
	if(I2cWrite(0x24, GainLow, 0) != 1) return 0;
	if(I2cWrite(0x1, 0x1, 0) != 1) return 0;
	
	
	return 1;
} 
void SP2509::WriteSensorSetting()
{
}
