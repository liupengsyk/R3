// POD.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "POD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPODApp

BEGIN_MESSAGE_MAP(CPODApp, CWinApp)
END_MESSAGE_MAP()


// CPODApp 构造

CPODApp::CPODApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPODApp 对象

CPODApp theApp;


// CPODApp 初始化

BOOL CPODApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
