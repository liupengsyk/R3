// MsgShow.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MsgShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMsgShowApp

BEGIN_MESSAGE_MAP(CMsgShowApp, CWinApp)
END_MESSAGE_MAP()


// CMsgShowApp ����

CMsgShowApp::CMsgShowApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMsgShowApp ����

CMsgShowApp theApp;


// CMsgShowApp ��ʼ��

BOOL CMsgShowApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
