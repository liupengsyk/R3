// SaveFile_FuseID.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SaveFile_FuseID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSaveFile_FuseIDApp

BEGIN_MESSAGE_MAP(CSaveFile_FuseIDApp, CWinApp)
END_MESSAGE_MAP()


// CSaveFile_FuseIDApp 构造

CSaveFile_FuseIDApp::CSaveFile_FuseIDApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSaveFile_FuseIDApp 对象

CSaveFile_FuseIDApp theApp;


// CSaveFile_FuseIDApp 初始化

BOOL CSaveFile_FuseIDApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
