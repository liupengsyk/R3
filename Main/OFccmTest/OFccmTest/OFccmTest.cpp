
// OFccmTest.cpp : ����Ӧ�ó��������Ϊ��
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
//����Dump�ļ�  
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



//�����쳣����  
void RunCrashHandler()  
{  
	SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);  
	PreventSetUnhandledExceptionFilter();  
} 
// COFccmTestApp ����

COFccmTestApp::COFccmTestApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COFccmTestApp ����

COFccmTestApp theApp;


// COFccmTestApp ��ʼ��

BOOL COFccmTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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
	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;
	 

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���")); 
//	RunCrashHandler();//�����쳣���� 
	pCcmCtrl = NULL;
	COFccmTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	SAFE_DELETE(pCcmCtrl);
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

OFccmControl* pCcmCtrl = NULL;
int isCreateCtrl = 0;
vector<BufInfo> bufManager;
//ConfigManage* pConfig;
//CDeviceInterface* pDevice;
//CCameraControl* pCamera;

 