// MesUpdate.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MesUpdate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMesUpdateApp

BEGIN_MESSAGE_MAP(CMesUpdateApp, CWinApp)
END_MESSAGE_MAP()


// CMesUpdateApp ����

CMesUpdateApp::CMesUpdateApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMesUpdateApp ����

CMesUpdateApp theApp;


// CMesUpdateApp ��ʼ��

BOOL CMesUpdateApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
