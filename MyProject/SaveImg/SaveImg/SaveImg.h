// SaveImg.h : SaveImg DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSaveImgApp
// �йش���ʵ�ֵ���Ϣ������� SaveImg.cpp
//

class CSaveImgApp : public CWinApp
{
public:
	CSaveImgApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
