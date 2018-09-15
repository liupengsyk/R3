#include "StdAfx.h"
#include "IMX_576.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214

IMX_576::IMX_576(void)
{
}


IMX_576::~IMX_576(void)
{
}

int IMX_576:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_576::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_576::GetTemperature(USHORT &temperature)
{
	unsigned short initData = 0x00;

	if(I2cWrite(0x0100, initData, 3) != 1) return 0 ;
	initData = 0x01;
	if(I2cWrite(0x0138, initData, 3) != 1) return 0;;
	Sleep(2000);
	unsigned short tmpTep = 0;
	if(I2cRead(0x013A, &tmpTep, 3) != 1) return 0;;
	if(I2cWrite(0x0100, initData, 3) != 1) return 0;

	temperature = tmpTep;

	return 1;
}
int IMX_576::GetFuseID(CString &FuseID) 
{
	USHORT tmpBuff[64] = {0};
	memset(tmpBuff, 0, sizeof(USHORT) * 64);
	if(IMX_576ReadOTPPage(0x3F, tmpBuff) != 1) return 0;

	CString strTmp = _T("");
	for (int i = (0x0A20 - 0x0A04); i <= (0x0A2A - 0x0A04); i++)
	{
		strTmp.Format(_T("%.2X"), tmpBuff[i]);
		FuseID += strTmp;
	}

	return 1;
}
int IMX_576::Init()
{
	return 1;
}

int IMX_576::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_576::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int IMX_576::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_576::ReadExp(int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202, &ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203, &ExpLow, 3) != 1) return 0;

	exp = (ExpHigh << 8) + (ExpLow & 0xFF);

	return 1;
}

int IMX_576::WriteExp(int exp)
{
	USHORT ExpHigh = exp >> 8;
	USHORT ExpLow = exp & 0xFF;
	if(I2cWrite(0x0202, ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203, ExpLow, 3) != 1) return 0;

	return 1;
}

int IMX_576::ReadGain(USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204, &GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205, &GainLow, 3) != 1) return 0;
	gain = (GainHigh << 8) + (GainLow & 0xFF);

	return 1;
}

int IMX_576::WriteGain(USHORT gain)
{
	USHORT GainHigh = gain >> 8;
	USHORT GainLow = gain & 0xFF;
	if(I2cWrite(0x0204, GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205, GainLow, 3) != 1) return 0;

	return 1;
}

BOOL IMX_576::IMX_576ReadOTPPage(int page, USHORT *ReadData)
{
	if(I2cWrite(0x0A02, page, 3) != 1) return 0;
	if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0;
	USHORT flag = 0;
	int cnt = 0;
	do
	{
		I2cRead(0x0A01, &flag, 3);
		cnt++;
		Sleep(100);
	}while((flag != 0x01) && (cnt < 10));

	if (cnt >= 10)
		return FALSE;
	else
	{
		for ( int i = 0; i < 64; i++ )
			I2cRead(0x0A04 + i, ReadData + i, 3);
	}

	return TRUE;
}

int IMX_576::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;

	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (Typical_rg) /(rg);
	b_ratio = 512 * (Typical_bg) /(bg);

	USHORT R_GAIN;
	USHORT B_GAIN;
	USHORT Gr_GAIN;
	USHORT Gb_GAIN;
	USHORT G_GAIN;

	if(r_ratio >= 512 )
	{
		if(b_ratio>=512) 
		{
			R_GAIN = (USHORT)(GAIN_DEFAULT * r_ratio / 512);
			G_GAIN = GAIN_DEFAULT;	
			B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / 512);
		}
		else
		{
			R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio);
			G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio);
			B_GAIN = GAIN_DEFAULT;	
		}
	}
	else 			
	{
		if(b_ratio >= 512)
		{
			R_GAIN = GAIN_DEFAULT;	
			G_GAIN =(USHORT)(GAIN_DEFAULT * 512 / r_ratio);
			B_GAIN =(USHORT)(GAIN_DEFAULT *  b_ratio / r_ratio);

		} 
		else 
		{
			Gr_GAIN = (USHORT)(GAIN_DEFAULT * 512 / r_ratio );
			Gb_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio );

			if(Gr_GAIN >= Gb_GAIN)
			{
				R_GAIN = GAIN_DEFAULT;
				G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / r_ratio );
				B_GAIN = (USHORT)(GAIN_DEFAULT * b_ratio / r_ratio);
			} 
			else
			{
				R_GAIN =  (USHORT)(GAIN_DEFAULT * r_ratio / b_ratio );
				G_GAIN = (USHORT)(GAIN_DEFAULT * 512 / b_ratio );
				B_GAIN = GAIN_DEFAULT;
			}
		}	
	}
	
	if(I2cWrite(  GAIN_RED_ADDR, R_GAIN>>8,    3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR+1, R_GAIN&0xff,3) != 1) return 0;

	if(I2cWrite(  GAIN_BLUE_ADDR, B_GAIN>>8,   3) != 1) return 0;
	if(I2cWrite(  GAIN_BLUE_ADDR+1, B_GAIN&0xff,3)!= 1) return 0;

	if(I2cWrite(  GAIN_GREEN1_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN1_ADDR+1, G_GAIN&0xff, 3)!= 1) return 0;

	if(I2cWrite(  GAIN_GREEN2_ADDR, G_GAIN>>8, 3) != 1) return 0;;
	if(I2cWrite(  GAIN_GREEN2_ADDR+1, G_GAIN&0xff, 3)!= 1) return 0;
	return 1;
}