// StereoCal.h : StereoCal DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStereoCalApp
// �йش���ʵ�ֵ���Ϣ������� StereoCal.cpp
//

class CStereoCalApp : public CWinApp
{
public:
	CStereoCalApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
