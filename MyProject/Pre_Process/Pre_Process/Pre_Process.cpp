// Pre_Process.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "Pre_Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CPre_ProcessApp

BEGIN_MESSAGE_MAP(CPre_ProcessApp, CWinApp)
END_MESSAGE_MAP()


// CPre_ProcessApp ¹¹Ôì

CPre_ProcessApp::CPre_ProcessApp()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö CPre_ProcessApp ¶ÔÏó

CPre_ProcessApp theApp;


// CPre_ProcessApp ³õÊ¼»¯

BOOL CPre_ProcessApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
