// RX_OTPCheck.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RX_OTPCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRX_OTPCheckApp

BEGIN_MESSAGE_MAP(CRX_OTPCheckApp, CWinApp)
END_MESSAGE_MAP()


// CRX_OTPCheckApp 构造

CRX_OTPCheckApp::CRX_OTPCheckApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRX_OTPCheckApp 对象

CRX_OTPCheckApp theApp;


// CRX_OTPCheckApp 初始化

BOOL CRX_OTPCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
