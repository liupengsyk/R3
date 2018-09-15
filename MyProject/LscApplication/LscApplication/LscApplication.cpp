// LscApplication.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LscApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CLscApplicationApp

BEGIN_MESSAGE_MAP(CLscApplicationApp, CWinApp)
END_MESSAGE_MAP()


// CLscApplicationApp 构造

CLscApplicationApp::CLscApplicationApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLscApplicationApp 对象

CLscApplicationApp theApp;


// CLscApplicationApp 初始化

BOOL CLscApplicationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
