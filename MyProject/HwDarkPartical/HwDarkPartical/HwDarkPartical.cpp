// HwDarkPartical.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HwDarkPartical.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHwDarkParticalApp

BEGIN_MESSAGE_MAP(CHwDarkParticalApp, CWinApp)
END_MESSAGE_MAP()


// CHwDarkParticalApp ����

CHwDarkParticalApp::CHwDarkParticalApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHwDarkParticalApp ����

CHwDarkParticalApp theApp;


// CHwDarkParticalApp ��ʼ��

BOOL CHwDarkParticalApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
