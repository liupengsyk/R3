// DualAA.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DualAA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDualAAApp

BEGIN_MESSAGE_MAP(CDualAAApp, CWinApp)
END_MESSAGE_MAP()


// CDualAAApp ����

CDualAAApp::CDualAAApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDualAAApp ����

CDualAAApp theApp;


// CDualAAApp ��ʼ��

BOOL CDualAAApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
