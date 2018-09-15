 
#include "StdAfx.h"
#include "IMX_363.h"


IMX_363::IMX_363(void)
{

}


IMX_363::~IMX_363(void)
{
}

int IMX_363:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_363::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL IMX_363::ReadOTPPage(int page, USHORT *ReadData)
{
	
	return 1;

}
int IMX_363::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_363::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	int i = 0;
	int j = 0;
	char section[256] = {0};

	USHORT tempVal[11] = {0};
	if(I2cWrite(0x0A02,0x0B,3) != 1) return 0;
	if(I2cWrite(0x0A00,0x01,3) != 1) return 0;
	USHORT CheckStatus;
	if(I2cRead(0x0A01,&CheckStatus,3) != 1) return 0;
	if (CheckStatus==1)
	{
		for (int i=0x0a21; i<=0x0a29; i++)
		{
			if(I2cRead(i,&tempVal[i-0x0a21],3) != 1) return 0;
		}
	}
	if(I2cWrite(0x0a00,0x00,3) != 1) return 0;

	for ( int j=0; j<9; j++)
	{ 
		sprintf(section, "%02X",tempVal[j]);
		CString str(section);
		FuseID += str; 
	} 
	FuseID.MakeUpper(); 

	return 1;
}
int IMX_363::Init()
{
	return 1;
}
int IMX_363::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int IMX_363::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_363::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_363::ApplySpc(short *pSPC)
{
	if(NULL == pSPC) return 0;

	if(I2cWrite(0x0100, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x0101, 0x00, 3) != 1) return 0; //V and H are both set by this. 	
	for (int i=0; i<48; i++)
	{
		if(I2cWrite(0x7d4c+i, pSPC[i], 3) != 1) return 0;
		if(I2cWrite(0x7d80+i, pSPC[i+48], 3) != 1) return 0;
	}

	if(I2cWrite(0x0101, 0x00, 3) != 1) return 0;  //setting 2
	if(I2cWrite(0x0b00, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x3051, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x3052, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x3055, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x3036, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x3047, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x3049, 0x01, 3) != 1) return 0;

	if(I2cWrite(0x0100, 0x01, 3) != 1) return 0;

	Sleep(500);

	return TRUE;
}

int IMX_363::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_363::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_363::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_363::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 