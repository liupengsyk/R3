// L476F00_OTP.h : L476F00_OTP DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CL476F00_OTPApp
// �йش���ʵ�ֵ���Ϣ������� L476F00_OTP.cpp
//

class CL476F00_OTPApp : public CWinApp
{
public:
	CL476F00_OTPApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
