// MesCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MesCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMesCheckApp

BEGIN_MESSAGE_MAP(CMesCheckApp, CWinApp)
END_MESSAGE_MAP()


// CMesCheckApp ����

CMesCheckApp::CMesCheckApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMesCheckApp ����

CMesCheckApp theApp;


// CMesCheckApp ��ʼ��

BOOL CMesCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
