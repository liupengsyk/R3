// HwDarkPartical.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "HwDarkPartical.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHwDarkParticalApp

BEGIN_MESSAGE_MAP(CHwDarkParticalApp, CWinApp)
END_MESSAGE_MAP()


// CHwDarkParticalApp 构造

CHwDarkParticalApp::CHwDarkParticalApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHwDarkParticalApp 对象

CHwDarkParticalApp theApp;


// CHwDarkParticalApp 初始化

BOOL CHwDarkParticalApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
