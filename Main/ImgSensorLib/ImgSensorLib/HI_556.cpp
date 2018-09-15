  

#include "StdAfx.h"
#include "HI_556.h"
#include "ccmBaseInterface.h"


HI_556::HI_556(void)
{

}


HI_556::~HI_556(void)
{
}
int HI_556::ReadReg(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
    if(I2cWrite(0x0a02, 0x01, 3) != 1) return 0; //Fast sleep on
    if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0; // stand by on
    Sleep(10);
    if(I2cWrite(0x0f02, 0x00, 3) != 1) return 0; // pll disable
    if(I2cWrite(0x011a, 0x01, 3) != 1) return 0; // CP TRIM_H
    if(I2cWrite(0x011b, 0x09, 3) != 1) return 0; // IPGM TRIM_H
    if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0; // Fsync(OTP busy) Output Enable
    if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0; // Fsync(OTP busy) Output Drivability
    if(I2cWrite(0x003e, 0x10, 3) != 1) return 0; // OTP R/W mode
    if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0; // stand by off

    if(I2cWrite (0x10a, (startAddr>>8)&0xff, 3) != 1) return 0; // start address H
    if(I2cWrite (0x10b, startAddr&0xff, 3) != 1) return 0; // start address L
    if(I2cWrite (0x102, 0x01, 3) != 1) return 0; // single read
    for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {  
        if(I2cRead(0x108, buf+uAddr-startAddr, 3) != 1) return 0;    
    }

    if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0;  // stand by on
    Sleep (10);
    if(I2cWrite(0x003e, 0x00, 3) != 1) return 0; // display mode
    if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0; // stand by off

	return 1;
}

int HI_556::WriteReg(USHORT startAddr, USHORT endAddr, USHORT *buf, USHORT page)
{
    if(I2cWrite(0x0a02, 0x01, 3) != 1) return 0; //Fast sleep on
    if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0; // stand by on
    Sleep(100);
    if(I2cWrite(0x0f02, 0x00, 3) != 1) return 0; // pll disable
    if(I2cWrite(0x011a, 0x01, 3) != 1) return 0; // CP TRIM_H
    if(I2cWrite(0x011b, 0x09, 3) != 1) return 0; // IPGM TRIM_H
    if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0; // Fsync(OTP busy) Output Enable
    if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0; // Fsync(OTP busy) Output Drivability
    if(I2cWrite(0x003e, 0x10, 3) != 1) return 0; // OTP R/W mode
    if(I2cWrite(0x010F, 0x03, 3) != 1) return 0;
    if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0; // stand by off
    Sleep(100);

    USHORT uTemp = 0;
    int iFalg = 1;
    for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {
        if(I2cWrite(0x10a, (uAddr>>8)&0xff, 3) != 1) return 0; // start address H
        if(I2cWrite(0x10b, uAddr&0xff, 3) != 1) return 0; // start address L
        if(I2cWrite(0x102, 0x02, 3) != 1) return 0; // single write
        if(I2cWrite(0x106, buf[uAddr-startAddr], 3) != 1) return 0; // OTP data write
        Sleep(5);

        if(I2cRead(0x010d, &uTemp, 3) != 1) return 0;
        if(buf[uAddr-startAddr] != uTemp) //verify
        {
            for (int i=0; i<3; i++)
            {
                if(I2cWrite(0x10a, (uAddr>>8)&0xff, 3) != 1) return 0;	// start address H
                if(I2cWrite(0x10b, uAddr&0xff, 3) != 1) return 0; // start address L
                if(I2cWrite(0x102, 0x02, 3) != 1) return 0; // single write
                if(I2cWrite(0x106, buf[uAddr-startAddr], 3) != 1) return 0; // OTP data write
                Sleep(5);
            }

            if(I2cRead(0x010d, &uTemp, 3) != 1) return 0;
        }

        if (uTemp != buf[uAddr-startAddr])
        {
            iFalg = 0;    
        }
    }

    Sleep(100);
    if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0; // stand by on
    Sleep(100);
    if(I2cWrite(0x003e, 0x00, 3) != 1) return 0; // display mode
    if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0; // stand by off

	return iFalg;
}

