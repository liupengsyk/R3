// RU.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRUApp

BEGIN_MESSAGE_MAP(CRUApp, CWinApp)
END_MESSAGE_MAP()


// CRUApp ����

CRUApp::CRUApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRUApp ����

CRUApp theApp;


// CRUApp ��ʼ��

BOOL CRUApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
