// POD.h : POD DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPODApp
// �йش���ʵ�ֵ���Ϣ������� POD.cpp
//

class CPODApp : public CWinApp
{
public:
	CPODApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
