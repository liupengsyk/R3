// AF_MoveToPos.h : AF_MoveToPos DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAF_MoveToPosApp
// �йش���ʵ�ֵ���Ϣ������� AF_MoveToPos.cpp
//

class CAF_MoveToPosApp : public CWinApp
{
public:
	CAF_MoveToPosApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
