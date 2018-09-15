#pragma once
#include "OFccmDef.h"
class LoggerManage
{
public:
	LoggerManage(HWND log);
	~LoggerManage(void);

	static logTab sFontInfo0[10240];
	static int index0;

	static logTab sFontInfo1[10240];
	static int index1;	 

	 
	HWND LogHwnd;
	void Addlog(CString info,COLORREF color,int yHeight,int CamID );
	void ClearLog();
	CRITICAL_SECTION m_cs;
};

