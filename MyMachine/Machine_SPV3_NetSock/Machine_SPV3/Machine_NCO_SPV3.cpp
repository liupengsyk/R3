// Machine_NCO_SPV3.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "Machine_NCO_SPV3.h"

BOOL gbDebug = FALSE;

#ifndef TCCM_EXP
#define TCCM_EXP	__declspec(dllexport)
#endif	/* TCCM_EXP */

extern "C" TCCM_EXP MachineInterface* CreateTestMachine(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return new Machine_NCO_SPV3(pInterface, lpszName);
}


Machine_NCO_SPV3::Machine_NCO_SPV3(ccmBaseInterface* pInterface, LPCTSTR lpszName)
{
	m_pInterface = pInterface;                      //�õ��ӿ�ָ��
	m_cfgName = CString(lpszName);
	SetName(lpszName);//���ò�����Ŀ������
	
	nMode = 2;

	bOpenSended = FALSE;
	 
	vecStationWork.clear();


	InitializeCriticalSection(&m_cs);
	InitializeCriticalSection(&m_csSend);
	pTcpServer = NULL;


	DlgStationWorkSetting.name = _T("OpenCam");
	DlgStationWorkSetting.Type = _T("0_OpenCam"); 
	for(int j=0;j<2;j++)
	{
		CString str;
		str.Format(_T("%d"),j);
		DlgStationWorkSetting.msgProtocal[j].Receive =  _T("");
		DlgStationWorkSetting.msgProtocal[j].SendFail =  _T("");
		DlgStationWorkSetting.msgProtocal[j].SendPass =  _T("");
		DlgStationWorkSetting.msgProtocal[j].SendEn =  1;
		DlgStationWorkSetting.msgProtocal[j].Reserved =  0;

		DlgStationWorkSetting.m_TestSet[j].count = 1;
		DlgStationWorkSetting.m_TestSet[j].strTestItems = _T("");
	}
	 
//	CamID = 0;
	currentPath = m_pInterface->Ctrl_GetCurrentPath();
	SetConfigPath(m_pInterface->Config_GetConfigPathName(0)); //����CONFIG·��	
	pTcpServer = new ApiSocket_tcpServer(this);
	bUseErrorCode = FALSE;
	LoadSetting();
	pDlg = new SettingDlg(NULL,this);             //���ɶԻ���
	pDlg->Create(SettingDlg::IDD,NULL);           //�����Ի���
	SetDlgHandle(pDlg->GetSafeHwnd());        //�õ��Ի�����
	pDlg->error_en = error_en;
	 
	

	bPass[0] = FALSE;
	bPass[1] = FALSE;
	  
	hControlerPro = CreateEvent(NULL, TRUE, FALSE, NULL);

	for(int i=0;i<2;i++)
	{
		hWorkStation_Cam[i] = CreateEvent(NULL, TRUE, TRUE, NULL);
	}	 
	Mode0_Cam1=CreateEvent(NULL, TRUE, TRUE, NULL);
	Cam1Result=FALSE;
	
}


Machine_NCO_SPV3::~Machine_NCO_SPV3(void)
{
	bControlThreadOn = FALSE;
	 
	CloseHandle(hControlerPro);

	CloseHandle(hWorkStation_Cam[0]);
	CloseHandle(hWorkStation_Cam[1]);
	CloseHandle(Mode0_Cam1);
	pTcpServer->Close();
	 SAFE_DELETE(pTcpServer);
	SAFE_DELETE(pDlg); 
}


int Machine_NCO_SPV3::InitDevice()
{
	hMainHwnd = GetMainHwnd();  
	// LoadSetting();
	InitSocket();  
	 
	CWinThread* WndThread = AfxBeginThread(WorkStationContoler,this);
	WorkStationContolerID = WndThread->m_nThreadID; 
	WndThread = NULL;
	return 0;
}


