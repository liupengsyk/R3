// AF.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAFApp

BEGIN_MESSAGE_MAP(CAFApp, CWinApp)
END_MESSAGE_MAP()


// CAFApp ����

CAFApp::CAFApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAFApp ����

CAFApp theApp;


// CAFApp ��ʼ��

BOOL CAFApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
