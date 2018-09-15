#include "StdAfx.h"
#include "IMX_476.h"

#define GAIN_DEFAULT       0x0100
#define GAIN_GREEN1_ADDR   0x020E
#define GAIN_BLUE_ADDR     0x0212
#define GAIN_RED_ADDR      0x0210
#define GAIN_GREEN2_ADDR   0x0214
IMX_476::IMX_476(void)
{

}


IMX_476::~IMX_476(void)
{
}

int IMX_476:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int IMX_476::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL IMX_476::ReadOTPPage(int page, USHORT *ReadData)
{

	return 1;

}
int IMX_476::GetTemperature(USHORT &temperature)
{
	return 1;
}
int IMX_476::GetFuseID(CString &FuseID) 
{
	int i = 0;
	int j = 0;
	char section[256] = {0};

	USHORT tempVal[11] = {0};
	I2cWrite(0x0A02,0x3F,3);
	I2cWrite(0x0A00,0x01,3);
	USHORT CheckStatus;
	I2cRead(0x0A01,&CheckStatus,3);
	if (CheckStatus==1)
	{
		for (int i=0x0a20; i<=0x0a2a; i++)
		{
			I2cRead(i,&tempVal[i-0x0a20],3);
		}
	}
	I2cWrite(0x0a00,0x00,3);

	for ( int j=0; j<=10; j++)
	{ 
		sprintf(section, "%02X",tempVal[j]);
		CString str(section);
		FuseID += str; 
	} 
	FuseID.MakeUpper(); 

	return 1;
}
int IMX_476::Init()
{
	return 1;
}
int IMX_476::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	USHORT r_ratio, b_ratio;

	r_ratio = 512 * (Typical_rg) /(rg);
	b_ratio = 512 * (Typical_bg) /(bg);

	if( !r_ratio || !b_ratio)
	{
		//AddString("ÇëÏÈGet AWB!",0);
		//CString temp;
		//temp.Format(_T("ÇëÏÈGet AWB!"));
		//m_pInterface->Ctrl_Addlog(CamID,temp,COLOR_BLUE,200);

		return FALSE;
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
	// 	USHORT tempVal[11];
	// 	I2cWrite(0x0a02,39,3);
	// 	I2cWrite(0x0a00,0x01,3);
	// 	USHORT CheckStatus;
	// 	I2cRead(0x0a01,&CheckStatus,3);


	// 	WriteSensorReg(CurrentSensor.SlaveID, 0x3ff9, 0x00, CurrentSensor.mode);
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_RED_ADDR, R_GAIN>>8, CurrentSensor.mode);
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_RED_ADDR+1, R_GAIN&0xff, CurrentSensor.mode);

	I2cWrite(0x3130,0x01,3);
	I2cWrite(GAIN_RED_ADDR,R_GAIN>>8,3);
	I2cWrite(GAIN_RED_ADDR+1,R_GAIN&0xff,3);


	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_BLUE_ADDR, B_GAIN>>8, CurrentSensor.mode);
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_BLUE_ADDR+1, B_GAIN&0xff, CurrentSensor.mode);

	I2cWrite(GAIN_BLUE_ADDR,B_GAIN>>8,3);
	I2cWrite(GAIN_BLUE_ADDR+1,B_GAIN&0xff,3);

	//	S5K3H2YXMIPI_wordwrite_cmos_sensor(GAIN_GREEN1_ADDR, G_GAIN); //Green 1 default gain 1x
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_GREEN1_ADDR, G_GAIN>>8, CurrentSensor.mode);
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_GREEN1_ADDR+1, G_GAIN&0xff, CurrentSensor.mode);

	I2cWrite(GAIN_GREEN1_ADDR,G_GAIN>>8,3);
	I2cWrite(GAIN_GREEN1_ADDR+1,G_GAIN&0xff,3);

	//	S5K3H2YXMIPI_wordwrite_cmos_sensor(GAIN_GREEN2_ADDR, G_GAIN); //Green 2 default gain 1x
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_GREEN2_ADDR, G_GAIN>>8, CurrentSensor.mode);
	// 	WriteSensorReg(CurrentSensor.SlaveID, GAIN_GREEN2_ADDR+1, G_GAIN&0xff, CurrentSensor.mode);

	I2cWrite(GAIN_GREEN2_ADDR,G_GAIN>>8,3);
	I2cWrite(GAIN_GREEN2_ADDR+1,G_GAIN&0xff,3);

	return 1;
}
int IMX_476::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int IMX_476::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int IMX_476::ApplySpc(short *pSPC)
{

	return TRUE;
}

int IMX_476::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	I2cRead(0x0202,&ExpHigh,3);
	I2cRead(0x0203,&ExpLow,3);
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int IMX_476::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	I2cWrite(0x0202,ExpHigh,3);
	I2cWrite(0x0203,ExpLow,3);

	return 1;
} 

int IMX_476::ReadGain( USHORT &gain)
{
	USHORT GainHigh;
	USHORT GainLow;
	I2cRead(0x0204,&GainHigh,3);
	I2cRead(0x0205,&GainLow,3);
	gain=(GainHigh<<8)+(GainLow&0xFF);

	return 1;
} 
int IMX_476::WriteGain( USHORT gain)
{
	USHORT GainHigh=gain>>8;
	USHORT GainLow=gain&0xFF;
	I2cWrite(0x0204,GainHigh,3);
	I2cWrite(0x0205,GainLow,3);

	return 1;
} 