// FF_SFR.h : FF_SFR DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFF_SFRApp
// �йش���ʵ�ֵ���Ϣ������� FF_SFR.cpp
//

class CFF_SFRApp : public CWinApp
{
public:
	CFF_SFRApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
