// RedSpotTest.h : RedSpotTest DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAEApp
// �йش���ʵ�ֵ���Ϣ������� AE.cpp
//

class RedSpotTest : public CWinApp
{
public:
	RedSpotTest();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
