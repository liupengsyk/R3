// GetDNPIndex.h : GetDNPIndex DLL µÄÖ÷Í·ÎÄ¼þ
//

#pragma once

#ifndef __AFXWIN_H__
	#error "ÔÚ°üº¬´ËÎÄ¼þÖ®Ç°°üº¬¡°stdafx.h¡±ÒÔÉú³É PCH ÎÄ¼þ"
#endif

#include "resource.h"		// Ö÷·ûºÅ


// CGetDNPIndexApp
// ÓÐ¹Ø´ËÀàÊµÏÖµÄÐÅÏ¢£¬Çë²ÎÔÄ GetDNPIndex.cpp
//

class CGetDNPIndexApp : public CWinApp
{
public:
	CGetDNPIndexApp();

// ÖØÐ´
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
