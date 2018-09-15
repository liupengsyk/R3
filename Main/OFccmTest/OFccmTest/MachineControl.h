
#pragma once

#include "MachineInterfaceLib.h"
#include "ccmBaseInterface.h"

#include <vector>
using namespace std;

//class ccmBaseInterface;
class OFccmControl;
class MachineControl
{
public:
	MachineControl(OFccmControl* pTccmControl, HWND hTestDlg,CString machinePath,CString machineName);
	~MachineControl(void);
	HWND hTestDlg;
 
	CString machineName;
	CString machinePath;
	OFccmControl* m_pTccm; 

public:	

	HMODULE hdll;
	
	void LoadMachine();

	void LoadMachineSetting();
	int SaveMachineSetting();

public:
	MachineInterface* p_machine;
 

	int CameraReturnResult(int CamID,BOOL bResult,int error_type = 0);
private:
	BOOL bCreate;

 

};