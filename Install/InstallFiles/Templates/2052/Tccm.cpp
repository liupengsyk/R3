// [!output PROJECT_NAME].cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// C[!output PROJECT_NAME]App

BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]App, CWinApp)
END_MESSAGE_MAP()


// C[!output PROJECT_NAME]App 构造

C[!output PROJECT_NAME]App::C[!output PROJECT_NAME]App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 C[!output PROJECT_NAME]App 对象

C[!output PROJECT_NAME]App theApp;


// C[!output PROJECT_NAME]App 初始化

BOOL C[!output PROJECT_NAME]App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
