// StereoCal.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "StereoCal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CStereoCalApp

BEGIN_MESSAGE_MAP(CStereoCalApp, CWinApp)
END_MESSAGE_MAP()


// CStereoCalApp 构造

CStereoCalApp::CStereoCalApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CStereoCalApp 对象

CStereoCalApp theApp;


// CStereoCalApp 初始化

BOOL CStereoCalApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
