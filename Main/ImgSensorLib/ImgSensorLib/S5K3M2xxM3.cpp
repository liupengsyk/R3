#include "StdAfx.h"
#include "S5K3M2xxM3.h"


S5K3M2xxM3::S5K3M2xxM3(void)
{
}


S5K3M2xxM3::~S5K3M2xxM3(void)
{
}

#define GAIN_DEFAULT 0x0100

int S5K3M2xxM3:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf) return 0;
	if(I2cRead(startAddr,buf,3) != 1) return 0;
	return 1;
}
int S5K3M2xxM3::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf) return 0;
	if(I2cWrite(startAddr,*buf,3) != 1) return 0;
	return 1;
}
int S5K3M2xxM3::GetTemperature(USHORT &temperature)
{
	return 1;
}
int S5K3M2xxM3::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	CString fuseid ;
	char section[512] = {0};
	char temp[512] = {0};
	USHORT tempVal[64];

	if(I2cWrite(  0x602a, 0x0a02, 4 ) != 1) return 0;    
	if(I2cWrite(  0x6f12, 0x0000, 4 ) != 1) return 0;    
	if(I2cWrite(  0x602a, 0x0a00, 4 ) != 1) return 0;    
	if(I2cWrite(  0x6f12, 0x0100, 4 ) != 1) return 0;    

	for( int j = 0; j <64 ; j++ )
	{
		if(I2cRead(  0x0a04+j, &tempVal[j], 3 ) != 1) return 0;
	}

	if(I2cWrite(  0x602a, 0x0a00, 4 ) != 1) return 0;
	if(I2cWrite(  0x6f12, 0x00, 3 ) != 1) return 0;

	for (int j=32; j<38; j++)
	{ 
		sprintf(section, "%02X",tempVal[j]);
		FuseID += section;
	}
	return 1;
}
int S5K3M2xxM3::Init()
{
	return 1;
}
int S5K3M2xxM3::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	if(rg == 0 || bg == 0 || Typical_rg == 0 || Typical_bg == 0) return 0;
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (Typical_rg) /(rg);
	b_ratio = 512 * (Typical_bg) /(bg);

	if( !r_ratio || !b_ratio)
	{
		Addlog(_T("ÇëÏÈGet AWB!\n"));
		return 0;
	}

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
	if(I2cWrite(  0x6028, 0x4000,4) != 1) return 0;                  
	if(I2cWrite(  0x602a, 0x0100,4) != 1) return 0;                  
	if(I2cWrite(  0x6f12, 0x00, 3 ) != 1) return 0;// Stream off     
	Sleep(200);               
             
	if(I2cWrite( 0x6028, 0x4000, 4) != 1) return 0;	                 
	if(I2cWrite( 0x602a, 0x3056, 4) != 1) return 0;                  
	if(I2cWrite( 0x6f12, 0x01,   3) != 1) return 0;                  
                
	if(I2cWrite( 0x602a, 0x0210, 4) != 1) return 0;                  
	if(I2cWrite( 0x6f12, R_GAIN, 4) != 1) return 0;                  
                
	if(I2cWrite( 0x602a, 0x0212, 4) != 1) return 0;                  
	if(I2cWrite( 0x6f12, B_GAIN, 4) != 1) return 0;                  
                 
	if(I2cWrite( 0x602a, 0x020e, 4) != 1) return 0;                  
	if(I2cWrite( 0x6f12, G_GAIN, 4) != 1) return 0;                  
                 
	if(I2cWrite( 0x602a, 0x0214, 4) != 1) return 0;                  
	if(I2cWrite( 0x6f12, G_GAIN, 4) != 1) return 0;                  
           
	if(I2cWrite(  0x6028, 0x4000,4) != 1) return 0;                  
	if(I2cWrite(  0x602a, 0x0100,4) != 1) return 0;                  
	if(I2cWrite(  0x6f12, 0x01,  3) != 1) return 0;// Stream on      


	return 1;
}
int S5K3M2xxM3::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int S5K3M2xxM3::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int S5K3M2xxM3::ApplySpc(short *pSPC)
{
	return 1;
}

int S5K3M2xxM3::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);
	return 1;
} 
int S5K3M2xxM3::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;
	return 1;
} 

int S5K3M2xxM3::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);
	return 1;
} 
int S5K3M2xxM3::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;
	return 1;
} 