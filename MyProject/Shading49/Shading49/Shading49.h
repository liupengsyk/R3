// Shading49.h : Shading49 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CShading49App
// �йش���ʵ�ֵ���Ϣ������� Shading49.cpp
//

class CShading49App : public CWinApp
{
public:
	CShading49App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
