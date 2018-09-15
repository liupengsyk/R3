// MsgShow.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "MsgShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMsgShowApp

BEGIN_MESSAGE_MAP(CMsgShowApp, CWinApp)
END_MESSAGE_MAP()


// CMsgShowApp 构造

CMsgShowApp::CMsgShowApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMsgShowApp 对象

CMsgShowApp theApp;


// CMsgShowApp 初始化

BOOL CMsgShowApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
