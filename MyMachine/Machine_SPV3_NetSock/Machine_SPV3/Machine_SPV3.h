// Machine_SPV3.h : Machine_SPV3 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMachine_SPV3App
// �йش���ʵ�ֵ���Ϣ������� Machine_SPV3.cpp
//

class CMachine_SPV3App : public CWinApp
{
public:
	CMachine_SPV3App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
