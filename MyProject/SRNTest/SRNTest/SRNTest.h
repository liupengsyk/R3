// SRNTest.h : SRNTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSRNTestApp
// �йش���ʵ�ֵ���Ϣ������� SRNTest.cpp
//

class CSRNTestApp : public CWinApp
{
public:
	CSRNTestApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
