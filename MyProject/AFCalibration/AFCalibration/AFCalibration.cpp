// AFCalibration.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AFCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAFCalibrationApp

BEGIN_MESSAGE_MAP(CAFCalibrationApp, CWinApp)
END_MESSAGE_MAP()


// CAFCalibrationApp 构造

CAFCalibrationApp::CAFCalibrationApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAFCalibrationApp 对象

CAFCalibrationApp theApp;


// CAFCalibrationApp 初始化

BOOL CAFCalibrationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
