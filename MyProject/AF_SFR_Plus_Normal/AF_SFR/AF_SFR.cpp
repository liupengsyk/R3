// AF_SFR.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AF_SFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAF_SFRApp

BEGIN_MESSAGE_MAP(CAF_SFRApp, CWinApp)
END_MESSAGE_MAP()


// CAF_SFRApp ����

CAF_SFRApp::CAF_SFRApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAF_SFRApp ����

CAF_SFRApp theApp;


// CAF_SFRApp ��ʼ��

BOOL CAF_SFRApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
