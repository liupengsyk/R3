// MesCheck.h : MesCheck DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMesCheckApp
// �йش���ʵ�ֵ���Ϣ������� MesCheck.cpp
//

class CMesCheckApp : public CWinApp
{
public:
	CMesCheckApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
