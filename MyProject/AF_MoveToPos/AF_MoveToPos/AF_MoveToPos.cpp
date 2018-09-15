// AF_MoveToPos.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AF_MoveToPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAF_MoveToPosApp

BEGIN_MESSAGE_MAP(CAF_MoveToPosApp, CWinApp)
END_MESSAGE_MAP()


// CAF_MoveToPosApp 构造

CAF_MoveToPosApp::CAF_MoveToPosApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAF_MoveToPosApp 对象

CAF_MoveToPosApp theApp;


// CAF_MoveToPosApp 初始化

BOOL CAF_MoveToPosApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
