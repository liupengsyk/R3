// SetLEDChannel.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SetLEDChannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSetLEDChannelApp

BEGIN_MESSAGE_MAP(CSetLEDChannelApp, CWinApp)
END_MESSAGE_MAP()


// CSetLEDChannelApp 构造

CSetLEDChannelApp::CSetLEDChannelApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSetLEDChannelApp 对象

CSetLEDChannelApp theApp;


// CSetLEDChannelApp 初始化

BOOL CSetLEDChannelApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
