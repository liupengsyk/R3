// SaveImg.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SaveImg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSaveImgApp

BEGIN_MESSAGE_MAP(CSaveImgApp, CWinApp)
END_MESSAGE_MAP()


// CSaveImgApp ����

CSaveImgApp::CSaveImgApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSaveImgApp ����

CSaveImgApp theApp;


// CSaveImgApp ��ʼ��

BOOL CSaveImgApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
