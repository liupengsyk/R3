 
#include "StdAfx.h"
#include "IMX_386.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214

IMX_386::IMX_386(void)
{

}


IMX_386::~IMX_386(void)
{
}
int IMX_386:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_386::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
BOOL IMX_386::ReadOTPPage(int page, USHORT *ReadData)
{
	
	return 1;

}
int IMX_386::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_386::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT temp1  = 0;
	int i=0;
	char section[256];
	
	USHORT tempVal[11];
	if(I2cWrite(0x0a02,0x13,3) != 1) return 0;
	if(I2cWrite(0x0a00,0x01,3) != 1) return 0;
	USHORT CheckStatus;
	if(I2cRead(0x0a01,&CheckStatus,3) != 1) return 0;
	if (CheckStatus==1)
	{
		for (int i=0x0a20; i<=0x0a2a; i++)
		{
			if(I2cRead(i,&tempVal[i-0x0a20],3) != 1) return 0;
		}
	}
	if(I2cWrite( 0x0a00,0x00,3) != 1) return 0;

	for ( int j=0; j<11; j++)
	{ 
		sprintf(section, "%02X",tempVal[j]);
		CString str(section);
		FuseID += str; 
	} 
	FuseID.MakeUpper(); 

	return 1;
}
int IMX_386::Init()
{
	return 1;
}
int IMX_386::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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

	if(I2cWrite(GAIN_RED_ADDR,R_GAIN>>8,3) != 1) return 0;
	if(I2cWrite(GAIN_RED_ADDR+1,R_GAIN&0xff,3) != 1) return 0;

	if(I2cWrite(GAIN_BLUE_ADDR,B_GAIN>>8,3) != 1) return 0;
	if(I2cWrite(GAIN_BLUE_ADDR+1,B_GAIN&0xff,3) != 1) return 0;

	if(I2cWrite(GAIN_GREEN1_ADDR,G_GAIN>>8,3) != 1) return 0;
	if(I2cWrite(GAIN_GREEN1_ADDR+1,G_GAIN&0xff,3) != 1) return 0;

	if(I2cWrite(GAIN_GREEN2_ADDR,G_GAIN>>8,3) != 1) return 0;
	if(I2cWrite(GAIN_GREEN2_ADDR+1,G_GAIN&0xff,3) != 1) return 0;

	return 1;
}
int IMX_386::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_386::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_386::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_386::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_386::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_386::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_386::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 