BOOL HI_556::ReadOTPPage(int page, USHORT *ReadData)
{
	
	return 1;

}
int HI_556::GetTemperature(USHORT &temperature)
{
	return 1;
}

int HI_556::GetFuseID(CString &FuseID) 
{
	FuseID = _T("");
	if(I2cWrite(0x0A02, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x0A00, 0x00, 3) != 1) return 0;
	Sleep(10);
	if(I2cWrite(0x0F02, 0x00, 3) != 1) return 0;
	if(I2cWrite(0x011A, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x011B, 0x09, 3) != 1) return 0;
	if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0;
	if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0;
	if(I2cWrite(0x003E, 0x10, 3) != 1) return 0;
	if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0;

	if(I2cWrite(0x010a, 0x00, 3) != 1) return 0; // start address H 
	if(I2cWrite(0x010b, 0x01, 3) != 1) return 0; //start address L 
	if(I2cWrite(0x0102, 0x01, 3) != 1) return 0; //Read Enable

	USHORT temp1  = 0;
	int i=0;
	FuseID = _T("");
	char section[256] = {0};
	for ( i=0; i<9; i++)
	{ 
		if(I2cRead(0x0108, &temp1, 3) != 1) return 0;
		sprintf( section,"%02X",temp1);
		CString str(section);
		FuseID += str;
	}

    if(I2cWrite(0x0A00, 0x00, 3) != 1) return 0; // stand by on 
    Sleep (10); 
    if(I2cWrite(0x003E, 0x00, 3) != 1) return 0; // display mode 
    if(I2cWrite(0x0A00, 0x01, 3) != 1) return 0; // stand by off 

	return 1;
}
int HI_556::Init()
{
	return 1;
}
int HI_556::ApplyAWBGain(int rg, int bg, int Typical_rg, int Typical_bg)
{
    USHORT HGain = (rg >> 8) & 0xff;
    USHORT LGain = rg & 0xff;
    if(I2cWrite(0x007C, HGain, 3) != 1) return 0;
    if(I2cWrite(0x007D, LGain, 3) != 1) return 0;
    HGain = (bg >> 8) & 0xff;
    LGain = bg & 0xff;
    if(I2cWrite(0x007E, HGain, 3) != 1) return 0;
    if(I2cWrite(0x007F, LGain, 3) != 1) return 0;

	return 1;
}
int HI_556::ApplyLsc(BYTE* raw8,int width,int height,int nLSCTarget,int ob,UCHAR* nLenCReg,int nLenCRegCount,int LSCGroup)
{
	return 1;
}

int HI_556::spcCal(USHORT *imgBuf,short *pSPC)
{
	return 1;
}

int HI_556::ApplySpc(short *pSPC)
{
	return 1;
}
 

int HI_556::ReadExp( int &exp)
{
	USHORT ExpHigh;
	USHORT ExpLow;
	if(I2cRead(0x0074, &ExpHigh, 3) != 1) return 0;
	if(I2cRead(0x0075, &ExpLow, 3) != 1) return 0;
	exp=(ExpHigh<<8)+(ExpLow&0xFF);

	return 1;
} 

int HI_556::WriteExp( int exp)
{
	USHORT ExpHigh=exp>>8;
	USHORT ExpLow=exp&0xFF;
	 
	if(I2cWrite(0x0074, ExpHigh, 3) != 1) return 0;
	if(I2cWrite(0x0075, ExpLow, 3) != 1) return 0;

	return 1;
} 

int HI_556::ReadGain( USHORT &gain)
{
    if(I2cRead(0x0077, &gain, 3) != 1) return 0;

	return 1;
} 
int HI_556::WriteGain( USHORT gain)
{
	gain = gain & 0xff;
    if(I2cWrite(0x0077, gain, 3) != 1) return 0;

	return 1;
} 