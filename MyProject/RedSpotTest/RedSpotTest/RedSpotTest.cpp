// AE.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RedSpotTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAEApp

BEGIN_MESSAGE_MAP(RedSpotTest, CWinApp)
END_MESSAGE_MAP()


// CAEApp ����

RedSpotTest::RedSpotTest()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAEApp ����

RedSpotTest theApp;


// CAEApp ��ʼ��

BOOL RedSpotTest::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
