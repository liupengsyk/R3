// MesUpdate.h : MesUpdate DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMesUpdateApp
// �йش���ʵ�ֵ���Ϣ������� MesUpdate.cpp
//

class CMesUpdateApp : public CWinApp
{
public:
	CMesUpdateApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
