 
#include "StdAfx.h"
#include "IMX_230.h"


IMX_230::IMX_230(void)
{

}
int IMX_230:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_230::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

IMX_230::~IMX_230(void)
{
}
 
int IMX_230::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_230::GetFuseID(CString &FuseID) 
{ 
	FuseID = _T("");
	USHORT NVM_status = 0;
	for(USHORT i=0;i<=10;i++)
	{
		if(I2cRead(0x0A01,&NVM_status,3) != 1) return 0;
		if (NVM_status==5)
		{
			 return 0;
		}
		if (NVM_status==1)//读取成功
		{
			break;
		}
		if (NVM_status==0)//正在读取，缓冲10ms以提供读取时间
		{
			Sleep(10);
			continue;
		}
	}
	//Read Data0-Data63 in the page N, as required.
	//Sleep(4);
    CString section = L"";
	FuseID ="";
	USHORT temp1 = 0;
	for (USHORT i=0x0A36;i<=0X0A3D;i++)
	{
		if(I2cRead(i,&temp1,3) != 1) return 0;
		section.Format(L"%02x",temp1);
		FuseID = FuseID+section;
	}
	 
	return 1;
}
int IMX_230::Init()
{
	return 1;
}
int IMX_230::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int IMX_230::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_230::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_230::ApplySpc(short *pSPC)
{
	return 1;
}


int IMX_230::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_230::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_230::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_230::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 