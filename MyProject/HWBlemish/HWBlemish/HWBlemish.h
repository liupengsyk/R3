// HWBlemish.h : HWBlemish DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHWBlemishApp
// �йش���ʵ�ֵ���Ϣ������� HWBlemish.cpp
//

class CHWBlemishApp : public CWinApp
{
public:
	CHWBlemishApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
