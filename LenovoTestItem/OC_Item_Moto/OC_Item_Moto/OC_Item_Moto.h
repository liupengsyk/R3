// OC_Item_Moto.h : OC_Item_Moto DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COC_Item_MotoApp
// �йش���ʵ�ֵ���Ϣ������� OC_Item_Moto.cpp
//

class COC_Item_MotoApp : public CWinApp
{
public:
	COC_Item_MotoApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
