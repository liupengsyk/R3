// MesUpdate.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MesUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMesUpdateApp

BEGIN_MESSAGE_MAP(CMesUpdateApp, CWinApp)
END_MESSAGE_MAP()


// CMesUpdateApp 构造

CMesUpdateApp::CMesUpdateApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMesUpdateApp 对象

CMesUpdateApp theApp;


// CMesUpdateApp 初始化

BOOL CMesUpdateApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
