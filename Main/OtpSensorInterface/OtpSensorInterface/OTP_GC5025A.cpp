#include "StdAfx.h"
#include "OTP_GC5025A.h"


OTP_GC5025A::OTP_GC5025A(void)
{
}


OTP_GC5025A::~OTP_GC5025A(void)
{
}


int OTP_GC5025A::OtpInit()
{	
    return 0;
}

int OTP_GC5025A::OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

    USHORT Check = 0;
	if (I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if (I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if (I2cWrite(0xfe, 0x00, 0) != 1) return 0;
	if (I2cWrite(0xf7, 0x01, 0) != 1) return 0;
	if (I2cWrite(0xf8, 0x11, 0) != 1) return 0;
	if (I2cWrite(0xf9, 0x00, 0) != 1) return 0;
	if (I2cWrite(0xfa, 0xa0, 0) != 1) return 0;
	if (I2cWrite(0xfc, 0x2e, 0) != 1) return 0;
	if (I2cWrite(0xfa, 0xb0, 0) != 1) return 0;
	if (I2cWrite(0xd4, 0x80, 0) != 1) return 0;  
    
    if(page)
	{
		if (I2cWrite(0xd4, (0x84+((startAddr>>8)&0x03)), 0) != 1) return 0;        
	}
    else
	{
		if (I2cWrite(0xd4, (0x80+((startAddr>>8)&0x03)), 0) != 1) return 0; 
	}

	if (I2cWrite(0xd5, (startAddr&0xff), 0) != 1) return 0;
	if (I2cWrite(0xf3, 0x20, 0) != 1) return 0;
	if (I2cWrite(0xf3, 0x88, 0) != 1) return 0;
    
    USHORT uCnt = 0;
    for (USHORT uAddr=startAddr; uAddr<=endAddr; uAddr+=8)
    {  
		if (I2cRead(0xd7, buf+uCnt, 0) != 1) return 0;
        uCnt++;
    }

	if (I2cWrite(0xf3, 0x00, 0) != 1) return 0;
    return 1;
}

int OTP_GC5025A::OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page)
{
	if(NULL == buf || endAddr < startAddr) return 0;

    USHORT nCnt = 0;
    USHORT uTemp = 0;
    USHORT uVal = 0;
    USHORT Check = 0;
    int ret = 1;

	if(OtpRead(0x0138, 0x0138, &Check, 1) != 1) return 0;
    if(Check&0x01)
    {
        for (USHORT uAddr=startAddr; uAddr<=endAddr; uAddr+=8)
        {
			if(I2cWrite(0xd4, 0x84+(uAddr>>8)&0x03, 0) != 1) return 0;
			if(I2cWrite(0xd5, (uAddr&0xff), 0) != 1) return 0;
            if(I2cWrite(0xd6, buf[nCnt], 0) != 1) return 0;
            if(I2cWrite(0xd8, 0x30, 0) != 1) return 0;   //Select T1
			if(I2cWrite(0xf3, 0x44, 0) != 1) return 0;
            
            do
            {
				if(I2cRead(0xd4, &Check, 0) != 1) return 0;                
            }while((Check&0x20));

            for (int i=0; i<5; i++)//Repeat 5 times
            {
				if(I2cWrite(0xf3, 0x20, 0) != 1) return 0;    //OTP write

                do
                {
					if(I2cRead(0xd4, &Check, 0) != 1) return 0;                    
                }while(Check&0x20);

				if(I2cRead(0xd7, &Check, 0) != 1) return 0;
                
                if(Check != buf[nCnt])
                {
                    for(int j=0; j<4; j++)
                    {
						if (I2cWrite(0xf3, 0x42, 0) != 1) return 0;
	                        
                        do
                        {
							if(I2cRead(0xd4, &Check, 0) != 1) return 0;
                            
                        }while(Check&0x20);
                    }

                    for(int j=0; j<3; j++)
                    {
						if(I2cWrite(0xd8, 0x28, 0) != 1) return 0;
						if(I2cWrite(0xf3, 0x20, 0) != 1) return 0;                        
                        
                        do
                        {
							if(I2cRead(0xd4, &Check, 0) != 1) return 0;                            
                        }while(Check&0x20);

						if(I2cRead(0xd7, &Check, 0) != 1) return 0;
                        
                        if(Check != buf[nCnt])
                        {
                            return 0;
                        }
                    }
                }
            }

			if(I2cWrite(0xd8, 0x40, 0) != 1) return 0;
            
            for(int i=0; i<3; i++)
            {
				if(I2cWrite(0xf3, 0x41, 0) != 1) return 0;                
                do
                {
					if(I2cRead(0xd4, &Check, 0) != 1) return 0;
                    
                }while(Check&0x20);
            }

            for(int i=0; i<3; i++)
            {
				if(I2cWrite(0xd8, 0x28, 0) != 1) return 0;
				if(I2cWrite(0xf3, 0x20, 0) != 1) return 0;                
                
                do
                {
					if(I2cRead(0xd4, &Check, 0) != 1) return 0;
                    
                }while(Check&0x20);

				if(I2cRead(0xd7, &Check, 0) != 1) return 0;
                
                if(Check != buf[nCnt])
                {
                    return 0;
                }
            }
            nCnt += 1;
        }
    }
    else
        ret = 0;

    return ret;
}