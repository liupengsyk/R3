// SetFocusStatusStart.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SetFocusStatusStart.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSetFocusStatusStartApp

BEGIN_MESSAGE_MAP(CSetFocusStatusStartApp, CWinApp)
END_MESSAGE_MAP()


// CSetFocusStatusStartApp ����

CSetFocusStatusStartApp::CSetFocusStatusStartApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSetFocusStatusStartApp ����

CSetFocusStatusStartApp theApp;


// CSetFocusStatusStartApp ��ʼ��

BOOL CSetFocusStatusStartApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
