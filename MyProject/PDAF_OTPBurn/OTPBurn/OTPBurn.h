// OTPBurn.h : OTPBurn DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COTPBurnApp
// �йش���ʵ�ֵ���Ϣ������� OTPBurn.cpp
//

class COTPBurnApp : public CWinApp
{
public:
	COTPBurnApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
