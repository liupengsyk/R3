// Shading49.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Shading49.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CShading49App

BEGIN_MESSAGE_MAP(CShading49App, CWinApp)
END_MESSAGE_MAP()


// CShading49App 构造

CShading49App::CShading49App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CShading49App 对象

CShading49App theApp;


// CShading49App 初始化

BOOL CShading49App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
