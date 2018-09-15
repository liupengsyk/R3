#include "StdAfx.h"
#include "IMX_319.h"


IMX_319::IMX_319(void)
{

}


IMX_319::~IMX_319(void)
{
}

int IMX_319:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_319::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int IMX_319::GetTemperature(USHORT &temperature)
{
	unsigned short initData = 0x00;

	if(I2cWrite(0x0100, initData, 3) != 1) return 0;
	initData = 0x01;
	if(I2cWrite(0x0138, initData, 3) != 1) return 0;
	Sleep(2000);
	unsigned short tmpTep = 0;
	if(I2cRead(0x013A, &tmpTep, 3) != 1) return 0;
	if(I2cWrite(0x0100, initData, 3) != 1) return 0;

	temperature = tmpTep;

	return 1;
}

BOOL IMX_319::IMX319_ReadOTPPage(int page, USHORT *ReadData)
{
	if(NULL == ReadData) return 0;

	if(I2cWrite(0x0A02, page, 3) != 1) return 0;
	if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0;
	USHORT flag = 0;
	int cnt = 0;
	do
	{
		if(I2cRead(0x0A01, &flag, 3) != 1) return 0;
		cnt++;
		Sleep(100);
	}while((flag != 0x01) && (cnt < 100));
	
	if (cnt >= 100)
		return FALSE;
	else
	{
		for ( int i = 0; i < 64; i++ )
			if(I2cRead(0x0A04 + i, ReadData + i, 3) != 1) return 0;
	}

	return TRUE;
}

int IMX_319::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tmpBuff[64] = {0};
	memset(tmpBuff, 0, sizeof(USHORT) * 64);
	if(IMX319_ReadOTPPage(0x1D, tmpBuff) != 1) return 0;

	CString strTmp = _T("");
	for (int i = 28; i < 39; i++)
	{
		strTmp.Format(_T("%.2X"), tmpBuff[i]);
		FuseID += strTmp;
	}

	return 1;
}

int IMX_319::Init()
{
	return 1;
}

int IMX_319::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}

int IMX_319::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_319::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_319::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_319::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202, &ExpHigh, 3) != 1) return 0;
	if(I2cRead(0x0203, &ExpLow, 3) != 1) return 0;

	exp = (ExpHigh << 8) + (ExpLow & 0xFF);

	return 1;
} 

int IMX_319::WriteExp( int exp)
{
	USHORT ExpHigh = exp >> 8;
	USHORT ExpLow = exp & 0xFF;
	if(I2cWrite(0x0202, ExpHigh, 3) != 1) return 0;
	if(I2cWrite(0x0203, ExpLow, 3) != 1) return 0;

	return 1;
} 

int IMX_319::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204, &GainHigh, 3) != 1) return 0;
	if(I2cRead(0x0205, &GainLow, 3) != 1) return 0;
	gain = (GainHigh << 8) + (GainLow & 0xFF);

	return 1;
} 

int IMX_319::WriteGain( USHORT gain)
{
	USHORT GainHigh = gain >> 8;
	USHORT GainLow = gain & 0xFF;
	if(I2cWrite(0x0204, GainHigh, 3) != 1) return 0;
	if(I2cWrite(0x0205, GainLow, 3) != 1) return 0;

	return 1;
} 