// RX_OTPCheck.h : RX_OTPCheck DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRX_OTPCheckApp
// �йش���ʵ�ֵ���Ϣ������� RX_OTPCheck.cpp
//

class CRX_OTPCheckApp : public CWinApp
{
public:
	CRX_OTPCheckApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
