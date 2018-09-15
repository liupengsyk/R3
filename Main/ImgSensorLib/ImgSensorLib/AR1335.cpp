#include "StdAfx.h"
#include "AR1335.h"

AR1335::AR1335(void)
{

}


AR1335::~AR1335(void)
{

}

int AR1335:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int AR1335::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL AR1335::ReadOTPPage(int page, USHORT *ReadData)
{
	return 1;

}
int AR1335::GetTemperature(USHORT &temperature)
{
	return 1;
}
int AR1335::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempVal[16];
	USHORT flag = 0;
	USHORT temp_1 = 0; 

	if(I2cRead(0x301A,&temp_1,4) != 1) return 0;
	if(I2cWrite(0x301A,0x0010|temp_1,4) != 1) return 0;
	if(I2cWrite(0x304C,0x0100,4) != 1) return 0;
	if(I2cWrite(0x304A,0x0210,4) != 1) return 0;
	if(I2cRead(0x304A,&flag,4) != 1) return 0;
	
	if(I2cRead(0x380E,&tempVal[0],4) != 1) return 0;
	if(I2cRead(0x380C,&tempVal[1],4) != 1) return 0;
	if(I2cRead(0x380A,&tempVal[2],4) != 1) return 0;
	if(I2cRead(0x3808,&tempVal[3],4) != 1) return 0;	
	if(I2cRead(0x3806,&tempVal[4],4) != 1) return 0;
	if(I2cRead(0x3804,&tempVal[5],4) != 1) return 0;
	if(I2cRead(0x3802,&tempVal[6],4) != 1) return 0;
	if(I2cRead(0x3800,&tempVal[7],4) != 1) return 0;

	char section[256] = {0};
	for ( int j=0; j<8; j++)
	{ 
		sprintf(section, "%04x",tempVal[j]);
		FuseID += section;
	}
	
	FuseID.MakeUpper();

	return 1;
}
int AR1335::Init()
{
	return 1;
}
int AR1335::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int AR1335::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int AR1335::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int AR1335::ApplySpc(short *pSPC)
{
	return 1;
}

int AR1335::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;

	if(I2cRead(0x3012,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x3013,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int AR1335::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x3012,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x3013,ExpLow,3) != 1) return 0;

	return 1;
} 

int AR1335::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x305E,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x305F,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int AR1335::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x305E,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x305F,GainLow,3) != 1) return 0;

	return 1;
} 