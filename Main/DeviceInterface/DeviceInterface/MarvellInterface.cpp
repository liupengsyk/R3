#include "StdAfx.h"
#include "deviceinterface.h"
#include "CusbFrameInput.h"
#include "MarvellInterface.h"

#pragma comment(lib,"Marvell/MrvlJigClient.lib")
#pragma comment(lib,"Marvell/usb-frm11.lib")

CMarvellInterface::CMarvellInterface(void)
{
	pMrvlCMTBClient = new CMrvlCMTBClient();
}


CMarvellInterface::~CMarvellInterface(void)
{

}
int CMarvellInterface::OpenDevice(int PortType ,int XCLK ,int DeviceNum,int iDevID,int channel )
{
	char stradbID[40] = {0}; 
	int flag = 0;
	BOOL bflag = FALSE;
	// pMrvlCMTBClient->Cleanup();
	//flag =  pMrvlCMTBClient->BoardReset();
	//flag =  pMrvlCMTBClient->CamReset();
	for (;;)
	{
		flag = pMrvlCMTBClient->Initialize(1,"mrvlhvs1");


		flag = pMrvlCMTBClient->CameraOn(1);
		int nImgSize = 2592*1920*2;
		char* pimagebuf_Data = new char[nImgSize+ 1024];
		flag = pMrvlCMTBClient->Get_CaptureData((char*)pimagebuf_Data, &nImgSize);

	}
	 
	return 0;
}
