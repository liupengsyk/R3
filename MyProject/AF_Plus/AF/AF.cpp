// AF.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAFApp

BEGIN_MESSAGE_MAP(CAFApp, CWinApp)
END_MESSAGE_MAP()


// CAFApp 构造

CAFApp::CAFApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAFApp 对象

CAFApp theApp;


// CAFApp 初始化

BOOL CAFApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
