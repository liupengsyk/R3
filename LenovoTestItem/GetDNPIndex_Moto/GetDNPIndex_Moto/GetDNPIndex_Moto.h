// GetDNPIndex_Moto.h : GetDNPIndex_Moto DLL µÄÖ÷Í·ÎÄ¼þ
//

#pragma once

#ifndef __AFXWIN_H__
	#error "ÔÚ°üº¬´ËÎÄ¼þÖ®Ç°°üº¬¡°stdafx.h¡±ÒÔÉú³É PCH ÎÄ¼þ"
#endif

#include "resource.h"		// Ö÷·ûºÅ


// CGetDNPIndex_MotoApp
// ÓÐ¹Ø´ËÀàÊµÏÖµÄÐÅÏ¢£¬Çë²ÎÔÄ GetDNPIndex_Moto.cpp
//

class CGetDNPIndex_MotoApp : public CWinApp
{
public:
	CGetDNPIndex_MotoApp();

// ÖØÐ´
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
