// CaliOk.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CaliOk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CCaliOkApp

BEGIN_MESSAGE_MAP(CCaliOkApp, CWinApp)
END_MESSAGE_MAP()


// CCaliOkApp 构造

CCaliOkApp::CCaliOkApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCaliOkApp 对象

CCaliOkApp theApp;


// CCaliOkApp 初始化

BOOL CCaliOkApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
