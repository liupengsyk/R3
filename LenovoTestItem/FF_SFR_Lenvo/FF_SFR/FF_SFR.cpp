// FF_SFR.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FF_SFR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFF_SFRApp

BEGIN_MESSAGE_MAP(CFF_SFRApp, CWinApp)
END_MESSAGE_MAP()


// CFF_SFRApp ����

CFF_SFRApp::CFF_SFRApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFF_SFRApp ����

CFF_SFRApp theApp;


// CFF_SFRApp ��ʼ��

BOOL CFF_SFRApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
