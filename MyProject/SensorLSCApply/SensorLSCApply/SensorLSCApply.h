// SensorLSCApply.h : SensorLSCApply DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSensorLSCApplyApp
// �йش���ʵ�ֵ���Ϣ������� SensorLSCApply.cpp
//

class CSensorLSCApplyApp : public CWinApp
{
public:
	CSensorLSCApplyApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
