// HwBPTTest.h : HwBPTTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHwBPTTestApp
// �йش���ʵ�ֵ���Ϣ������� HwBPTTest.cpp
//

class CHwBPTTestApp : public CWinApp
{
public:
	CHwBPTTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
