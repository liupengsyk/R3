// POG.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "POG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPOGApp

BEGIN_MESSAGE_MAP(CPOGApp, CWinApp)
END_MESSAGE_MAP()


// CPOGApp ����

CPOGApp::CPOGApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CPOGApp ����

CPOGApp theApp;


// CPOGApp ��ʼ��

BOOL CPOGApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
