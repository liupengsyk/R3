// DualParallelCamCheck.h : DualParallelCamCheck DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDualParallelCamCheckApp
// �йش���ʵ�ֵ���Ϣ������� DualParallelCamCheck.cpp
//

class CDualParallelCamCheckApp : public CWinApp
{
public:
	CDualParallelCamCheckApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
