// POG.h : POG DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPOGApp
// �йش���ʵ�ֵ���Ϣ������� POG.cpp
//

class CPOGApp : public CWinApp
{
public:
	CPOGApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
