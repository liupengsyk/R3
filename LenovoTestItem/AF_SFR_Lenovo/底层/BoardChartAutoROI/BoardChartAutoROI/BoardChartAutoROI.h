// BoardChartAutoROI.h : BoardChartAutoROI DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBoardChartAutoROIApp
// �йش���ʵ�ֵ���Ϣ������� BoardChartAutoROI.cpp
//

class CBoardChartAutoROIApp : public CWinApp
{
public:
	CBoardChartAutoROIApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
