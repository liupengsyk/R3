// RU.h : RU DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRUApp
// �йش���ʵ�ֵ���Ϣ������� RU.cpp
//

class CRUApp : public CWinApp
{
public:
	CRUApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
