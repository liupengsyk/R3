#include "StdAfx.h"
#include "S5K5E2.h"

#define gain_default	   0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214
S5K5E2::S5K5E2(void)
{
}


S5K5E2::~S5K5E2(void)
{
}
int S5K5E2:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E2::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int S5K5E2::S5K5E2_ReadOTP(int group,USHORT *tempdata)
{	
	if(NULL == tempdata) return 0;

	if(I2cWrite(0x0100, 0x00, 3) != 1) return 0; 
	Sleep(10);  
	if(I2cWrite(0x0A00, 0x04, 3) != 1) return 0; 
	if(I2cWrite(0x0A02, group,3) != 1) return 0;	
	if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0; 

	Sleep(100);

	for (int i = 0; i < 64; i++ )
	{
		if(I2cRead(0x0A04 + i, tempdata+i,  3) != 1) return 0;
	}

	if(I2cWrite(0x0A00, 0x04, 3) != 1) return 0;    
	if(I2cWrite(0x0A00, 0x00, 3) != 1) return 0;    
	Sleep(10);	  
	if(I2cWrite(0x0100, 0x01, 3) != 1) return 0;	  
	return 1;
}
int S5K5E2::GetTemperature(USHORT &temperature)
{
	return 1;
}
int S5K5E2::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	USHORT tempVal[64];
	if(S5K5E2_ReadOTP(0,tempVal) != 1) return 0;
	char section[256];
	for ( int j=0; j<8; j++)
	{ 
		sprintf(section, "%02X",tempVal[j]);
		FuseID += section;
	}
	return 1;
}
int S5K5E2::Init()
{
	return 1;
}
int S5K5E2::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;
	USHORT R_gain;
	USHORT B_gain;
	USHORT G_gain;  

	float R_ration=Typical_rg*1.0/rg;
	float B_ration=Typical_bg*1./bg;

	if (R_ration>=1)
	{
		if (B_ration>=1)
		{
			G_gain=gain_default ;
			R_gain=gain_default*R_ration;
			B_gain=gain_default*B_ration;
		}
		else
		{
			B_gain=gain_default;
			G_gain=gain_default/B_ration;
			R_gain=gain_default*R_ration/B_ration;
		}
	}
	else
	{
		if (B_ration>=1)
		{
			R_gain=gain_default;
			G_gain=gain_default/R_ration;
			B_gain=gain_default*B_ration/R_ration;
		}
		else
		{
			if( (R_ration >= B_ration) && (B_ration < 1))
			{
				B_gain=gain_default;
				G_gain=gain_default/B_ration;
				R_gain=gain_default*R_ration/B_ration;
			}
			else
			{
				R_gain=gain_default;
				G_gain=gain_default/R_ration;
				B_gain=gain_default*B_ration/R_ration;
			}
		}
	} 
	
	if(I2cWrite( GAIN_RED_ADDR, R_gain>>8,       3) != 1) return 0;  
	if(I2cWrite( GAIN_RED_ADDR+1, R_gain&0xff,   3) != 1) return 0;  

	if(I2cWrite( GAIN_BLUE_ADDR, B_gain>>8,      3) != 1) return 0;  
	if(I2cWrite( GAIN_BLUE_ADDR+1,B_gain&0xff,   3) != 1) return 0;  
 	
	if(I2cWrite( GAIN_GREEN1_ADDR, G_gain>>8,    3) != 1) return 0;  
	if(I2cWrite( GAIN_GREEN1_ADDR+1,G_gain&0xff, 3) != 1) return 0;  
  
	if(I2cWrite( GAIN_GREEN2_ADDR, G_gain>>8,    3) != 1) return 0;  
	if(I2cWrite( GAIN_GREEN2_ADDR+1,G_gain&0xff, 3) != 1) return 0;  

	return 1;
}
int S5K5E2::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int S5K5E2::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int S5K5E2::ApplySpc(short *pSPC)
{
	return 1;
}
 
 
int S5K5E2::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3)  != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int S5K5E2::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3)  != 1) return 0;

	return 1;
} 

int S5K5E2::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3)  != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int S5K5E2::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3)  != 1) return 0;

	return 1;
} 