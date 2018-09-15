// SRNTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SRNTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSRNTestApp

BEGIN_MESSAGE_MAP(CSRNTestApp, CWinApp)
END_MESSAGE_MAP()


// CSRNTestApp 构造

CSRNTestApp::CSRNTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSRNTestApp 对象

CSRNTestApp theApp;


// CSRNTestApp 初始化

BOOL CSRNTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
