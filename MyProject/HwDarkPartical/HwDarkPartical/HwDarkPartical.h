// HwDarkPartical.h : HwDarkPartical DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHwDarkParticalApp
// �йش���ʵ�ֵ���Ϣ������� HwDarkPartical.cpp
//

class CHwDarkParticalApp : public CWinApp
{
public:
	CHwDarkParticalApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
