// POG.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "POG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPOGApp

BEGIN_MESSAGE_MAP(CPOGApp, CWinApp)
END_MESSAGE_MAP()


// CPOGApp 构造

CPOGApp::CPOGApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPOGApp 对象

CPOGApp theApp;


// CPOGApp 初始化

BOOL CPOGApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
