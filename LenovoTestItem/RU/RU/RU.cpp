// RU.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRUApp

BEGIN_MESSAGE_MAP(CRUApp, CWinApp)
END_MESSAGE_MAP()


// CRUApp 构造

CRUApp::CRUApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRUApp 对象

CRUApp theApp;


// CRUApp 初始化

BOOL CRUApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
