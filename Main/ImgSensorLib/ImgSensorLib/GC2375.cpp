#include "StdAfx.h"
#include "GC2375.h"


GC2375::GC2375(void)
{
}


GC2375::~GC2375(void)
{
}

int GC2375:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int GC2375::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

int GC2375::GetTemperature(USHORT &temperature)
{
	return 1;
}
int GC2375::GetFuseID(CString &FuseID) 
{
	//return 1;
	FuseID = _T("");
	int i=0;
	int j=0;
	char section[256];

	if(I2cWrite(0xf7, 0x01, 0) != 1) return 0;
	if(I2cWrite(0xf9, 0x42, 0) != 1) return 0;
	if(I2cWrite(0xfc, 0x9e, 0) != 1) return 0;

	if(I2cWrite(0xfa, 0x88, 0) != 1) return 0;
	if(I2cWrite(0xd4, 0x81, 0) != 1) return 0;

	USHORT buf[16] ={0};
	USHORT startAddr = 0;
	for (i=0,j=0;i<0x78,j<16;i+=8,j++)
	{
		Sleep(10);
		if (I2cWrite(0xd5, (startAddr+i)&0xFF, 0) != 1) return 0;
		if (I2cWrite(0xf3, 0x20, 0) != 1) return 0;
		if (I2cRead(0xd7, &buf[j], 0) != 1) return 0;
	}

	for ( i=0; i<16; i++)
	{ 
		//		sprintf( section,"%02X",buf[i]);
		sprintf_s( section,"%c",buf[i]);
		FuseID += section;
	}
	
	return 1;
}
int GC2375::Init()
{
	return 1;
}
int GC2375::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	int R_gain, G_gain, B_gain;
	int nR_G_gain, nB_G_gain, nG_G_gain, nBase_gain;

	nR_G_gain = (Typical_rg * 1000)/rg;
	nB_G_gain = (Typical_bg * 1000)/bg;
	nG_G_gain = 1000;

	if (nR_G_gain < 1000 || nB_G_gain <1000)
	{
		if (nR_G_gain < nB_G_gain)
		{
			nBase_gain = nR_G_gain;
		}
		else
		{
			nBase_gain = nB_G_gain;
		}
	}
	else
	{
		nBase_gain = nG_G_gain;
	}

	R_gain = (0x400 * nR_G_gain) / nBase_gain;
	B_gain = (0x400 * nB_G_gain) / nBase_gain;
	G_gain = (0x400 * nG_G_gain) / nBase_gain;

	USHORT AWBGianBuffer[6];
	AWBGianBuffer[0]=0x80;
	AWBGianBuffer[1]=0x80;
	AWBGianBuffer[2]=0x80;
	AWBGianBuffer[3]=0x80;
	AWBGianBuffer[4]=0x00;
	AWBGianBuffer[5]=0x00;

	if (R_gain>0x400) 
	{   
		//R
		AWBGianBuffer[1]=(R_gain>>3);
		AWBGianBuffer[4]+=(R_gain&0x07);
	}   
	if (G_gain>0x400)
	{   
		//G1
		AWBGianBuffer[0]=(G_gain>>3);
		AWBGianBuffer[4]+=((G_gain&0x07)<<4);

		//G2
		AWBGianBuffer[3]=(G_gain>>3);
		AWBGianBuffer[5]+=(G_gain&0x07);
	}   
	if (B_gain>0x400)
	{   
		//B
		AWBGianBuffer[2]=(B_gain>>3);
		AWBGianBuffer[5]+=((B_gain&0x07)<<4); 
	}	
	I2cWrite(0xfe,0x00,0);

	I2cWrite(0xB8,AWBGianBuffer[0],0);
	Sleep(10);
	I2cWrite(0xB9,AWBGianBuffer[1],0);
	Sleep(10);
	I2cWrite(0xBA,AWBGianBuffer[2],0);
	Sleep(10);
	I2cWrite(0xBB,AWBGianBuffer[3],0);
	Sleep(10);
	I2cWrite(0xBE,AWBGianBuffer[4],0);
	Sleep(10);
	I2cWrite(0xBF,AWBGianBuffer[5],0);
	Sleep(10);

	return 1;
}
int GC2375::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int GC2375::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int GC2375::ApplySpc(short *pSPC)
{
	return 1;
}

