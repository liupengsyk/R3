// HwBPTTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "HwBPTTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHwBPTTestApp

BEGIN_MESSAGE_MAP(CHwBPTTestApp, CWinApp)
END_MESSAGE_MAP()


// CHwBPTTestApp 构造

CHwBPTTestApp::CHwBPTTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CHwBPTTestApp 对象

CHwBPTTestApp theApp;


// CHwBPTTestApp 初始化

BOOL CHwBPTTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
