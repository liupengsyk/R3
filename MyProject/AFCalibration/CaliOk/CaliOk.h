// CaliOk.h : CaliOk DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCaliOkApp
// �йش���ʵ�ֵ���Ϣ������� CaliOk.cpp
//

class CCaliOkApp : public CWinApp
{
public:
	CCaliOkApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
