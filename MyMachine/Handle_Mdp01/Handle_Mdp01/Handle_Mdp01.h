
// Handle_Mdp01.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
#include "Handle_Mdp01Dlg.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������




// CHandle_Mdp01App: 
// �йش����ʵ�֣������ Handle_Mdp01.cpp
//

class CHandle_Mdp01App : public CWinApp
{
public:
	CHandle_Mdp01App();

// ��д
public:
	virtual BOOL InitInstance();
	

// ʵ��

	DECLARE_MESSAGE_MAP()
};
extern CHandle_Mdp01Dlg* pDlgS;
extern CHandle_Mdp01App theApp;

