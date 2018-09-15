// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0B75C281_2AD0_4C10_949E_DA316A9DC5C4__INCLUDED_)
#define AFX_STDAFX_H__0B75C281_2AD0_4C10_949E_DA316A9DC5C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

// #ifndef _AFX_NO_DAO_SUPPORT
// #include <afxdao.h>			// MFC DAO database classes
// #endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls]

#include "Resource.h"
#endif // _AFX_NO_AFXCMN_SUPPORT
//ADO SUPPORT
#define DBNAME TESTDB
#define DBUSER sa
#define DBPWD  liangwan100zl

#include <icrsint.h>
#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","LiuEOF")
//////////////////////////////////////////////////////////WEB_SERVICE
#include <stdio.h>
#import "msxml4.dll" 

// #import "C:\Program Files\Common Files\MSSoap\Binaries\mssoap30.dll" \
// 	exclude("IStream", "IErrorInfo", "ISequentialStream", "_LARGE_INTEGER", \
// "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")

#import "C:\Program Files (x86)\Common Files\MSSoap\Binaries\MSSOAP30.DLL" \
	exclude("IStream", "IErrorInfo", "ISequentialStream", "_LARGE_INTEGER", \
"_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")

using namespace MSXML2;
using namespace MSSOAPLib30;
//////////////////////////////////////////////////////////////////////END


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0B75C281_2AD0_4C10_949E_DA316A9DC5C4__INCLUDED_)
