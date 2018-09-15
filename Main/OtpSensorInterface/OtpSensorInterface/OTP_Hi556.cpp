#include "StdAfx.h"     
#include "ccmBaseInterface.h"
#include "OTP_Hi556.h"


OTP_Hi556::OTP_Hi556(void)
{
}


OTP_Hi556::~OTP_Hi556(void)
{
}


int OTP_Hi556::OtpInit()
{	
	return 0;
}

int OTP_Hi556::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if (NULL == buf || endAddr < startAddr)	return 0;

	if(I2cWrite(0x0a02, 0x01, 3) != 1) return 0;     //Fast sleep on
	if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0;     // stand by on

	Sleep(5);

	if(I2cWrite(0x0f02, 0x00, 3) != 1) return 0;     // pll disable
	if(I2cWrite(0x011a, 0x01, 3) != 1) return 0;     // CP TRIM_H
	if(I2cWrite(0x011b, 0x09, 3) != 1) return 0;     // IPGM TRIM_H
	if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0;     // Fsync(OTP busy) Output Enable
	if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0;     // Fsync(OTP busy) Output Drivability
	if(I2cWrite(0x003e, 0x10, 3) != 1) return 0;     // OTP R/W mode
	if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;     // stand by off

	if(I2cWrite(0x10a, (startAddr>>8)&0xff, 3) != 1) return 0;     // start address H
	if(I2cWrite(0x10b, startAddr&0xff, 3) != 1) return 0;          // start address L
	if (I2cWrite(0x102, 0x01, 3) != 1) return 0;                   // single read
 
	for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {  
		if(I2cRead(0x108, buf+uAddr-startAddr, 3) != 1) return 0;         
    }
	if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0;     // stand by on
	Sleep(10);
	if(I2cWrite(0x003e, 0x00, 3) != 1) return 0;     // display mode
	if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;     // stand by off

	return 1;
}

int OTP_Hi556::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if (NULL == buf || endAddr < startAddr)	return 0;

	if(I2cWrite(0x0a02, 0x01, 3) != 1) return 0;     //Fast sleep on
	if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0;     // stand by on
    Sleep(100);
	if(I2cWrite(0x0f02, 0x00, 3) != 1) return 0;     // pll disable
	if(I2cWrite(0x011a, 0x01, 3) != 1) return 0;     // CP TRIM_H
	if(I2cWrite(0x011b, 0x09, 3) != 1) return 0;     // IPGM TRIM_H
	if(I2cWrite(0x0d04, 0x01, 3) != 1) return 0;     // Fsync(OTP busy) Output Enable
	if(I2cWrite(0x0d00, 0x07, 3) != 1) return 0;     // Fsync(OTP busy) Output Drivability
	if(I2cWrite(0x003e, 0x10, 3) != 1) return 0;     // OTP R/W mode
	if(I2cWrite(0x010F, 0x03, 3) != 1) return 0;
    if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;     // stand by off
    Sleep(100);

    USHORT uTemp = 0;
    int iFalg = 1;
    for (USHORT uAddr=startAddr; uAddr<=endAddr; ++uAddr)
    {
		if(I2cWrite(0x10a, (uAddr>>8)&0xff, 3) != 1) return 0;              // start address H
		if(I2cWrite(0x10b, uAddr&0xff, 3) != 1) return 0;                   // start address L
		if(I2cWrite(0x102, 0x02, 3) != 1) return 0;                         // single write
		if(I2cWrite(0x106, buf[uAddr-startAddr], 3) != 1) return 0;         // OTP data write
        Sleep(5);

		if(I2cRead(0x010d, &uTemp, 3) != 1) return 0;
        
        if(buf[uAddr-startAddr] != uTemp) //verify
        {
            for (int i=0; i<3; i++)
            {
				if(I2cWrite(0x10a, (uAddr>>8)&0xff, 3) != 1) return 0;      // start address H
                if(I2cWrite(0x10b, uAddr&0xff, 3) != 1) return 0;           // start address L
                if(I2cWrite(0x102, 0x02, 3) != 1) return 0;                 // single write
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
	if(I2cWrite(0x0a00, 0x00, 3) != 1) return 0;     // stand by on
    Sleep(100);
	if(I2cWrite(0x003e, 0x00, 3) != 1) return 0;     // display mode
	if(I2cWrite(0x0a00, 0x01, 3) != 1) return 0;     // stand by off

	return iFalg;
}
 