// LscApplication.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "LscApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CLscApplicationApp

BEGIN_MESSAGE_MAP(CLscApplicationApp, CWinApp)
END_MESSAGE_MAP()


// CLscApplicationApp ����

CLscApplicationApp::CLscApplicationApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLscApplicationApp ����

CLscApplicationApp theApp;


// CLscApplicationApp ��ʼ��

BOOL CLscApplicationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
