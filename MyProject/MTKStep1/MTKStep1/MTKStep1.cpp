// MTKStep1.cpp : ¶¨Òå DLL µÄ³õÊ¼»¯Àý³Ì¡£
//

#include "stdafx.h"
#include "MTKStep1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CMTKStep1App

BEGIN_MESSAGE_MAP(CMTKStep1App, CWinApp)
END_MESSAGE_MAP()


// CMTKStep1App ¹¹Ôì

CMTKStep1App::CMTKStep1App()
{
	// TODO: ÔÚ´Ë´¦Ìí¼Ó¹¹Ôì´úÂë£¬
	// ½«ËùÓÐÖØÒªµÄ³õÊ¼»¯·ÅÖÃÔÚ InitInstance ÖÐ
}


// Î¨Ò»µÄÒ»¸ö CMTKStep1App ¶ÔÏó

CMTKStep1App theApp;


// CMTKStep1App ³õÊ¼»¯

BOOL CMTKStep1App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
