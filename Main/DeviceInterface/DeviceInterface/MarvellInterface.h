#pragma once

#include "CMrvlCMTBClient.h"
class CMarvellInterface :
	public CDeviceInterface
{
public:
	CMarvellInterface(void);
	~CMarvellInterface(void);
	int OpenDevice(int PortType  =0 ,int XCLK  = 24 ,int DeviceNum=0,int iDevID=0,int channel  = 1);

private:
	CMrvlCMTBClient* pMrvlCMTBClient;
};

