// SetLEDChannel.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "SetLEDChannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSetLEDChannelApp

BEGIN_MESSAGE_MAP(CSetLEDChannelApp, CWinApp)
END_MESSAGE_MAP()


// CSetLEDChannelApp ����

CSetLEDChannelApp::CSetLEDChannelApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CSetLEDChannelApp ����

CSetLEDChannelApp theApp;


// CSetLEDChannelApp ��ʼ��

BOOL CSetLEDChannelApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