void Machine_NCO_SPV3::InitSocket()
{
	BOOL  bIpFinded = FALSE;
	for (int i=0;i<pDlg->ipvec.size();i++)
	{
		if (ipAddr==pDlg->ipvec[i])
		{
			bIpFinded = TRUE;
			break;
		}
	}
	//if (!bIpFinded)
	//{
	//	m_pInterface->Ctrl_Addlog(0,L"IP���ô�������������IP�����������������Ч��",COLOR_RED,200);
	//	return;
	//}
	
	//pTcpServer->ipAddr = ipAddr;
	pTcpServer->ipAddr = L"";
	
	pTcpServer->StartSocket(ServerPort);
}
 
 
UINT Machine_NCO_SPV3::WorkStationContoler(void* param)
{
	Machine_NCO_SPV3* pTccm = static_cast<Machine_NCO_SPV3*>(param);

	pTccm->WorkStationContoler();

	pTccm = NULL;

	return 1;
}

int Machine_NCO_SPV3::TestCam(int CamID)
{
	WaitForSingleObject(hWorkStation_Cam[CamID],INFINITE);
	switch(CamID)
	{
	case 0:
		if(!isPass(CamID))
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("isPass FAIL1"),COLOR_RED,200);
		}
		AfxBeginThread(WorkStation_Cam0,this);
		break;
	case 1:
		Sleep(10);
		AfxBeginThread(WorkStation_Cam1,this);
		break; 
	default:
		break;
	}
	return 1;
}

int Machine_NCO_SPV3::WorkStationContoler()
{
	MSG msg; 

	bControlThreadOn = TRUE;
	while(bControlThreadOn)
	{ 
		if(!PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			Sleep(100); 
			continue;
		}
		else
		{
			switch(msg.message)
			{
			case CAM_TEST:
				{
					int CAMid_ =msg.wParam;
					if(!isPass(CAMid_))
					{
						m_pInterface->Ctrl_Addlog(CAMid_,_T("isPass FAIL 4"),COLOR_RED,200);
					}
					TestCam(CAMid_); 
				}
				break;
			case CAM_HANDLE:
				{
					HandleBack();
				}
				break;
			case CAM_SOCKET_MSG: 
				{
					//SendMsg();
				}
				break;
			case SOCKET_INIT:
				{
					InitSocket();
				}
				break;
	 
			}
		
		} 
		//SetEvent(hControlerPro);
		
	}
	return 0;
}
 
void Machine_NCO_SPV3::HandleBack()
{
	//MsgInfo = HandleSend;
	 int UI_ID =  0;
	 m_pInterface->Ctrl_Addlog(UI_ID ,_T("Sendid 0"),COLOR_BLUE,200);
	 bPass[0] = TRUE;
	 bPass[1] = TRUE;
	 SendMsg(HandleSend,UI_ID);
	
}
int Machine_NCO_SPV3::PostMsgControler( int CamID,int iMsg)
{
	int UI_ID =  0;
	EnterCriticalSection(&m_cs);	
	PostThreadMessage(WorkStationContolerID,iMsg,CamID,0);
	Sleep(200);
	LeaveCriticalSection(&m_cs);
	return 1;
}

int Machine_NCO_SPV3::SendMsg(CString Msg,int CamID)
{   
	 int Flag = 0;
	EnterCriticalSection(&m_csSend);
	Flag =  pTcpServer->Send(Msg,CamID);
	Sleep(500);
	LeaveCriticalSection(&m_csSend); 
	return Flag;
}
 
UINT Machine_NCO_SPV3::WorkStation_Cam0(void* param)
{
	Machine_NCO_SPV3* pTccm = static_cast<Machine_NCO_SPV3*>(param);

	pTccm->TestStation(0);
	pTccm = NULL;
	return 1;
} 
UINT Machine_NCO_SPV3::WorkStation_Cam1(void* param)
{
	Machine_NCO_SPV3* pTccm = static_cast<Machine_NCO_SPV3*>(param);

	pTccm->TestStation(1);
	pTccm = NULL;
	return 1;
} 

