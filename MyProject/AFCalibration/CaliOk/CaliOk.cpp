// CaliOk.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "CaliOk.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CCaliOkApp

BEGIN_MESSAGE_MAP(CCaliOkApp, CWinApp)
END_MESSAGE_MAP()


// CCaliOkApp ����

CCaliOkApp::CCaliOkApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CCaliOkApp ����

CCaliOkApp theApp;


// CCaliOkApp ��ʼ��

BOOL CCaliOkApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
