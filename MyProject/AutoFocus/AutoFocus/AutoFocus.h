// AutoFocus.h : AutoFocus DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAutoFocusApp
// �йش���ʵ�ֵ���Ϣ������� AutoFocus.cpp
//

class CAutoFocusApp : public CWinApp
{
public:
	CAutoFocusApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
