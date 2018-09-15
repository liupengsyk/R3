// RI.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "RI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRIApp

BEGIN_MESSAGE_MAP(CRIApp, CWinApp)
END_MESSAGE_MAP()


// CRIApp 构造

CRIApp::CRIApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CRIApp 对象

CRIApp theApp;


// CRIApp 初始化

BOOL CRIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
