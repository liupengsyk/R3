// OTPBurn.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "OTPBurn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COTPBurnApp

BEGIN_MESSAGE_MAP(COTPBurnApp, CWinApp)
END_MESSAGE_MAP()


// COTPBurnApp ����

COTPBurnApp::COTPBurnApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COTPBurnApp ����

COTPBurnApp theApp;


// COTPBurnApp ��ʼ��

BOOL COTPBurnApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
