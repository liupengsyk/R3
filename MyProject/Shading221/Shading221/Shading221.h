// Shading221.h : Shading221 DLL µÄÖ÷Í·ÎÄ¼þ
//

#pragma once

#ifndef __AFXWIN_H__
	#error "ÔÚ°üº¬´ËÎÄ¼þÖ®Ç°°üº¬¡°stdafx.h¡±ÒÔÉú³É PCH ÎÄ¼þ"
#endif

#include "resource.h"		// Ö÷·ûºÅ


// CShading221App
// ÓÐ¹Ø´ËÀàÊµÏÖµÄÐÅÏ¢£¬Çë²ÎÔÄ Shading221.cpp
//

class CShading221App : public CWinApp
{
public:
	CShading221App();

// ÖØÐ´
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
