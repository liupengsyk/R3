#include "StdAfx.h"
#include "LoggerManage.h"

logTab LoggerManage::sFontInfo0[10240];
 int LoggerManage::index0 = 0;

 logTab LoggerManage::sFontInfo1[10240];
 int LoggerManage::index1 = 0;	 

LoggerManage::LoggerManage(HWND log)
{
	LogHwnd = log;
	InitializeCriticalSection(&m_cs);
}


LoggerManage::~LoggerManage(void)
{
	DeleteCriticalSection(&m_cs);
}

void LoggerManage::Addlog(CString info,COLORREF color,int yHeight,int CamID)
{ 	
	//CString str;
    EnterCriticalSection(&m_cs);
	switch(CamID)
	{
	case 0:
		{
			//str.Format(_T("%d:%s"),index0,info);

			sFontInfo0[index0].logstr =  info;
			sFontInfo0[index0].height = yHeight;
			sFontInfo0[index0].color = color;

			WPARAM wpara = reinterpret_cast<WPARAM>(&sFontInfo0[index0]);
			TRY
			{
				if(LogHwnd!=NULL)
				{
					PostMessage(LogHwnd,WM_ADDLOG,wpara,NULL);
					Sleep(10); 
				}
				index0++;
				if (index0 >= 5000) index0 = 0;

			}
			CATCH(CException,e)
			{

			}
			END_CATCH 
		}
		break;
	case 1:
		{
			//str.Format(_T("log%d:%s"),index1,info);

			sFontInfo1[index1].logstr =  info;
			sFontInfo1[index1].height = yHeight;
			sFontInfo1[index1].color = color;

			WPARAM wpara = reinterpret_cast<WPARAM>(&sFontInfo1[index1]);
			TRY
			{
				if(LogHwnd!=NULL)
				{
					PostMessage(LogHwnd,WM_ADDLOG,wpara,NULL);
					Sleep(10); 
				}
				index1++;
				if (index1 >= 5000) index1 = 0;
				 
			}
			CATCH(CException,e)
			{

			}
			END_CATCH 
		}
		break;
	 
	default:
		break;
	}
    LeaveCriticalSection(&m_cs);

}
void LoggerManage::ClearLog()
{ 
	if (LogHwnd == NULL)   //ÅÐ¶ÏhanderÊÇ·ñÎª¿Õ
	{
		return;
	}
	SendMessage(LogHwnd,WM_CLEARLOG,NULL,NULL);	 
}