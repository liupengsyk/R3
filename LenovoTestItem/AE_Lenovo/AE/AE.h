// AE.h : AE DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAEApp
// �йش���ʵ�ֵ���Ϣ������� AE.cpp
//

class CAEApp : public CWinApp
{
public:
	CAEApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
