#include "StdAfx.h"
#include "IMX_519.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214

IMX_519::IMX_519(void)
{

}


IMX_519::~IMX_519(void)
{
}

int IMX_519:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_519::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int IMX_519::GetTemperature(USHORT &temperature)
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

BOOL IMX_519::IMX519_ReadOTPPage(int page, USHORT *ReadData)
{
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

int IMX_519::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tmpBuff[64] = {0};
	memset(tmpBuff, 0, sizeof(USHORT) * 64);
	if(IMX519_ReadOTPPage(0x27, tmpBuff) != 1) return 0;

	CString strTmp = _T("");
	for (int i = 28; i < 39; i++)
	{
		strTmp.Format(_T("%.2X"), tmpBuff[i]);
		FuseID += strTmp;
	}

	return 1;
}

int IMX_519::Init()
{
	return 1;
}

int IMX_519::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{	
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	USHORT r_ratio, b_ratio;

	r_ratio = 1024 * (Typical_rg) /(rg);
	b_ratio = 1024 * (Typical_bg) /(bg);


	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;


	if(r_ratio >= 1024 )
	{
		if(b_ratio>=1024) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT * r_ratio / 1024);
			G_GAIN = GAIN_DEFAULT;	
			B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / 1024);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT * 1024 / b_ratio);
			B_GAIN = GAIN_DEFAULT;	
		}
	}
	else 			
	{
		if(b_ratio >= 1024)
		{
			R_GAIN = GAIN_DEFAULT;	
			G_GAIN =(USHORT)(GAIN_DEFAULT * 1024 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT * 1024 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT * 1024 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT;
				G_GAIN = (USHORT)(GAIN_DEFAULT * 1024 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT * 1024 / b_ratio );
				B_GAIN = GAIN_DEFAULT;
			}
		}	
	}
	if(I2cWrite(0x3ff9,0x00,3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR, R_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR+1, R_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_BLUE_ADDR, B_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_BLUE_ADDR+1, B_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_GREEN1_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN1_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_GREEN2_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN2_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;
	return 1;

}

int IMX_519::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_519::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_519::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_519::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202, &ExpHigh, 3) != 1) return 0;
	if(I2cRead(0x0203, &ExpLow, 3) != 1) return 0;

	exp = (ExpHigh << 8) + (ExpLow & 0xFF);

	return 1;
} 

int IMX_519::WriteExp( int exp)
{
	USHORT ExpHigh = exp >> 8;
	USHORT ExpLow = exp & 0xFF;
	if(I2cWrite(0x0202, ExpHigh, 3) != 1) return 0;
	if(I2cWrite(0x0203, ExpLow, 3) != 1) return 0;

	return 1;
} 

int IMX_519::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204, &GainHigh, 3) != 1) return 0;
	if(I2cRead(0x0205, &GainLow, 3) != 1) return 0;
	gain = (GainHigh << 8) + (GainLow & 0xFF);

	return 1;
} 

int IMX_519::WriteGain( USHORT gain)
{
	USHORT GainHigh = gain >> 8;
	USHORT GainLow = gain & 0xFF;
	if(I2cWrite(0x0204, GainHigh, 3) != 1) return 0;
	if(I2cWrite(0x0205, GainLow, 3) != 1) return 0;

	return 1;
} 