int GC2375::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x03,&ExpHigh,0) != 1) return 0;
	if(I2cRead(0x04,&ExpLow,0) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);
	return 1;
} 
int GC2375::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;

	if(I2cWrite(0x03,ExpHigh,0) != 1) return 0;
	if(I2cWrite(0x04,ExpLow,0) != 1) return 0;
	return 1;
}


int GC2375::ReadGain( USHORT &gain)
{
	/*USHORT GainHigh;
	if(I2cRead(0xB6,&GainHigh,0) != 1) return 0;
	gain=GainHigh;
	return 1;*/
	kal_uint16 ANALOG_GAIN[9] = {64, 92, 128, 182, 254, 363, 521, 725, 1038};

	kal_uint16 again,dgain; 	

	I2cWrite(0xfe,0x00,0);

	I2cRead(0xb6, &again, 0);
	if(again<9)
		again = ANALOG_GAIN[again];
	else
	{
		printf("again is error");
		return 0;
	}

	kal_uint16 temp;
	I2cRead(0xb1, &temp, 0);

	dgain = ((temp&0x0f)<<6) + (temp>>2);

	gain = static_cast<USHORT>(dgain*again/64.0);

	return 1;
} 

int GC2375::WriteGain( USHORT gain)
{
	//if(I2cWrite(0xB6,gain&0xff,0) != 1) return 0;
	//return 1;
	kal_uint16 iReg,temp; 
	bool bRet = false;

	iReg = gain;

	if(iReg < 0x40)
		iReg = 0x40;	

	if((ANALOG_GAIN_1<= iReg)&&(iReg < ANALOG_GAIN_2))
	{
		bRet &= (I2cWrite(0x20,0x0b, 0) == 1);
		I2cWrite(0x22,0x0c, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x00, 0);
		temp = iReg;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 1x, GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_2<= iReg)&&(iReg < ANALOG_GAIN_3))
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);
		//analog gain
		I2cWrite(0xb6,0x01, 0);
		temp = 64*iReg/ANALOG_GAIN_2;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 1.43x , GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_3<= iReg)&&(iReg < ANALOG_GAIN_4))
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x02, 0);
		temp = 64*iReg/ANALOG_GAIN_3;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 2x , GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_4<= iReg)&&(iReg < ANALOG_GAIN_5))
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x03, 0);
		temp = 64*iReg/ANALOG_GAIN_4;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 2.84x , GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_5<= iReg)&&(iReg < ANALOG_GAIN_6))
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x04, 0);
		temp = 64*iReg/ANALOG_GAIN_5;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 3.97x , GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_6<= iReg)&&(iReg < ANALOG_GAIN_7))
	{
		I2cWrite(0x20,0x0e, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);
		//analog gain
		I2cWrite(0xb6,0x05, 0);
		temp = 64*iReg/ANALOG_GAIN_6;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 5.68x , GC2375HMIPI add pregain = %d\n",temp);
	}	
	else if((ANALOG_GAIN_7<= iReg)&&(iReg < ANALOG_GAIN_8))
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0c, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x06, 0);
		temp = 64*iReg/ANALOG_GAIN_7;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 8.14x , GC2375HMIPI add pregain = %d\n",temp);
	}
	else if((ANALOG_GAIN_8<= iReg)&&(iReg < ANALOG_GAIN_9))
	{
		I2cWrite(0x20,0x0e, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x07, 0);
		temp = 64*iReg/ANALOG_GAIN_8;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 11.34x , GC2375HMIPI add pregain = %d\n",temp);
	}	
	else 
	{
		I2cWrite(0x20,0x0c, 0);
		I2cWrite(0x22,0x0e, 0);
		I2cWrite(0x26,0x0e, 0);	
		//analog gain
		I2cWrite(0xb6,0x08, 0);
		temp = 64*iReg/ANALOG_GAIN_9;
		I2cWrite(0xb1,temp>>6, 0);
		I2cWrite(0xb2,(temp<<2)&0xfc, 0);
		//LOG_INF("GC2375HMIPI analogic gain 16.23x , GC2375HMIPI add pregain = %d\n",temp);
	}

	return 1;
} 