#define  ERRORCODEMAX 15
int Machine_NCO_SPV3::LoadSetting()
{ 
	
	int CameraCnt = m_pInterface->Ctrl_GetCamType() + 1;
	//For R3 ErrorCode;

	for (int _h=0; _h<CameraCnt; _h++)
	{
		SetConfigPath(m_pInterface->Config_GetConfigPathName(_h));
		SetName(L"ErrorCode");
		m_ErrorCodeTab[_h].ErrorCode_en=ReadConfigInt(_T("ErrorCode_en"), 0);
	//	m_ErrorCodeTab[_h].ErrorCode_en=ReadConfigInt(_T("ERROR_EN"), 0);
		for(int i=0;i<ERRORCODEMAX;i++)
		{
			CString temp;
			temp.Format(_T("_%d"),i);
			m_ErrorCodeTab[_h].strSocket[i]=ReadConfigString( _T("strSocket")+temp,_T(""));
			m_ErrorCodeTab[_h].strTestID[i]=ReadConfigString(_T("strTestID")+temp,_T(""));
		}
	}
	

	//�ȵõ�setting�ļ�������
	InitFilePath = _T("Init.ini");
	SettingPath = currentPath + _T("SettingTab\\machineSetting\\");
	SetConfigPath(SettingPath + InitFilePath);
	SetName(_T("InitSet")); //name changed
	SettingFileName = ReadConfigString(_T("SettingFileName"), _T("Machine_SPV3")); 
	station_Max = ReadConfigInt(_T("station_Max"),15); 
	nMode =  ReadConfigInt(_T("nMode"), 1);

	ServerPort = ReadConfigInt(_T("SVR_PORT0"), 0); 
	ipAddr = ReadConfigString(_T("ipAddr"), L"127.0.0.1");

	error_en = ReadConfigInt(L"ERROR_EN",1);
	 
	false_continue = ReadConfigInt(L"false_continue",0);
	 
	
	SetConfigPath(SettingPath + SettingFileName );
	SetName(_T("STATION_LIST")); //name changed
	CString str;
	stationNameVec.clear();
	for(int i=0;i<station_Max;i++)
	{
		str.Format(_T("Station%d"),i);
		CString strStation = ReadConfigString(str, _T(""));
		if(strStation.GetLength()>2)
		{
			stationNameVec.push_back(strStation);
		}
	} 
	SetName(_T("HANDLE_PROTOCAL"));////name changed
	HandleRevcive = ReadConfigString(_T("HandleRevcive"),_T(""));
	HandleSend = ReadConfigString(_T("HandleSend"),_T(""));

	//��ȡÿվ��Э��
	int stationNum = stationNameVec.size();
	vecStationWork.clear();
	for(int i=0;i<stationNum;i++)
	{
		SetName(stationNameVec[i]);//name changed
		DlgStationWorkSetting.name = stationNameVec[i];
		DlgStationWorkSetting.Type = ReadConfigString(_T("Type"),_T("")); 
		for(int j=0;j<2;j++)
		{
			CString str;
			str.Format(_T("%d"),j);
			DlgStationWorkSetting.msgProtocal[j].Receive = ReadConfigString(_T("Receive")+str,_T(""));
			DlgStationWorkSetting.msgProtocal[j].SendFail = ReadConfigString(_T("SendFail")+str,_T(""));
			DlgStationWorkSetting.msgProtocal[j].SendPass = ReadConfigString(_T("SendPass")+str,_T(""));
			DlgStationWorkSetting.msgProtocal[j].SendEn = ReadConfigInt(_T("SendEn")+str,0);
			DlgStationWorkSetting.msgProtocal[j].Reserved = ReadConfigInt(_T("Reserved")+str,0);

			DlgStationWorkSetting.m_TestSet[j].count = ReadConfigInt(_T("count")+str,0);
			DlgStationWorkSetting.m_TestSet[j].strTestItems = ReadConfigString(_T("strTestItems")+str,_T(""));
			vector<CString> strID;
			SeparateString(DlgStationWorkSetting.m_TestSet[j].strTestItems, ',', &strID); 
			DlgStationWorkSetting.m_TestSet[j].vecTestItemID.clear();
			for(UINT k=0;k<strID.size();k++)
			{
				DlgStationWorkSetting.m_TestSet[j].vecTestItemID.push_back(_ttoi(strID[k]));
			}
		}
		 
		vecStationWork.push_back(DlgStationWorkSetting);
	} 

	CString dir = currentPath + _T("SettingTab\\machineSetting\\");
	//machineParas
	vector<CString> strMachineParas;
	machineParas.clear();
	m_pInterface->GetDirAllFile(dir,strMachineParas);
	for(UINT i=0;i<strMachineParas.size();i++)
	{
		if(strMachineParas[i].GetLength()>10)
		{
			machineParas.push_back(strMachineParas[i]);
		}
	}
	
	bUseErrorCode = ReadConfigInt(L"bUseErrorCode",0);
	bRefreshUi = ReadConfigInt(L"bRefreshUi",0);
	return MachineInterface::LoadSetting();
}


