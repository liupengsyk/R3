// OPCurrentTest.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "OPCurrentTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// COPCurrentTestApp

BEGIN_MESSAGE_MAP(COPCurrentTestApp, CWinApp)
END_MESSAGE_MAP()


// COPCurrentTestApp ¹¹Ôì

COPCurrentTestApp::COPCurrentTestApp()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö COPCurrentTestApp ¶ÔÏó

COPCurrentTestApp theApp;


// COPCurrentTestApp ³õÊ¼»¯

BOOL COPCurrentTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
