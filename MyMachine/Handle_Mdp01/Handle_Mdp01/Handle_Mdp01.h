
// Handle_Mdp01.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once
#include "Handle_Mdp01Dlg.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号




// CHandle_Mdp01App: 
// 有关此类的实现，请参阅 Handle_Mdp01.cpp
//

class CHandle_Mdp01App : public CWinApp
{
public:
	CHandle_Mdp01App();

// 重写
public:
	virtual BOOL InitInstance();
	

// 实现

	DECLARE_MESSAGE_MAP()
};
extern CHandle_Mdp01Dlg* pDlgS;
extern CHandle_Mdp01App theApp;