int Machine_NCO_SPV3::SaveSetting()
{
	if(!MachineInterface::SaveSetting()) return -1; 
	 
	SetConfigPath(SettingPath + InitFilePath);
	SetName(_T("InitSet")); //name changed

	error_en = pDlg->error_en;
	WriteConfigInt(L"ERROR_EN",error_en);
 
	WriteConfigInt(L"false_continue",false_continue);
	 
	WriteConfigString(_T("SettingFileName"), SettingFileName); 
	WriteConfigInt(_T("station_Max"),station_Max);

	WriteConfigInt(_T("SVR_PORT0"), ServerPort); 
	WriteConfigString(_T("ipAddr"), ipAddr);

	 
	WriteConfigInt(_T("nMode"), nMode);

	 if(!m_pInterface->isFileExist(SettingPath + SettingFileName))
	{
		CStdioFile file;
		file.Open(SettingPath,CFile::modeCreate);
		file.Close();
	}
	SetConfigPath(SettingPath + SettingFileName);
	SetName(_T("STATION_LIST")); //name changed ���
	Config_ClearConfig(_T("STATION_LIST"));
	
	CString str;
	int stationNum = (int)stationNameVec.size();
	for(int i=0;i<stationNum;i++)
	{
		str.Format(_T("Station%d"),i);
		WriteConfigString(str,stationNameVec[i]); 
	}

	SetName(_T("HANDLE_PROTOCAL"));////name changed
	WriteConfigString(_T("HandleRevcive"),HandleRevcive);
	WriteConfigString(_T("HandleSend"),HandleSend);

	//��ȡÿվ��Э��

	for(int i=0;i<stationNum;i++)
	{
		DlgStationWorkSetting = vecStationWork[i];
		SetName(stationNameVec[i]);//name changed
		DlgStationWorkSetting.name = stationNameVec[i];
		WriteConfigString(_T("Type"),DlgStationWorkSetting.Type); 
		for(int j=0;j<2;j++)//����Cam0 ��Cam1���趨�Ϳ����ˡ�
		{
			CString str;
			str.Format(_T("%d"),j);
			WriteConfigString(_T("Receive")+str,DlgStationWorkSetting.msgProtocal[j].Receive);
			WriteConfigString(_T("SendFail")+str,DlgStationWorkSetting.msgProtocal[j].SendFail);
			WriteConfigString(_T("SendPass")+str,DlgStationWorkSetting.msgProtocal[j].SendPass);
			WriteConfigInt(_T("SendEn")+str,DlgStationWorkSetting.msgProtocal[j].SendEn);
			WriteConfigInt(_T("Reserved")+str,DlgStationWorkSetting.msgProtocal[j].Reserved);

			WriteConfigInt(_T("count")+str,DlgStationWorkSetting.m_TestSet[j].count);
			WriteConfigString(_T("strTestItems")+str,DlgStationWorkSetting.m_TestSet[j].strTestItems); 
		} 
	} 

	WriteConfigInt(L"bUseErrorCode",bUseErrorCode);
	WriteConfigInt(L"bRefreshUi",bRefreshUi);
	return 0;
}


