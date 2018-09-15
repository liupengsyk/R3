// SaveFile_FuseID.h : SaveFile_FuseID DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CSaveFile_FuseIDApp
// 有关此类实现的信息，请参阅 SaveFile_FuseID.cpp
//

class CSaveFile_FuseIDApp : public CWinApp
{
public:
	CSaveFile_FuseIDApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
