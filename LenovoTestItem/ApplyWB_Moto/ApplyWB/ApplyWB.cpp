// ApplyWB.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "ApplyWB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CApplyWBApp

BEGIN_MESSAGE_MAP(CApplyWBApp, CWinApp)
END_MESSAGE_MAP()


// CApplyWBApp ¹¹Ôì

CApplyWBApp::CApplyWBApp()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö CApplyWBApp ¶ÔÏó

CApplyWBApp theApp;


// CApplyWBApp ³õÊ¼»¯

BOOL CApplyWBApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
