// SaveImg.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SaveImg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSaveImgApp

BEGIN_MESSAGE_MAP(CSaveImgApp, CWinApp)
END_MESSAGE_MAP()


// CSaveImgApp 构造

CSaveImgApp::CSaveImgApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSaveImgApp 对象

CSaveImgApp theApp;


// CSaveImgApp 初始化

BOOL CSaveImgApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