void Machine_NCO_SPV3::SetCurrentStation(StationWork currentStationWork)
{ 
	this->currentStationWork = currentStationWork;
}


int Machine_NCO_SPV3::SendCamResult(int CamID,BOOL bResult)
{  	
	if(bResult)
	{
		if (currentStationWork.msgProtocal[CamID].SendPass.GetLength() > 1)
		{
			m_pInterface->Ctrl_Addlog(CamID,_T("SendCamResult Sendid 1��set true"),COLOR_BLUE,200);
				bPass[CamID] = TRUE;
			SendMsg(currentStationWork.msgProtocal[CamID].SendPass,CamID); 
		}
	}
	else
	{	
	    bPass[CamID] = FALSE;
		if (m_ErrorCodeTab[CamID].ErrorCode_en)
		{
	//		AfxMessageBox(_T("01"));
			int ItemID = CamID | 0x02;   //[1]Ϊ1���ܻ�ȡLastID������Ϊ-1
			int NGItemID = m_pInterface->Ctrl_GetTestItemID(ItemID);
			CString strNGItemID;
			strNGItemID.Format(L"%d",NGItemID);
			int errorCode = 99;
			CString strErrorCode = L"99";
			if (NGItemID != -1)
			{
				for (int i=0; i<ERRORCODEMAX; i++)
				{
					vector<CString> strID;
					strID.clear();
					m_ErrorCodeTab[CamID].strTestID[i].Trim(L" ");
					SeparateString(m_ErrorCodeTab[CamID].strTestID[i], ',', &strID); 
					for (int j=0; j<strID.size(); j++)
					{
						if (strID[j] == strNGItemID)
						{
							m_ErrorCodeTab[CamID].strSocket[i].TrimLeft();
							m_ErrorCodeTab[CamID].strSocket[i].TrimRight();
							strErrorCode = m_ErrorCodeTab[CamID].strSocket[i].Right(2);
						}
					}	
				}
			}
			
			int oriErrorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
			if ((oriErrorCode>0)&&(oriErrorCode<10))
			{
				strErrorCode.Format(L"0%d",oriErrorCode);
			}
			CString returncode = currentStationWork.msgProtocal[CamID].Receive;
			returncode.Trim(L"T");
			SendMsg(L"H" + returncode + strErrorCode,CamID);
		}
		else if(bUseErrorCode)
		{		
	//		AfxMessageBox(_T("02"));
			int errorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
			CString strErrorCode;
			if(errorCode<10)
			{
				strErrorCode.Format(L"0%d",errorCode);
			}
			else
			{
				strErrorCode.Format(L"%d",errorCode);
			}
			m_pInterface->Ctrl_Addlog(CamID,_T("SendCamResult Sendid 2.0,set false"),COLOR_BLUE,200);
		 
			CString returncode = currentStationWork.msgProtocal[CamID].Receive;
			returncode.Trim(L"T");
			SendMsg(L"H" + returncode + strErrorCode,CamID);
		}
		else if (currentStationWork.msgProtocal[CamID].SendFail.GetLength() > 1)
		{
	//		AfxMessageBox(_T("03"));
	//		AfxMessageBox(currentStationWork.msgProtocal[CamID].SendFail);
			m_pInterface->Ctrl_Addlog(CamID,_T("SendCamResult Sendid 2,set false"),COLOR_BLUE,200);
			SendMsg(currentStationWork.msgProtocal[CamID].SendFail,CamID); 											
		}
		
		int lp = (bRefreshUi<<1)|CamID;
		if (!m_pInterface->isStandby(CamID) && !false_continue)
		{
	//		;
		}

		int ItemID = CamID | 0x02;   //[1]Ϊ1���ܻ�ȡLastID������Ϊ-1
		int NGItemID = m_pInterface->Ctrl_GetTestItemID(ItemID);
		if(NGItemID < 1)
			return 1;
		CString strLog;
		strLog.Format(L"NG ID = %d",NGItemID);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_RED, 200);
		TestItemInfo m_testItemInfo;
		m_pInterface->Ctrl_GetTestInfo(CamID, NGItemID, m_testItemInfo);
		CString m_testItemName = m_testItemInfo.strName;
		strLog.Format(L"NG Name = %s", m_testItemName);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_RED, 200);
		CString strErrorInfo = m_pInterface->Ctrl_GetCamErrorInfo(CamID);
		strLog.Format(L"NG Info = %s", strErrorInfo);
		m_pInterface->Ctrl_Addlog(CamID, strLog, COLOR_RED, 200);
		if (strErrorInfo == "TEST FAIL!")
		{
			strErrorInfo.Format(L"[%s]\nTEST_FAIL!",m_testItemName);
			m_pInterface->Ctrl_Addlog(CamID,strErrorInfo,COLOR_RED, 200);
			m_pInterface->Ctrl_SetCamErrorInfo(CamID, strErrorInfo);
		}
	}

	return 1;
}
int Machine_NCO_SPV3::OnCtrlKeyNum()
{
	return 0;
}
 
