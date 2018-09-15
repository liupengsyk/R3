// FPN.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FPN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFPNApp

BEGIN_MESSAGE_MAP(CFPNApp, CWinApp)
END_MESSAGE_MAP()


// CFPNApp 构造

CFPNApp::CFPNApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFPNApp 对象

CFPNApp theApp;


// CFPNApp 初始化

BOOL CFPNApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
