#include "StdAfx.h"
#include "IMX_355.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214
IMX_355::IMX_355(void)
{
}

IMX_355::~IMX_355(void)
{
}

int IMX_355:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int IMX_355::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL IMX_355::IMX355_ReadOTPPage(int page, USHORT *ReadData)
{
	return 1;
}

int IMX_355::GetTemperature(USHORT &temperature)
{
	return 1;
}

int IMX_355::GetFuseID(CString &FuseID) 
{
	FuseID = L"";
	//Step 1: Set the NVM page number 0~17
	if(I2cWrite(0x0A02, 0x16, 3) != 1) return 0;
	//Step 2: Set up for NVM read transfer mode.
	if(I2cWrite(0x0A00, 1, 3) != 1) return 0;
	Sleep(10);
	//step 3: Read NVM status register. This is not mandatory.
	USHORT NVM_status;
	for(USHORT i=0;i<=10;i++)
	{
		I2cRead(0x0A01, &NVM_status, 3);
		if (NVM_status==5)
		{
			Addlog(_T("fuseID 读取失败！"));
			break;
		}
		if (1 == (NVM_status & 0x01))//读取成功
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
	for (int i = 0x0A08; i <= 0x0A12; i++)
	{
		I2cRead(i, &Rgtemp, 3);
		mystr.Format(_T("%02x"), Rgtemp);
		FuseID = FuseID + mystr;
	}
	return 1;
}

int IMX_355::Init()
{
	return 1;
}

int IMX_355::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	Addlog(_T("IMX355 ApplyWBGain"));
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
	if(I2cWrite(0x3070,0x00,3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR, R_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_RED_ADDR+1, R_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_BLUE_ADDR, B_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_BLUE_ADDR+1, B_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_GREEN1_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN1_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;

	if(I2cWrite(  GAIN_GREEN2_ADDR, G_GAIN>>8, 3) != 1) return 0;
	if(I2cWrite(  GAIN_GREEN2_ADDR+1, G_GAIN&0xff, 3) != 1) return 0;
	Addlog(_T("success!"));
	return 1;
}

int IMX_355::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_355::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_355::ApplySpc(short *pSPC)
{
	return 1;
}

int IMX_355::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0202,&ExpHigh,3) != 1) return 0;
	if(I2cRead(0x0203,&ExpLow,3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_355::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	if(I2cWrite(0x0202,ExpHigh,3) != 1) return 0;
	if(I2cWrite(0x0203,ExpLow,3) != 1) return 0;

	return 1;
} 

int IMX_355::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	if(I2cRead(0x0204,&GainHigh,3) != 1) return 0;
	if(I2cRead(0x0205,&GainLow,3) != 1) return 0;
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
}

int IMX_355::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	if(I2cWrite(0x0204,GainHigh,3) != 1) return 0;
	if(I2cWrite(0x0205,GainLow,3) != 1) return 0;

	return 1;
} 