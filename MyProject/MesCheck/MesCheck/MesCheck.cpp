// MesCheck.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MesCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMesCheckApp

BEGIN_MESSAGE_MAP(CMesCheckApp, CWinApp)
END_MESSAGE_MAP()


// CMesCheckApp 构造

CMesCheckApp::CMesCheckApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMesCheckApp 对象

CMesCheckApp theApp;


// CMesCheckApp 初始化

BOOL CMesCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
