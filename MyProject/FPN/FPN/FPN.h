// FPN.h : FPN DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFPNApp
// �йش���ʵ�ֵ���Ϣ������� FPN.cpp
//

class CFPNApp : public CWinApp
{
public:
	CFPNApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
