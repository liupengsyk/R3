// SetFocusStatusStart.h : SetFocusStatusStart DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetFocusStatusStartApp
// �йش���ʵ�ֵ���Ϣ������� SetFocusStatusStart.cpp
//

class CSetFocusStatusStartApp : public CWinApp
{
public:
	CSetFocusStatusStartApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
