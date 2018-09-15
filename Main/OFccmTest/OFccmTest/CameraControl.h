#pragma once
#include "stdafx.h"
#include "OFccmDef.h" 

class CCameraControl
{
public:
	CCameraControl(int CamID);
	~CCameraControl(void);

	int CamID;
	ConfigManage* pConfig; 

	int OpenCamera(int Camid); 

	int CloseCamera(int CamID=0);

	  
	SensorTab SensorInfo;
	BOOL IsOpenDevice;
	 

	 ULONG m_GrabSize;
	/* BYTE DeviceID;*/
	 int Init();
	  
	int SetLocalExp();
 
	
};

