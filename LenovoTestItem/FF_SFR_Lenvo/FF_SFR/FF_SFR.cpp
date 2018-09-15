// FF_SFR.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FF_SFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFF_SFRApp

BEGIN_MESSAGE_MAP(CFF_SFRApp, CWinApp)
END_MESSAGE_MAP()


// CFF_SFRApp 构造

CFF_SFRApp::CFF_SFRApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFF_SFRApp 对象

CFF_SFRApp theApp;


// CFF_SFRApp 初始化

BOOL CFF_SFRApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
