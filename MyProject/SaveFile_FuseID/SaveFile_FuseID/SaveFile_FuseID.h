// SaveFile_FuseID.h : SaveFile_FuseID DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSaveFile_FuseIDApp
// �йش���ʵ�ֵ���Ϣ������� SaveFile_FuseID.cpp
//

class CSaveFile_FuseIDApp : public CWinApp
{
public:
	CSaveFile_FuseIDApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
