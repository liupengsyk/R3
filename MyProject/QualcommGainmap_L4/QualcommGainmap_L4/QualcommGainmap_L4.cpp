// QualcommGainmap_L4.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "QualcommGainmap_L4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CQualcommGainmap_L4App

BEGIN_MESSAGE_MAP(CQualcommGainmap_L4App, CWinApp)
END_MESSAGE_MAP()


// CQualcommGainmap_L4App 构造

CQualcommGainmap_L4App::CQualcommGainmap_L4App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CQualcommGainmap_L4App 对象

CQualcommGainmap_L4App theApp;


// CQualcommGainmap_L4App 初始化

BOOL CQualcommGainmap_L4App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
