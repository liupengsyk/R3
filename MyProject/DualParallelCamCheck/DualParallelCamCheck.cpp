// DualParallelCamCheck.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DualParallelCamCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDualParallelCamCheckApp

BEGIN_MESSAGE_MAP(CDualParallelCamCheckApp, CWinApp)
END_MESSAGE_MAP()


// CDualParallelCamCheckApp ����

CDualParallelCamCheckApp::CDualParallelCamCheckApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDualParallelCamCheckApp ����

CDualParallelCamCheckApp theApp;


// CDualParallelCamCheckApp ��ʼ��

BOOL CDualParallelCamCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
