// SaveFile_FuseID.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SaveFile_FuseID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSaveFile_FuseIDApp

BEGIN_MESSAGE_MAP(CSaveFile_FuseIDApp, CWinApp)
END_MESSAGE_MAP()


// CSaveFile_FuseIDApp ����

CSaveFile_FuseIDApp::CSaveFile_FuseIDApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSaveFile_FuseIDApp ����

CSaveFile_FuseIDApp theApp;


// CSaveFile_FuseIDApp ��ʼ��

BOOL CSaveFile_FuseIDApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
