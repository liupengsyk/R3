// POD.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "POD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPODApp

BEGIN_MESSAGE_MAP(CPODApp, CWinApp)
END_MESSAGE_MAP()


// CPODApp ����

CPODApp::CPODApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPODApp ����

CPODApp theApp;


// CPODApp ��ʼ��

BOOL CPODApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
