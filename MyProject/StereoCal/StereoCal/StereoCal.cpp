// StereoCal.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "StereoCal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CStereoCalApp

BEGIN_MESSAGE_MAP(CStereoCalApp, CWinApp)
END_MESSAGE_MAP()


// CStereoCalApp ����

CStereoCalApp::CStereoCalApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CStereoCalApp ����

CStereoCalApp theApp;


// CStereoCalApp ��ʼ��

BOOL CStereoCalApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
