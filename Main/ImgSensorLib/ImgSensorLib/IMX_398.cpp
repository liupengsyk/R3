#include "StdAfx.h"
#include "IMX_398.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214
IMX_398::IMX_398(void)
{
}


IMX_398::~IMX_398(void)
{
}
int IMX_398:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_398::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
BOOL IMX_398::IMX398_ReadOTPPage(int page, USHORT *ReadData)
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
int IMX_398::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_398::GetFuseID(CString &FuseID) 
{
	FuseID = L"";
	//Step 1: Set the NVM page number 0~17
	if(I2cWrite(0x0A02,0x1F,3) != 1) return 0;
	//Step 2: Set up for NVM read transfer mode.
	if(I2cWrite(0x0A00,1,3) != 1) return 0;
	//step 3: Read NVM status register. This is not mandatory.
	USHORT NVM_status;
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
	CString mystr;
	USHORT Rgtemp;
	for (int i=0x0A27;i<=0x0A2E;i++)
	{
		if(I2cRead(i,&Rgtemp,3) != 1) return 0;
		mystr.Format(_T("%02x"),Rgtemp);
		FuseID = FuseID+mystr;
	}
	return 1;
}
int IMX_398::Init()
{
	return 1;
}
int IMX_398::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
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
int IMX_398::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_398::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_398::ApplySpc(short *pSPC)
{
	return 1;
}


int IMX_398::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_398::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_398::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_398::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 
