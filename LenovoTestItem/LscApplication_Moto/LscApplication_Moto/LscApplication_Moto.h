// LscApplication_Moto.h : LscApplication_Moto DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLscApplication_MotoApp
// �йش���ʵ�ֵ���Ϣ������� LscApplication_Moto.cpp
//

class CLscApplication_MotoApp : public CWinApp
{
public:
	CLscApplication_MotoApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
