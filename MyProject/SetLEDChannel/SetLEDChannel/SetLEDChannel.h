// SetLEDChannel.h : SetLEDChannel DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetLEDChannelApp
// �йش���ʵ�ֵ���Ϣ������� SetLEDChannel.cpp
//

class CSetLEDChannelApp : public CWinApp
{
public:
	CSetLEDChannelApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
