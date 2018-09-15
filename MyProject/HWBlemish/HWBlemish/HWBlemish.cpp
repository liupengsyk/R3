// HWBlemish.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "HWBlemish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHWBlemishApp

BEGIN_MESSAGE_MAP(CHWBlemishApp, CWinApp)
END_MESSAGE_MAP()


// CHWBlemishApp 构造

CHWBlemishApp::CHWBlemishApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHWBlemishApp 对象

CHWBlemishApp theApp;


// CHWBlemishApp 初始化

BOOL CHWBlemishApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
