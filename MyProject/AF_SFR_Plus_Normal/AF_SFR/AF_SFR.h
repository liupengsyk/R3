// AF_SFR.h : AF_SFR DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAF_SFRApp
// �йش���ʵ�ֵ���Ϣ������� AF_SFR.cpp
//

class CAF_SFRApp : public CWinApp
{
public:
	CAF_SFRApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
