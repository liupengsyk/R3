// FPN.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FPN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFPNApp

BEGIN_MESSAGE_MAP(CFPNApp, CWinApp)
END_MESSAGE_MAP()


// CFPNApp ����

CFPNApp::CFPNApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFPNApp ����

CFPNApp theApp;


// CFPNApp ��ʼ��

BOOL CFPNApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
