// AFCalibration.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "AFCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CAFCalibrationApp

BEGIN_MESSAGE_MAP(CAFCalibrationApp, CWinApp)
END_MESSAGE_MAP()


// CAFCalibrationApp ����

CAFCalibrationApp::CAFCalibrationApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAFCalibrationApp ����

CAFCalibrationApp theApp;


// CAFCalibrationApp ��ʼ��

BOOL CAFCalibrationApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
