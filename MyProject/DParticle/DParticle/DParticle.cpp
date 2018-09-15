// DParticle.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DParticle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDParticleApp

BEGIN_MESSAGE_MAP(CDParticleApp, CWinApp)
END_MESSAGE_MAP()


// CDParticleApp 构造

CDParticleApp::CDParticleApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CDParticleApp 对象

CDParticleApp theApp;


// CDParticleApp 初始化

BOOL CDParticleApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
