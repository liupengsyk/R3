// OC_Item_Moto_Moto.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "OC_Item_Moto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COC_Item_Moto_MotoApp

BEGIN_MESSAGE_MAP(COC_Item_MotoApp, CWinApp)
END_MESSAGE_MAP()


// COC_Item_Moto_MotoApp 构造

COC_Item_MotoApp::COC_Item_MotoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COC_Item_Moto_MotoApp 对象

COC_Item_MotoApp theApp;


// COC_Item_Moto_MotoApp 初始化

BOOL COC_Item_MotoApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
