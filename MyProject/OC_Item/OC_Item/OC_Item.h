// OC_Item.h : OC_Item DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COC_ItemApp
// �йش���ʵ�ֵ���Ϣ������� OC_Item.cpp
//

class COC_ItemApp : public CWinApp
{
public:
	COC_ItemApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
