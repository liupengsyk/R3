// RI.h : RI DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRIApp
// �йش���ʵ�ֵ���Ϣ������� RI.cpp
//

class CRIApp : public CWinApp
{
public:
	CRIApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
