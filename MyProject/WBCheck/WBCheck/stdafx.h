// stdafx.h : ±ê×¼ÏµÍ³°üº¬ÎÄ¼þµÄ°üº¬ÎÄ¼þ£¬
// »òÊÇ¾­³£Ê¹ÓÃµ«²»³£¸ü¸ÄµÄ
// ÌØ¶¨ÓÚÏîÄ¿µÄ°üº¬ÎÄ¼þ

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ´Ó Windows Í·ÖÐÅÅ³ý¼«ÉÙÊ¹ÓÃµÄ×ÊÁÏ
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // Ä³Ð© CString ¹¹Ôìº¯Êý½«ÊÇÏÔÊ½µÄ

#include <afxwin.h>         // MFC ºËÐÄ×é¼þºÍ±ê×¼×é¼þ
#include <afxext.h>         // MFC À©Õ¹

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE Àà
#include <afxodlgs.h>       // MFC OLE ¶Ô»°¿òÀà
#include <afxdisp.h>        // MFC ×Ô¶¯»¯Àà
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC Êý¾Ý¿âÀà
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO Êý¾Ý¿âÀà
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC ¶Ô Internet Explorer 4 ¹«¹²¿Ø¼þµÄÖ§³Ö
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC ¶Ô Windows ¹«¹²¿Ø¼þµÄÖ§³Ö
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>


