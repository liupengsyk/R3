// AF_SFR.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AF_SFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAF_SFRApp

BEGIN_MESSAGE_MAP(CAF_SFRApp, CWinApp)
END_MESSAGE_MAP()


// CAF_SFRApp 构造

CAF_SFRApp::CAF_SFRApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAF_SFRApp 对象

CAF_SFRApp theApp;


// CAF_SFRApp 初始化

BOOL CAF_SFRApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
