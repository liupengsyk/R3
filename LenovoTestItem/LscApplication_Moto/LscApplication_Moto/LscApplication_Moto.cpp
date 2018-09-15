// LscApplication_Moto.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LscApplication_Moto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CLscApplication_MotoApp

BEGIN_MESSAGE_MAP(CLscApplication_MotoApp, CWinApp)
END_MESSAGE_MAP()


// CLscApplicationApp 构造

CLscApplication_MotoApp::CLscApplication_MotoApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLscApplication_MotoApp 对象

CLscApplication_MotoApp theApp;


// CLscApplication_MotoApp 初始化

BOOL CLscApplication_MotoApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
