// DualAA.h : DualAA DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDualAAApp
// �йش���ʵ�ֵ���Ϣ������� DualAA.cpp
//

class CDualAAApp : public CWinApp
{
public:
	CDualAAApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
