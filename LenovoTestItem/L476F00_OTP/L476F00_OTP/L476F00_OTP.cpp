// L476F00_OTP.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "L476F00_OTP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CL476F00_OTPApp

BEGIN_MESSAGE_MAP(CL476F00_OTPApp, CWinApp)
END_MESSAGE_MAP()


// CL476F00_OTPApp 构造

CL476F00_OTPApp::CL476F00_OTPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CL476F00_OTPApp 对象

CL476F00_OTPApp theApp;


// CL476F00_OTPApp 初始化

BOOL CL476F00_OTPApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
