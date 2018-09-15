// SetFocusStatusStart.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SetFocusStatusStart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSetFocusStatusStartApp

BEGIN_MESSAGE_MAP(CSetFocusStatusStartApp, CWinApp)
END_MESSAGE_MAP()


// CSetFocusStatusStartApp 构造

CSetFocusStatusStartApp::CSetFocusStatusStartApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSetFocusStatusStartApp 对象

CSetFocusStatusStartApp theApp;


// CSetFocusStatusStartApp 初始化

BOOL CSetFocusStatusStartApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
