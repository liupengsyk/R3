// AE.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAEApp

BEGIN_MESSAGE_MAP(CAEApp, CWinApp)
END_MESSAGE_MAP()


// CAEApp ����

CAEApp::CAEApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAEApp ����

CAEApp theApp;


// CAEApp ��ʼ��

BOOL CAEApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}
