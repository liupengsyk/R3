// AF_SFR.h : AF_SFR DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CAF_SFRApp
// 有关此类实现的信息，请参阅 AF_SFR.cpp
//

class CAF_SFRApp : public CWinApp
{
public:
	CAF_SFRApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
