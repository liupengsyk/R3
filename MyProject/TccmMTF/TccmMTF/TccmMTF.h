// TccmMTF.h : TccmMTF DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTccmMTFApp
// �йش���ʵ�ֵ���Ϣ������� TccmMTF.cpp
//

class CTccmMTFApp : public CWinApp
{
public:
	CTccmMTFApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
