// [!output PROJECT_NAME].h : [!output PROJECT_NAME] DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C[!output PROJECT_NAME]App
// �йش���ʵ�ֵ���Ϣ������� [!output PROJECT_NAME].cpp
//

class C[!output PROJECT_NAME]App : public CWinApp
{
public:
	C[!output PROJECT_NAME]App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
