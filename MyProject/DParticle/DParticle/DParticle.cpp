// DParticle.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "DParticle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CDParticleApp

BEGIN_MESSAGE_MAP(CDParticleApp, CWinApp)
END_MESSAGE_MAP()


// CDParticleApp ����

CDParticleApp::CDParticleApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CDParticleApp ����

CDParticleApp theApp;


// CDParticleApp ��ʼ��

BOOL CDParticleApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
