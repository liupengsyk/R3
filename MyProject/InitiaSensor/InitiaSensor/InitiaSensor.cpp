// InitiaSensor.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "InitiaSensor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CInitiaSensorApp

BEGIN_MESSAGE_MAP(CInitiaSensorApp, CWinApp)
END_MESSAGE_MAP()


// CInitiaSensorApp ¹¹Ôì

CInitiaSensorApp::CInitiaSensorApp()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö CInitiaSensorApp ¶ÔÏó

CInitiaSensorApp theApp;


// CInitiaSensorApp ³õÊ¼»¯

BOOL CInitiaSensorApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
