
// OFccmTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "OFccmControl.h"
//#include "CameraControl.h"
//#include "DeviceInterface.h"
//#include "ConfigManage.h"

// COFccmTestApp:
// �йش����ʵ�֣������ OFccmTest.cpp
//
#define WM_GETMASKEDEDIT_TEXT 10

class COFccmTestApp : public CWinApp
{
public:
	COFccmTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
//
  
};

extern COFccmTestApp theApp;
extern OFccmControl* pCcmCtrl;
extern int isCreateCtrl;
extern vector<BufInfo> bufManager;

