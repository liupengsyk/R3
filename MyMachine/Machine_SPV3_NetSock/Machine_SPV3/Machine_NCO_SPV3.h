#pragma once
#include "ccmBaseInterface.h"
#include "SettingDlg.h"
#include "MachineInterfaceLib.h"
#include "ApiSocket.h" 
#include <deque>

using namespace std;
#define CAM_TEST WM_USER + 6000
#define CAM_HANDLE WM_USER + 6001
#define CAM_SOCKET_MSG WM_USER + 6002
#define SOCKET_INIT WM_USER + 6003
 
 
struct MSG_PROTOCAL
{
	CString Receive;
	CString SendPass;
	BOOL SendEn;
	CString SendFail;
	int Reserved;
};

struct TEST_SET
{  
	CString strTestItems;
	vector<int> vecTestItemID; 
	int count;
};
 struct StationWork  //一个测试站包含每个camera的状态
{
	TEST_SET m_TestSet[2];
	MSG_PROTOCAL msgProtocal[2];
	CString name;
	CString Type;
};

class Machine_NCO_SPV3: public MachineInterface
{
public:
	Machine_NCO_SPV3(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~Machine_NCO_SPV3(void);

	int InitDevice() ;	 

	int LoadSetting();
	int SaveSetting(); 
	 
	int ServerPort;	
	BOOL bPass[2];

	ApiSocket_tcpServer * pTcpServer;
	CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_csSend;

	SettingDlg* pDlg;
	int  OnCtrlKeyNum();
	//int CamID; 
		 
public:
	void InitSocket();
	HWND hMainHwnd; 
	 
	HANDLE hControlerPro;

	BOOL isPass(int CamID);
	 
	int nMode;//一次发送一次接收
	
public:
	
	StationWork DlgStationWorkSetting;  //Dlg Setting Set
	CString HandleRevcive;
	CString HandleSend;
	vector<CString> stationNameVec;
	void UpdateStationNameVec();
		
	int station_Max;
	CString SettingFileName;

	vector<CString> machineParas;//通过遍历文件夹得到

public:
	void SetDlgStationWork(StationWork currentStationWork);
	  
	vector<StationWork> vecStationWork; 
	  
	static UINT WorkStationContoler(void* param);
	int WorkStationContolerID;
	int WorkStationContoler(); 
	  
	int TestCam(int CamID);

	static UINT WorkStation_Cam0(void* param);
 	static UINT WorkStation_Cam1(void* param); 
	HANDLE hWorkStation_Cam[2];
	
	void SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist);
 
	CString currentPath;
	void SetCurrentStation(StationWork currentStationWork); 
	int PostMsgControler(int CamID,int iMsg);	

private:
	
	StationWork currentStationWork;
	BOOL bControlThreadOn; 

	BOOL TestStation(int CamID);

	CString m_cfgName;
	int SendMsg(CString Msg,int CamID);
	int SendCamResult(int CamID,BOOL bResult);
	CString SettingPath;
	
	CString InitFilePath;
	HANDLE Mode0_Cam1;
	BOOL Cam1Result;
private:
	 
	 void HandleBack();	
	 BOOL error_en;
	 
public:
	virtual int CameraReturnResult(int CamID,BOOL bReturnResult,int error_type=0);
	CString ipAddr;
private:
	BOOL bOpenSended;

public:
	BOOL bUseErrorCode;
	BOOL bRefreshUi; 
	BOOL false_continue;
	ErrorCodeTab m_ErrorCodeTab[2];
};