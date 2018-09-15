
// OFccmTest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OFccmControl.h"
//#include "CameraControl.h"
//#include "DeviceInterface.h"
//#include "ConfigManage.h"
#include "OFccmTest.h"
#include "OFccmTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COFccmTestApp

BEGIN_MESSAGE_MAP(COFccmTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

#include <windows.h>  
#include <imagehlp.h>  
#include <stdlib.h>  
#pragma comment(lib, "dbghelp.lib")  

inline BOOL IsDataSectionNeeded(const WCHAR* pModuleName)  
{ 
	if(pModuleName == 0)  
	{
		return FALSE;  
	}  

	WCHAR szFileName[_MAX_FNAME] = L"";  
	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);  
	if(wcsicmp(szFileName, L"ntdll") == 0)  
		return TRUE;  
	return FALSE;   
}  

inline BOOL CALLBACK MiniDumpCallback(PVOID                            pParam,   
	const PMINIDUMP_CALLBACK_INPUT   pInput,   
	PMINIDUMP_CALLBACK_OUTPUT        pOutput)  
{  
	if(pInput == 0 || pOutput == 0)  
		return FALSE;  
	switch(pInput->CallbackType)  
	{  
	case ModuleCallback:   
		if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)   
			if(!IsDataSectionNeeded(pInput->Module.FullPath))   
				pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
	case IncludeModuleCallback:  
	case IncludeThreadCallback:  
	case ThreadCallback:  
	case ThreadExCallback:  
		return TRUE;  
	default:;  
	}  
	return FALSE;  
}  
//创建Dump文件  
inline void CreateMiniDump(EXCEPTION_POINTERS* pep, LPCTSTR strFileName)  
{  
	HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  
	if((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))  
	{  
		MINIDUMP_EXCEPTION_INFORMATION mdei;  
		mdei.ThreadId           = GetCurrentThreadId();  
		mdei.ExceptionPointers  = pep;  
		mdei.ClientPointers     = FALSE;  
		MINIDUMP_CALLBACK_INFORMATION mci;  
		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;  
		mci.CallbackParam       = 0;  
		MINIDUMP_TYPE mdt       = (MINIDUMP_TYPE)0x0000ffff;  
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, &mci);  
		CloseHandle(hFile);   
	}  
}  
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI MyDummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)  
{  
	return NULL;  
}  

BOOL PreventSetUnhandledExceptionFilter()  
{  
	HMODULE hKernel32 = LoadLibrary(_T("kernel32.dll"));  
	if (hKernel32 ==   NULL)  
		return FALSE;  
	void *pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");  
	if(pOrgEntry == NULL)  
		return FALSE;  
	unsigned char newJump[ 100 ];  
	DWORD dwOrgEntryAddr = (DWORD) pOrgEntry;  
	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far  
	void *pNewFunc = &MyDummySetUnhandledExceptionFilter;  
	DWORD dwNewEntryAddr = (DWORD) pNewFunc;  
	DWORD dwRelativeAddr = dwNewEntryAddr -  dwOrgEntryAddr;  
	newJump[ 0 ] = 0xE9;  // JMP absolute  
	memcpy(&newJump[ 1 ], &dwRelativeAddr, sizeof(pNewFunc));  
	SIZE_T bytesWritten;  
	BOOL bRet = WriteProcessMemory(GetCurrentProcess(),    pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);  
	return bRet;  
}  


LONG WINAPI UnhandledExceptionFilterEx(struct _EXCEPTION_POINTERS *pException)  
{  
	TCHAR szMbsFile[MAX_PATH] = { 0 };  
	::GetModuleFileName(NULL, szMbsFile, MAX_PATH);  
	TCHAR* pFind = _tcsrchr(szMbsFile, '\\');  
	if(pFind)  
	{  
		*(pFind+1) = 0;  
		SYSTEMTIME time;
		GetLocalTime(&time);
		TCHAR fileName[MAX_PATH] = {0};
		swprintf_s(fileName,512,_T("Minidump_%04d%02d%02d_%02d%02d%02d.dmp"),time.wYear,time.wMonth,
			time.wDay,time.wHour,time.wMinute,time.wSecond);
		_tcscat(szMbsFile, fileName);  
		CreateMiniDump(pException,szMbsFile);  
	}  
	// TODO: MiniDumpWriteDump  
	FatalAppExit(-1,  _T("Fatal Error"));  
	return EXCEPTION_CONTINUE_SEARCH;  
}  



//运行异常处理  
void RunCrashHandler()  
{  
	SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);  
	PreventSetUnhandledExceptionFilter();  
} 
// COFccmTestApp 构造

COFccmTestApp::COFccmTestApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COFccmTestApp 对象

COFccmTestApp theApp;


// COFccmTestApp 初始化

BOOL COFccmTestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxInitRichEdit2();

	if (!AfxSocketInit())
	{
		//AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	bufManager.clear();
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;
	 

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序")); 
//	RunCrashHandler();//运行异常处理 
	pCcmCtrl = NULL;
	COFccmTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	SAFE_DELETE(pCcmCtrl);
	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

OFccmControl* pCcmCtrl = NULL;
int isCreateCtrl = 0;
vector<BufInfo> bufManager;
//ConfigManage* pConfig;
//CDeviceInterface* pDevice;
//CCameraControl* pCamera;

 