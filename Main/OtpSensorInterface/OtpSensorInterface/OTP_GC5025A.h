#pragma once
#include "otpsensorinterface.h"
class OTP_GC5025A :
    public COtpSensorInterface
{
public:
    OTP_GC5025A(void);
    ~OTP_GC5025A (void);

    int OtpInit();
    int OtpRead(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 1);
    int OtpWrite(USHORT startAddr, USHORT endAddr, USHORT *buf,USHORT page = 1);


};

