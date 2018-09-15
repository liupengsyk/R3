// OC_Item.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OC_Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COC_ItemApp

BEGIN_MESSAGE_MAP(COC_ItemApp, CWinApp)
END_MESSAGE_MAP()


// COC_ItemApp 构造

COC_ItemApp::COC_ItemApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COC_ItemApp 对象

COC_ItemApp theApp;


// COC_ItemApp 初始化

BOOL COC_ItemApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