void Machine_NCO_SPV3::SeparateString(CString SrcString, TCHAR charSepareate, vector<CString>* strlist)
{
	if(SrcString == _T("")) return;
	strlist->clear();
	CString temp, str;
	int nStart = 0, index = 0;
	TCHAR endChar = SrcString.GetAt(SrcString.GetLength()-1); //�ж��Ƿ���','��β
	while(1)
	{
		index = SrcString.Find(charSepareate, nStart);
		if (index == -1) 
		{
			if (endChar != charSepareate)//������һ�����Ƿָ��ַ����ٰѺ����һ�α������˳�
			{
				str = SrcString.Mid(nStart,SrcString.GetLength()-nStart);

				if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
				strlist->push_back(str);
			}
			break;//������һ�����Ƿָ��ַ����˳�
		}
		str = SrcString.Mid(nStart, index-nStart);
		if (str == _T("")) str = _T("0"); //���Ϊ�գ�Ĭ�ϡ�0��,��ֹ���ݿ��ַ���Ϊ�ճ���
		strlist->push_back(str);
		nStart = index + 1;
	}
}

BOOL Machine_NCO_SPV3::isPass(int CamID)
{  
	if (nMode==0)
	{
		if(!bPass[0]) return bPass[0];
		if(!bPass[1]) return bPass[1];
	}
	return bPass[CamID];
}

 

