// SensorLSCApply.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SensorLSCApply.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSensorLSCApplyApp

BEGIN_MESSAGE_MAP(CSensorLSCApplyApp, CWinApp)
END_MESSAGE_MAP()


// CSensorLSCApplyApp 构造

CSensorLSCApplyApp::CSensorLSCApplyApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSensorLSCApplyApp 对象

CSensorLSCApplyApp theApp;


// CSensorLSCApplyApp 初始化

BOOL CSensorLSCApplyApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
