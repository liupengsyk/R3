// RI.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRIApp

BEGIN_MESSAGE_MAP(CRIApp, CWinApp)
END_MESSAGE_MAP()


// CRIApp ����

CRIApp::CRIApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRIApp ����

CRIApp theApp;


// CRIApp ��ʼ��

BOOL CRIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
