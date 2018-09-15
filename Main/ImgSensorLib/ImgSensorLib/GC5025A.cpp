#include "StdAfx.h"
#include "GC5025A.h"


GC5025A::GC5025A(void)
{
}


GC5025A::~GC5025A(void)
{
}

int GC5025A:: ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	USHORT Check = 0;
	
	if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if(I2cWrite(0xf7, 0x01, 0) != 1) return 0;
	if(I2cWrite(0xf8, 0x11, 0) != 1) return 0;
	if(I2cWrite(0xf9, 0x00, 0) != 1) return 0;
	if(I2cWrite(0xfa, 0xa0, 0) != 1) return 0;
	if(I2cWrite(0xfc, 0x2e, 0) != 1) return 0;
	if(I2cWrite(0xfa, 0xb0, 0) != 1) return 0;
	if(I2cWrite(0xd4, 0x80, 0) != 1) return 0;

	if(page)
		if(I2cWrite(0xd4, (0x84+((startAddr>>8)&0x03)), 0) != 1) return 0;
	    
	else
		if(I2cWrite(0xd4, (0x80+((startAddr>>8)&0x03)), 0) != 1) return 0;

    if(I2cWrite(0xd5, (startAddr&0xff), 0) != 1) return 0;
    if(I2cWrite(0xf3, 0x20, 0) != 1) return 0;
	if(I2cWrite(0xf3, 0x88, 0) != 1) return 0;
    USHORT uCnt = 0;
    for (USHORT uAddr=startAddr; uAddr<=endAddr; uAddr+=8)
    {  
        Sleep(2);
        if(I2cRead(0xd7, buf+uCnt, 0) != 1) return 0;
        uCnt += 1;
    }

    if(I2cWrite(0xf3, 0x00, 0) != 1) return 0;
	return 1;
}

int GC5025A::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf, USHORT page)
{

    return 1;
}

int GC5025A::GetTemperature(USHORT &temperature)
{
	return 1;
}
int GC5025A::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
    USHORT buf[(0x03f8-0x3b0)/8 + 1];
    if(ReadReg(0x03b0, 0x03f8, buf, 1) != 1) return 0;
    for (USHORT uAddr=0x3b0; uAddr<=0x3f8; uAddr+=8)
    {
        FuseID.Format(_T("%s%02x"), FuseID.GetBuffer(), buf[(uAddr-0x3b0)/8]);    
    }

	return 1;
}
int GC5025A::Init()
{
	return 1;
}
int GC5025A::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
	return 1;
}
int GC5025A::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int GC5025A::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}
int GC5025A::ApplySpc(short *pSPC)
{
	return 1;
}

 
int GC5025A::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
    if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
    if(I2cRead(0x03, &ExpHigh, 0) != 1) return 0;
    if(I2cRead(0x04, &ExpLow, 0) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int GC5025A::WriteExp( int exp)
{
    exp = exp * 2;
    if (exp <= 10)
    {
        if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
        if(I2cWrite(0xd9, 0xdd, 0) != 1) return 0;
        if(I2cWrite(0xb0, 0x58, 0) != 1) return 0;
    }
    else
    {
        if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
        if(I2cWrite(0xd9, 0xaa, 0) != 1) return 0;
        if(I2cWrite(0xb0, 0x4b, 0) != 1) return 0;
    }

	USHORT ExpHigh = (exp>>8) & 0x3F;
	USHORT ExpLow = exp & 0xFF;
    if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if(I2cWrite(0x03, ExpHigh, 0) != 1) return 0;
	if(I2cWrite(0x04, ExpLow, 0) != 1) return 0;

	return 1;
} 

int GC5025A::ReadGain( USHORT &gain)
{
    USHORT lGain = 0, hGain = 0;
	if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
    if(I2cRead(0xb1, &hGain, 0) != 1) return 0;
    if(I2cRead(0xb2, &lGain, 0) != 1) return 0;
    gain = ((hGain & 0x3F) << 8) + (lGain>>2);

	return 1;
} 
int GC5025A::WriteGain( USHORT gain)
{
    if (gain < 0x40)
    {
        gain = 0x40;
    }
    
    if(I2cWrite(0xfe, 0x00, 0) != 1) return 0;
    if ((64 <= gain) && (gain <91))
    {
        if(I2cWrite(0xb6, 0x00, 0) != 1) return 0;
    }
    else if ((91 <= gain) && (gain < 127))
    {
        if(I2cWrite(0xb6, 0x01, 0) != 1) return 0;
    }
    else if ((127 <= gain) && (gain < 181))
    {
        if(I2cWrite(0xb6, 0x02, 0) != 1) return 0;
    }
    else
    {
        if(I2cWrite(0xb6, 0x03, 0) != 1) return 0;
    }

    if(I2cWrite(0xb1, (gain >> 6), 0) != 1) return 0;
    if(I2cWrite(0xb2, (gain << 2) & 0xfc, 0) != 1) return 0;

	return 1;
} 