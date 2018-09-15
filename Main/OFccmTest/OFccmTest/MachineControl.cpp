#include "StdAfx.h"
#include "MachineControl.h"
#include "OFccmTest.h"

MachineControl::MachineControl(OFccmControl* pTccmControl, HWND hTestDlg,CString machinePath,CString machineName)
{
	m_pTccm = pTccmControl; 
	this->hTestDlg = hTestDlg;
	this->machineName = machineName;
	this->machinePath = machinePath; 
	bCreate = TRUE;
}


MachineControl::~MachineControl(void)
{ 
	m_pTccm = NULL; 
	this->hTestDlg = NULL; 

	if (hdll != NULL)
		::FreeLibrary(hdll); 

	p_machine = NULL;
}

 
void MachineControl::LoadMachine()
{	
	CString DllFullPath = machinePath + machineName ;

	hdll = ::LoadLibrary(DllFullPath);
		
	if (hdll == NULL)
	{
		CString Notice;
		Notice.Format(_T("NO DLL\n   %s"), machineName);
		//AfxMessageBox(Notice);
		m_pTccm->Ctrl_Addlog(0,Notice,COLOR_RED,200);			 
		return;
	}

	//定义函数指针
	typedef MachineInterface* (*ADDPROC)(ccmBaseInterface*,LPCTSTR);
	//得到dll接口函数地址
	ADDPROC GetTestItemAddr = (ADDPROC)GetProcAddress(hdll,"CreateTestMachine");//与dll中的名字一致。
	//执行dll接口函数
	if (!GetTestItemAddr)
	{
		AfxMessageBox(_T("GET DLL ADDR FAIL"));
		return;
	}
	//得到文件Title

	ccmBaseInterface* pInterface = dynamic_cast<ccmBaseInterface*> (pCcmCtrl);

	//调用DLL接口函数
	 
	TRY
	{
		p_machine = GetTestItemAddr(pInterface,machineName);
		p_machine->SetMainHwnd(hTestDlg);
		p_machine->InitDevice();
	}
	CATCH(CException,e)
	{
		AfxMessageBox(_T("DLL FAIL ADD"));
		return;
	}
	END_CATCH
	m_pTccm->Ctrl_Addlog(0,_T("ADD ") + machineName,COLOR_GREEN,200);
	LoadMachineSetting();
	SaveMachineSetting();
	
}

void MachineControl::LoadMachineSetting()
{
	p_machine->LoadSetting();
}

int MachineControl::SaveMachineSetting()
{
   return p_machine->SaveSetting();
}

int MachineControl::CameraReturnResult(int CamID,BOOL bResult,int error_type)
{
	if (p_machine!=NULL)	 
		return p_machine->CameraReturnResult(CamID,bResult,error_type);
	else
		return 0;
}

 
 