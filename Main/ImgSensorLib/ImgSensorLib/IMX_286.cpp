#include "StdAfx.h"
#include "IMX_286.h"


IMX_286::IMX_286(void)
{

}


IMX_286::~IMX_286(void)
{
}

int IMX_286:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_286::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL IMX_286::IMX214_ReadOTPPage(int page, USHORT *ReadData)
{
	if(NULL == ReadData) return 0;

	if(I2cWrite(0x0a02, page, 3) != 1) return 0;
	if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;
	USHORT flag = 0;
	int cnt = 0;
	do 
	{
		if(I2cRead(0x0a01, &flag, 3) != 1) return 0;
		cnt++;
	} while ( ( flag != 0x01 ) && ( cnt < 100 ) );
	if ( cnt >=100 )
	{
		return 0;
	}
	for ( int i = 0; i < 64; i++ )
	{
		if(I2cRead(0x0a04+i, ReadData+i, 3) != 1) return 0;
	}
	return 1;

}
int IMX_286::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_286::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1[64] = {0};
	int i=0;
	char section[256];

	if(I2cWrite(0x0A02,0x13,3) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x0A00,0x01,3) != 1) return 0;
	Sleep(10);
	USHORT CheckStatus;
	if(I2cRead(0x0A01,&CheckStatus,3) != 1) return 0;
	if (CheckStatus == 1)
	{
		for (int i=0x0a20; i<=0x0a2a; i++)
		{
			if(I2cRead(i,&temp1[i-0x0a20],3) != 1) return 0;
		}
	}
	if(I2cWrite(0x0A00,0x00,3) != 1) return 0;
	Sleep(10);
	for ( i=0; i<11; i++)
	{ 
		sprintf( section,"%02X",temp1[i]);
		FuseID += section;
	}
	FuseID.MakeUpper();
	return 1;
}
int IMX_286::Init()
{
	return 1;
}
int IMX_286::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int IMX_286::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_286::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_286::ApplySpc(short *pSPC)
{
	return 1;
}


int IMX_286::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_286::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_286::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_286::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 