// DualParallelCamCheck.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DualParallelCamCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDualParallelCamCheckApp

BEGIN_MESSAGE_MAP(CDualParallelCamCheckApp, CWinApp)
END_MESSAGE_MAP()


// CDualParallelCamCheckApp 构造

CDualParallelCamCheckApp::CDualParallelCamCheckApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDualParallelCamCheckApp 对象

CDualParallelCamCheckApp theApp;


// CDualParallelCamCheckApp 初始化

BOOL CDualParallelCamCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
