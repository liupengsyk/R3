// WBCheck.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "WBCheck.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CWBCheckApp

BEGIN_MESSAGE_MAP(CWBCheckApp, CWinApp)
END_MESSAGE_MAP()


// CWBCheckApp ¹¹Ôì

CWBCheckApp::CWBCheckApp()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö CWBCheckApp ¶ÔÏó

CWBCheckApp theApp;


// CWBCheckApp ³õÊ¼»¯

BOOL CWBCheckApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
