// AE.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAEApp

BEGIN_MESSAGE_MAP(CAEApp, CWinApp)
END_MESSAGE_MAP()


// CAEApp 构造

CAEApp::CAEApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAEApp 对象

CAEApp theApp;


// CAEApp 初始化

BOOL CAEApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
