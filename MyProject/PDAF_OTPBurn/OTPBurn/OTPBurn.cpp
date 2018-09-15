// OTPBurn.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OTPBurn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COTPBurnApp

BEGIN_MESSAGE_MAP(COTPBurnApp, CWinApp)
END_MESSAGE_MAP()


// COTPBurnApp 构造

COTPBurnApp::COTPBurnApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COTPBurnApp 对象

COTPBurnApp theApp;


// COTPBurnApp 初始化

BOOL COTPBurnApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
