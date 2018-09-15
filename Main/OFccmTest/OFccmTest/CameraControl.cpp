#include "StdAfx.h"
#include "DeviceInterface.h"
#include "ConfigManage.h"
#include "OFccmTest.h"
#include "CameraControl.h"


CCameraControl::CCameraControl(int CamID)
{
	this->CamID = CamID;
	pConfig = NULL;
	m_GrabSize =0; 
 
}


CCameraControl::~CCameraControl(void)
{
	//SAFE_DELETE(pConfig);
	pConfig = NULL;
}

int CCameraControl::Init()
{	 
	/*DeviceID = pCcmCtrl->0;*/
	this->pConfig = pCcmCtrl->Cam[CamID].pConfig;
	SensorInfo = *(this->pConfig->GetSensorTab());
 
	return 0;
}
 
 

int CCameraControl::OpenCamera(int Camid)
{ 
	//Init();
	int Flag = pCcmCtrl->pDevice->OpenVideo(Camid);
 
	return Flag;  
}  

int CCameraControl::CloseCamera(int CamID)
{ 
	return pCcmCtrl->pDevice->CloseDevice(CamID);  
}

 
 
int CCameraControl::SetLocalExp()
{
	if (pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->bUseLocalExp)	
	{ 
		int lastexp = pCcmCtrl->Cam[CamID].pConfig->GetLastExp();
		int LastAgain = pCcmCtrl->Cam[CamID].pConfig->GetLastAgain();
		if (lastexp!=0)
		{
			CString str;
			int ret = pCcmCtrl->Sensor_WriteExp(CamID,lastexp);  //读当前EXP 
			if (ret!=1)
			{
				
				str.Format(_T("Cam%d,Exp写入失败"),CamID);
				pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_RED,200);
			}
			else
			{
				str.Format(_T("Cam%d,Exp %d"),CamID,lastexp);
				pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
			}
			Sleep(10);
			ret = pCcmCtrl->Sensor_WriteGain(CamID,LastAgain);    //读当前EXP 
			if (ret!=1)
			{ 
				str.Format(_T("Cam%d,aGain写入失败"),CamID);
				pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_RED,200);
			}
			else
			{
				str.Format(_T("Cam%d,aGain %d"),CamID,LastAgain);
				pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
			}

		}
	}
	return 0;
}