// OC_Item.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "OC_Item.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COC_ItemApp

BEGIN_MESSAGE_MAP(COC_ItemApp, CWinApp)
END_MESSAGE_MAP()


// COC_ItemApp ����

COC_ItemApp::COC_ItemApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COC_ItemApp ����

COC_ItemApp theApp;


// COC_ItemApp ��ʼ��

BOOL COC_ItemApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
