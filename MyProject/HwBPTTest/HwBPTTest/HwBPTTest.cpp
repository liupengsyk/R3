// HwBPTTest.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "HwBPTTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHwBPTTestApp

BEGIN_MESSAGE_MAP(CHwBPTTestApp, CWinApp)
END_MESSAGE_MAP()


// CHwBPTTestApp ����

CHwBPTTestApp::CHwBPTTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CHwBPTTestApp ����

CHwBPTTestApp theApp;


// CHwBPTTestApp ��ʼ��

BOOL CHwBPTTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
