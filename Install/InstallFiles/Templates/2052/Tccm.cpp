// [!output PROJECT_NAME].cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// C[!output PROJECT_NAME]App

BEGIN_MESSAGE_MAP(C[!output PROJECT_NAME]App, CWinApp)
END_MESSAGE_MAP()


// C[!output PROJECT_NAME]App ����

C[!output PROJECT_NAME]App::C[!output PROJECT_NAME]App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� C[!output PROJECT_NAME]App ����

C[!output PROJECT_NAME]App theApp;


// C[!output PROJECT_NAME]App ��ʼ��

BOOL C[!output PROJECT_NAME]App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
