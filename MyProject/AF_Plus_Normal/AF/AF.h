// AF.h : AF DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAFApp
// �йش���ʵ�ֵ���Ϣ������� AF.cpp
//

class CAFApp : public CWinApp
{
public:
	CAFApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
