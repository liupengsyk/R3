// HWBlemish.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HWBlemish.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHWBlemishApp

BEGIN_MESSAGE_MAP(CHWBlemishApp, CWinApp)
END_MESSAGE_MAP()


// CHWBlemishApp ����

CHWBlemishApp::CHWBlemishApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHWBlemishApp ����

CHWBlemishApp theApp;


// CHWBlemishApp ��ʼ��

BOOL CHWBlemishApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
