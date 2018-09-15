// DualAA.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DualAA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDualAAApp

BEGIN_MESSAGE_MAP(CDualAAApp, CWinApp)
END_MESSAGE_MAP()


// CDualAAApp 构造

CDualAAApp::CDualAAApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDualAAApp 对象

CDualAAApp theApp;


// CDualAAApp 初始化

BOOL CDualAAApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
