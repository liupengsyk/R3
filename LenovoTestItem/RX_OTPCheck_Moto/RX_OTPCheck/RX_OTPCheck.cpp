// RX_OTPCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RX_OTPCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CRX_OTPCheckApp

BEGIN_MESSAGE_MAP(CRX_OTPCheckApp, CWinApp)
END_MESSAGE_MAP()


// CRX_OTPCheckApp ����

CRX_OTPCheckApp::CRX_OTPCheckApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CRX_OTPCheckApp ����

CRX_OTPCheckApp theApp;


// CRX_OTPCheckApp ��ʼ��

BOOL CRX_OTPCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
