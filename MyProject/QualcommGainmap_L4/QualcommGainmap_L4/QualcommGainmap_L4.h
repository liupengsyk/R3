// QualcommGainmap_L4.h : QualcommGainmap_L4 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQualcommGainmap_L4App
// �йش���ʵ�ֵ���Ϣ������� QualcommGainmap_L4.cpp
//

class CQualcommGainmap_L4App : public CWinApp
{
public:
	CQualcommGainmap_L4App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
