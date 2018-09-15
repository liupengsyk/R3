  

#include "StdAfx.h"
#include "HI_846.h"


HI_846::HI_846(void)
{

}


HI_846::~HI_846(void)
{
}

int HI_846:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}
int HI_846::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	return 1;
}

BOOL HI_846::ReadOTPPage(int page, USHORT *ReadData)
{
	
	return 1;

}
int HI_846::GetTemperature(USHORT &temperature)
{
	return 1;
}

int HI_846::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");

#if 0
	I2cWrite(0x0a02, 0x01,3); 
	I2cWrite(0x0a00, 0x00,3); 
	Sleep(20);
	I2cWrite(0x0f02, 0x00,3); 
	I2cWrite(0x071a, 0x01,3); 
	I2cWrite(0x071b, 0x09,3); 
	I2cWrite(0x0d04, 0x01,3); 
	I2cWrite(0x0d00, 0x07,3); 
	I2cWrite(0x003e, 0x10,3); 
	I2cWrite(0x070f, 0x05,3);
	I2cWrite(0x0a00, 0x01,3); 

	I2cWrite(0x070a, 0x00,3); 
	I2cWrite(0x070b, 0x01,3); 
	I2cWrite(0x0702, 0x01,3); 

	 USHORT temp = 0;
	int i=0;
	FuseID = _T("");
	char section[256] = {0};
	for ( i=0; i<9; i++)
	{ 
		int flag = I2cRead(0x0708, &temp, 3);
		sprintf( section,"%02X",temp);
		CString str(section);
		FuseID += str;
	}

	I2cWrite(0x0a00, 0x00,3); 
	Sleep(20);
	I2cWrite(0x003e, 0x00,3); 
	I2cWrite(0x0a00, 0x01,3);
#else
	USHORT uFuseID[30] = {0};
	for (int i=0; i<30; i++)
	{
		if(I2cRead(0xA8,0x0006+i,&uFuseID[i],3) != 1) return 0;
	}
	CString temp = _T("");
	for ( int i=0; i<15; i++)
	{ 
		temp.Format(_T("%02X"),uFuseID[i]);
		FuseID += temp;
	}
	
#endif

	return 1;
}

int HI_846::Init()
{
	return 1;
}

int HI_846::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}

int HI_846::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int HI_846::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int HI_846::ApplySpc(short *pSPC)
{
	return 1;
}
 

int HI_846::ReadExp( int &exp)
{
	USHORT ExpT = 0;

	if(I2cRead(0x0074, &ExpT, 4) != 1) return 0;
	
	exp = ExpT;

	return 1;
} 

int HI_846::WriteExp( int exp)
{
	if(I2cWrite(0x0074, exp, 4) != 1) return 0;
	return 1;
} 

int HI_846::ReadGain( USHORT &gain)
{
	USHORT analogGain;

	if(I2cRead(0x0076, &analogGain, 4) != 1) return 0;	 
	gain = analogGain;

	return 1;
} 

int HI_846::WriteGain( USHORT gain)
{
	if(I2cWrite(0x0076, gain, 4) != 1) return 0;

	return 1;
} 