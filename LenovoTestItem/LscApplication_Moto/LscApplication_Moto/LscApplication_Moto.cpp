// LscApplication_Moto.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "LscApplication_Moto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CLscApplication_MotoApp

BEGIN_MESSAGE_MAP(CLscApplication_MotoApp, CWinApp)
END_MESSAGE_MAP()


// CLscApplicationApp ����

CLscApplication_MotoApp::CLscApplication_MotoApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLscApplication_MotoApp ����

CLscApplication_MotoApp theApp;


// CLscApplication_MotoApp ��ʼ��

BOOL CLscApplication_MotoApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