BOOL Machine_NCO_SPV3::TestStation(int CamID)
{
	ResetEvent(hWorkStation_Cam[CamID]);
	ResetEvent(Mode0_Cam1);
	TestItemInfo mTestItemInfo;
	 int cnt = currentStationWork.m_TestSet[CamID].count;
	vector<int> TsID = currentStationWork.m_TestSet[CamID].vecTestItemID; 

	vector<CString> strType;
	SeparateString(currentStationWork.Type,'_',&strType);
	//m_pInterface->Ctrl_Addlog(CamID,currentStationWork.Type,COLOR_RED,200);
	if(strType.size()<2)
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("TYPE���ô���"),COLOR_RED,200);
		SetEvent(hWorkStation_Cam[CamID]);
		AfxEndThread(0);
		return FALSE;
	}
	int typeId = _ttoi(strType[0]);
	if(!isPass(CamID))
	{
		m_pInterface->Ctrl_Addlog(CamID,_T("isPass FAIL 0 "),COLOR_RED,200);
	}
	switch(typeId)
	{
	case 0:
		bOpenSended = FALSE;
		if(m_pInterface->isStandby(CamID))
		{
			m_pInterface->Ctrl_SetCamErrorCode(CamID|0x02,0);
			m_pInterface->Ctrl_Addlog(CamID,L"��ʼ��ErrorCode",COLOR_BLUE,200);
			CWnd::FromHandle(hMainHwnd)->PostMessage (WM_UPDATEMSG,(WPARAM)WM_SOCKET_OPENCAMERA,(LPARAM)(CamID)); 
		}
		else
			SendCamResult(CamID,TRUE);
		break;
	case 1:
		//ִ�йر�camera test Pass�� //������Ϣ��handle  
		if (isPass(CamID))
		{	
			if (currentStationWork.msgProtocal[CamID].SendPass.GetLength() > 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 3"),COLOR_BLUE,200);
				SendMsg(currentStationWork.msgProtocal[CamID].SendPass,CamID);
			}
			 
			int lp = (bRefreshUi<<1)|CamID;
			 
			if (!m_pInterface->isStandby(CamID))
			{
				CWnd::FromHandle(hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_TESTPASS,(LPARAM)(lp));
			}
			 
		}
		else
		{  
			if (m_ErrorCodeTab[CamID].ErrorCode_en)
			{
				int ItemID = CamID | 0x02;   //[1]Ϊ1���ܻ�ȡLastID������Ϊ-1
				int NGItemID = m_pInterface->Ctrl_GetTestItemID(ItemID);
				CString strNGItemID;
				strNGItemID.Format(L"%d",NGItemID);
				int errorCode = 99;
				CString strErrorCode = L"99";
				for (int i=0; i<ERRORCODEMAX; i++)
				{
					vector<CString> strID;
					strID.clear();
					m_ErrorCodeTab[CamID].strTestID[i].Trim(L" ");
					SeparateString(m_ErrorCodeTab[CamID].strTestID[i], ',', &strID); 
					for (int j=0; j<strID.size(); j++)
					{
						if (strID[j] == strNGItemID)
						{
							m_ErrorCodeTab[CamID].strSocket[i].TrimLeft();
							m_ErrorCodeTab[CamID].strSocket[i].TrimRight();
							strErrorCode = m_ErrorCodeTab[CamID].strSocket[i].Right(2);
						}
					}	
				}
				int oriErrorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
				if ((oriErrorCode>0)&&(oriErrorCode<10))
				{
					strErrorCode.Format(L"0%d",oriErrorCode);
				}
				CString returncode = currentStationWork.msgProtocal[CamID].Receive;
				returncode.Trim(L"T");
				SendMsg(L"H" + returncode + strErrorCode,CamID);
			}
			else if (bUseErrorCode)
			{
				CString strErrorCode;
				int errorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 4"),COLOR_BLUE,200);
				if(errorCode<10)
				{
					strErrorCode.Format(L"0%d",errorCode);
				}
				else
				{
					strErrorCode.Format(L"%d",errorCode);
				}
				CString returncode = currentStationWork.msgProtocal[CamID].Receive;
				returncode.Trim(L"T");
				SendMsg(L"H" + returncode + strErrorCode,CamID);  
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 4"),COLOR_BLUE,200);
				SendMsg(currentStationWork.msgProtocal[CamID].SendFail,CamID); 
			}
		 
// 			if (!bRefreshUi)           //�˴����¸�ֵ�ᵼ�º���ˢ�������00
// 			{
// 				bPass[CamID] = TRUE;
// 			} 
	//		Sleep(50);
			int lp = (bRefreshUi<<1)|CamID;
			if (!m_pInterface->isStandby(CamID))
			{
				CWnd::FromHandle(hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_TESTFAIL,(LPARAM)(lp));	
			} 
		}
		break;
	case 2:

		if (isPass(CamID))
		{	
			if (currentStationWork.msgProtocal[CamID].SendPass.GetLength() > 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 3"),COLOR_BLUE,200);
				SendMsg(currentStationWork.msgProtocal[CamID].SendPass,CamID);
			}

			int lp = (bPass[CamID]<<1)|CamID;


			CWnd::FromHandle(hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_SHOW_RESULT,(LPARAM)(lp));

		}
		else
		{  
			if (m_ErrorCodeTab[CamID].ErrorCode_en)
			{
				int ItemID = CamID | 0x02;   //[1]Ϊ1���ܻ�ȡLastID������Ϊ-1
				int NGItemID = m_pInterface->Ctrl_GetTestItemID(ItemID);
				CString strNGItemID;
				strNGItemID.Format(L"%d",NGItemID);
				int errorCode = 99;
				CString strErrorCode = L"99";
				for (int i=0; i<ERRORCODEMAX; i++)
				{
					vector<CString> strID;
					strID.clear();
					m_ErrorCodeTab[CamID].strTestID[i].Trim(L" ");
					SeparateString(m_ErrorCodeTab[CamID].strTestID[i], ',', &strID); 
					for (int j=0; j<strID.size(); j++)
					{
						if (strID[j] == strNGItemID)
						{
							m_ErrorCodeTab[CamID].strSocket[i].TrimLeft();
							m_ErrorCodeTab[CamID].strSocket[i].TrimRight();
							strErrorCode = m_ErrorCodeTab[CamID].strSocket[i].Right(2);
						}
					}	
				}
				int oriErrorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
				if ((oriErrorCode>0)&&(oriErrorCode<10))
				{
					strErrorCode.Format(L"0%d",oriErrorCode);
				}
				CString returncode = currentStationWork.msgProtocal[CamID].Receive;
				returncode.Trim(L"T");
				SendMsg(L"H" + returncode + strErrorCode,CamID);
			}
			else if (bUseErrorCode)
			{
				CString strErrorCode;
				int errorCode = m_pInterface->Ctrl_GetCamErrorCode(CamID);
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 4"),COLOR_BLUE,200);
				if(errorCode<10)
				{
					strErrorCode.Format(L"0%d",errorCode);
				}
				else
				{
					strErrorCode.Format(L"%d",errorCode);
				}
				CString returncode = currentStationWork.msgProtocal[CamID].Receive;
				returncode.Trim(L"T");
				SendMsg(L"H" + returncode + strErrorCode,CamID);  
			}
			else
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 4"),COLOR_BLUE,200);
				SendMsg(currentStationWork.msgProtocal[CamID].SendFail,CamID); 
			}
			 
			Sleep(50);
			int lp = (bPass[CamID]<<1)|CamID;
			bPass[CamID] = TRUE;
			CWnd::FromHandle(hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_SHOW_RESULT,(LPARAM)(lp));			 
		}
		break; 
	case 3:	
		{
			if ((m_pInterface->isStandby(CamID)))
			{
				SendCamResult(CamID,FALSE);
			}
			else
			{
				m_pInterface->Ctrl_SetMachineItemID(CamID, TsID);
				m_pInterface->Ctrl_Addlog(CamID,_T("Done!!"),COLOR_BLUE,200);			
			}
		}
		break;
	case 4:	
		{
			if (currentStationWork.msgProtocal[CamID].SendPass.GetLength() > 1)
			{
				m_pInterface->Ctrl_Addlog(CamID,_T("Sendid 4"),COLOR_BLUE,200);
				SendMsg(currentStationWork.msgProtocal[CamID].SendPass,CamID);
			}

			CWnd::FromHandle(hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_UPDATE,(LPARAM)(CamID));
		}
		break;
	
	default: 
		m_pInterface->Ctrl_Addlog(CamID,_T("TYPE���ô���"),COLOR_RED,200);
		break;
	} 
	SetEvent(hWorkStation_Cam[CamID]);
	AfxEndThread(0);
	return TRUE;
}

void Machine_NCO_SPV3::UpdateStationNameVec()
{
	 stationNameVec.clear();
	for(UINT i=0;i< vecStationWork.size();i++)
	{
		 stationNameVec.push_back(vecStationWork[i].name);
	}
}
 
int Machine_NCO_SPV3::CameraReturnResult(int CamID,BOOL bReturnResult,int error_type) 
{ 
	if (nMode==0)
	{
		if(CamID==1)
		{
			SetEvent(Mode0_Cam1);
			Cam1Result=bReturnResult;
		}
		else{
			WaitForSingleObject(Mode0_Cam1,INFINITE);
			if(!Cam1Result) SendCamResult(0,Cam1Result);
			else
				SendCamResult(0,bReturnResult);
			ResetEvent(Mode0_Cam1);
		}
	}
	else
	SendCamResult(CamID,bReturnResult);
	return 0; 
}

