// AFCalibration.h : AFCalibration DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAFCalibrationApp
// �йش���ʵ�ֵ���Ϣ������� AFCalibration.cpp
//

class CAFCalibrationApp : public CWinApp
{
public:
	CAFCalibrationApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
