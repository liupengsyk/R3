// AF_MoveToPos.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AF_MoveToPos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAF_MoveToPosApp

BEGIN_MESSAGE_MAP(CAF_MoveToPosApp, CWinApp)
END_MESSAGE_MAP()


// CAF_MoveToPosApp ����

CAF_MoveToPosApp::CAF_MoveToPosApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAF_MoveToPosApp ����

CAF_MoveToPosApp theApp;


// CAF_MoveToPosApp ��ʼ��

BOOL CAF_MoveToPosApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
