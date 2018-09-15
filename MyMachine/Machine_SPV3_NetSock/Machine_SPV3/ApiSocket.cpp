#include "StdAfx.h"
#include "OFccmDef.h"
#include "Machine_NCO_SPV3.h"
#include "ApiSocket.h"
#include "ImageProc.h" 

enum
{
	CMD_HANDSHAKE = 0,
	CMD_TEST,
	CMD_UNKOWN,
};
bool RegSetKey(char *SubReg,char *SubKeyName,char *keyValue)
{
	HKEY hKey=0;
	char *SubRegName=new char[512];
	strcpy(SubRegName,"Software\\Ofilm\\");
	if (strlen(SubReg))
	{
		strcat(SubRegName,SubReg);
	}
	::RegOpenKeyA(HKEY_CURRENT_USER,SubRegName,&hKey);
	if (hKey==0)
	{
		::RegCreateKeyA(HKEY_CURRENT_USER,SubRegName,&hKey);
	}

	::RegSetValueExA(hKey,SubKeyName,0,REG_SZ,(CONST BYTE*)keyValue,512);
	::RegCloseKey(hKey);
	delete []SubRegName;
	SubRegName = NULL;
	return true;
}
 //ApiSocket_tcpServer::sock_;
ApiSocket_tcpServer::ApiSocket_tcpServer(Machine_NCO_SPV3* m_spv3)
{
	this->m_spv3 = m_spv3;

	//InitSock();
	isReset = FALSE;
	  
	CamType = this->m_spv3->m_pInterface->Ctrl_GetCamType() + 1;
	
	//for(int i=0;i<1;i++)
	{
		sock_.hBusy = NULL;		 
		 
		hReceiveThreadDead = CreateEvent(NULL,TRUE,TRUE,NULL);
		Sleep(10);
		 
		hSetCurrentStation = CreateEvent(NULL,TRUE,FALSE,NULL);
		Sleep(10);
		InitializeCriticalSection(&(sock_.m_cs));
	}	
	NetSockLib =   gcnew SocketNetLibProgram(); 

}

int ApiSocket_tcpServer::StartSocket(int nPort)
{
	this->nPort = nPort;
	
	//Sleep(200);
	
	sock_.bCreateSucess = TRUE;
	sock_.hBusy = CreateEvent(NULL,TRUE,FALSE,NULL);

	sock_.SendTimes = 0;
	sock_.recvinfoMsg.clear(); 
	sock_.bOnSocket = TRUE;
	  
	ResetEvent(hReceiveThreadDead);

	ResetEvent(hSetCurrentStation);
	AfxBeginThread(ReceiveThread0,this); 

	return 1;

}

ApiSocket_tcpServer::~ApiSocket_tcpServer(void)
{
	Close();
	SAFE_CLOSE_HANDLE(hReceiveThreadDead ); 

	delete NetSockLib;
	NetSockLib = NULL;
	//WSACleanup();
	m_spv3 = NULL;
}

 

UINT ApiSocket_tcpServer::ReceiveThread0(void* param)
{
	ApiSocket_tcpServer* pSrv = static_cast<ApiSocket_tcpServer*>(param);
	pSrv->Receive();
	pSrv = NULL;
	return 0;
}
 
 
int ApiSocket_tcpServer::Send(CString Msg,int CamID)
{ 
	int flag = -1;  
	char tmpBuf[1024] = {0};//"T020101";
	ImageProc::ConvertWideToChar(Msg,tmpBuf);
	
	//if(sockConn != NULL)
	EnterCriticalSection(&(sock_.m_cs));
	//m_spv3->m_pInterface->Ctrl_Addlog(CamID, _T("out:")+Msg, COLOR_BLUE, 200);
	 
	 System::String^ sendbuf = gcnew String(Msg);
	 NetSockLib->SendMsg(sendbuf); 

	 m_spv3->m_pInterface->Ctrl_Addlog(CamID, _T("out:")+Msg, COLOR_GREEN, 200);


	 sock_.SendTimes++;
	if(Msg.Find(_T("T"))!=-1)
	{
		sock_.SendTimes--;
		ResetEvent(sock_.hBusy);
	}
	else if(sock_.recvinfoMsg.size()==sock_.SendTimes)
	{
		ResetEvent(sock_.hBusy);
		sock_.recvinfoMsg.clear();
	}
	if(m_spv3->nMode==1||m_spv3->nMode==0)
	{
		ResetEvent(sock_.hBusy);
		sock_.recvinfoMsg.clear();
		sock_.SendTimes = 0;
	}
	LeaveCriticalSection(&(sock_.m_cs));

	return flag;	
} 

vector<CString> ApiSocket_tcpServer::GetIpList()
{
	vector<CString> ipList;

	int iPCnt = NetSockLib->GetIpAddrCnt();

	for (int i=0;i<iPCnt;i++)
	{
		System::String^ LocalIp =  NetSockLib->GetCurentIpAddr()[i];
		CString strIp(LocalIp);
		m_spv3->m_pInterface->Ctrl_Addlog(0, strIp, COLOR_BLUE, 200);
		ipList.push_back(strIp);
	}
	return ipList;
}

int ApiSocket_tcpServer::Receive()
{ 
	TRY 
	{
		if(Create(ipAddr,nPort) != 1)
		{
			m_spv3->m_pInterface->Ctrl_Addlog(0, _T("服务器创建失败"), COLOR_RED, 200);	
			return -1;
		}
		m_spv3->m_pInterface->Ctrl_Addlog(0, _T("GET CONNECT"), COLOR_BLUE, 200);	
		Sleep(500);

		int waits = 0;

		while(sock_.bOnSocket)
		{ 
			m_spv3->m_pInterface->Ctrl_Addlog(0, _T("WAIT MSG"), COLOR_BLUE, 200);			
			//EnterCriticalSection(&(sock_.m_cs));
			System::String^ recvBuf = NetSockLib->recvMsg();	
			//LeaveCriticalSection(&(sock_.m_cs));
			CString recMsg(recvBuf);		
			int length = recMsg.GetLength();
			m_spv3->m_pInterface->Ctrl_Addlog(0, L"接收消息:" +recMsg, COLOR_BLUE, 200);

			if (length > 6)
			{
				int nLenRunCard = 0;
				int nLenBarCode = 0;
				int nLenCmd = recMsg.Find(_T("T040100"));
				if (-1 != nLenCmd)
				{
					recMsg.Trim(_T(" "));
					CString temp = _T("");
					CString RunCard = _T("");
					CString Barcode =_T("");		

#if 0
					//temp = recMsg.Mid(recMsg.Find(_T(":"))+1);
					recMsg.Left(recMsg.Find(_T(":")));
					recMsg.Mid(recMsg.Find(_T(":"))+1);
#else
					temp = recMsg.Mid(nLenCmd);	//RunCard和BarCode是接在命令后面的，所有从命令开始取字符串--Added by LiHai--20180206
					AfxExtractSubString(RunCard, temp, 1, _T(':'));//recMsg.Left(recMsg.Find(_T(":")));
					AfxExtractSubString(Barcode, temp, 2, _T(':'));//recMsg.Mid(recMsg.Find(_T(":"))+1);
					RunCard.Trim(_T(" "));
					Barcode.Trim(_T(" "));

					CString str;
					str.Format(_T("RunCard:%s Barcode:%s"), RunCard, Barcode);
					m_spv3->m_pInterface->Ctrl_Addlog(0, str.GetBuffer(str.GetLength()), COLOR_RED, 200);
#endif
					/**
					*	要支持没有二维码的情况，即要支持不用扫BarCode的情况，没有二维码就是
					*	RunCard不为空而Barcode为空。
					*	--Added by LiHai--20180206
					**/
					if (RunCard == _T(""))
					{
						m_spv3->m_pInterface->Ctrl_Addlog(0, _T("Handler绑定信息错误!"), COLOR_RED, 200);
						m_spv3->m_pInterface->Ctrl_DrawingToMenDC(0,RGB(200,0,0));
						m_spv3->m_pInterface->Ctrl_DrawingToMenDC(0 ,RGB(0,0,0),_T("Handler绑定信息错误!"),150,4,2);
						m_spv3->m_pInterface->Ctrl_ShowMemDC(0);
						SendNonTestMsg(_T("H04010001"));
					}
					else
					{
#define IDC_strRuncard   1100
#define IDC_strBarCode   1013
#define IDC_DisplayWnd   1009
						
						/**
						*	搜索主界面上的RunCard和BarCode编辑框空间，赋值给编辑框，MFC框架导致产生空间数据变更消息然后把命令写入注册表。
						*	--Added by LiHai--20180206
						**/
						CWnd* pWnd = CWnd::FromHandle(m_spv3->hMainHwnd)->GetDlgItem(IDC_strRuncard);
						pWnd->SetWindowText(RunCard);
						pWnd = CWnd::FromHandle(m_spv3->hMainHwnd)->GetDlgItem(IDC_strBarCode);
						pWnd->SetWindowText(Barcode);
						m_spv3->m_pInterface->Ctrl_Addlog( 0,_T("获取绑定信息成功!"),COLOR_BLUE,220);
 						m_spv3->m_pInterface->Ctrl_DrawingToMenDC(0,RGB(0,200,0));
 						m_spv3->m_pInterface->Ctrl_DrawingToMenDC(0 ,RGB(0,0,0),_T("获取绑定信息成功!"),150,4,2);
 						m_spv3->m_pInterface->Ctrl_ShowMemDC(0);	
						SendNonTestMsg(_T("H04010000"));

#undef IDC_strRuncard
#undef IDC_strBarCode
#undef IDC_DisplayWnd
					}	
					continue;
				}

				waits = 0;
				vector<CString> infoList;
				processEvenPacket(recMsg,infoList);
				for (int i = 0; i < infoList.size(); i++)
				{
					int cmdType = isFindHandShakeCmd(infoList[i]);
					switch(cmdType)
					{
					case CMD_HANDSHAKE:
						//m_spv3->PostMsgControler(0,CAM_HANDLE);//发送握手
						SendNonTestMsg(m_spv3->HandleSend);
						break;
					case  CMD_TEST:
						processTestMsg(infoList[i]);
						break;
					case  CMD_UNKOWN:
						SendNonTestMsg(infoList[i]+_T("00"));
						break;
					}
					Sleep(1);
				}
				continue;
			}
			else if (length > 4)
			{
				continue;
			}
			waits++;
			if(waits>5)//5次消息为空，判断socket 异常
			{
				m_spv3->m_pInterface->Ctrl_Addlog(0, _T("Msg error"), COLOR_BLUE, 200);
				break;
			}
		}
		if (CamType == 1)
		{
			int CamID = 0;
			m_spv3->bPass[CamID] = FALSE;
			int lp = (m_spv3->bRefreshUi<<1)|CamID;
			if (!m_spv3->m_pInterface->isStandby(CamID))
				//if (!m_pInterface->isStandby(CamID) && false_continue)
			{
				m_spv3->m_pInterface->Ctrl_Addlog(CamID,_T("网络连接断开，关闭模组"),COLOR_BLUE,200);
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_TESTFAIL,(LPARAM)(lp));	
				Sleep(50);
				lp = (m_spv3->bPass[CamID]<<1)|CamID;
				m_spv3->m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("网络连接断开"));
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_SHOW_RESULT,(LPARAM)(lp));	
			}
		}else if (CamType == 2)
		{

			int CamID = 0;
			m_spv3->bPass[CamID] = FALSE;
			int lp = (m_spv3->bRefreshUi<<1)|CamID;
			if (!m_spv3->m_pInterface->isStandby(CamID))
			{
				m_spv3->m_pInterface->Ctrl_Addlog(CamID,_T("网络连接断开，关闭模组"),COLOR_BLUE,200);
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_TESTFAIL,(LPARAM)(lp));	
				Sleep(50);
				lp = (m_spv3->bPass[CamID]<<1)|CamID;
				m_spv3->m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("网络连接断开"));
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_SHOW_RESULT,(LPARAM)(lp));	
			}
			CamID = 1;
			m_spv3->bPass[CamID] = FALSE;
			lp = (m_spv3->bRefreshUi<<1)|CamID;
			if (!m_spv3->m_pInterface->isStandby(CamID))
			{
				m_spv3->m_pInterface->Ctrl_Addlog(CamID,_T("网络连接断开，关闭模组"),COLOR_BLUE,200);
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_TESTFAIL,(LPARAM)(lp));	
				Sleep(50);
				lp = (m_spv3->bPass[CamID]<<1)|CamID;
				m_spv3->m_pInterface->Ctrl_SetCamErrorInfo(CamID,_T("网络连接断开"));
				CWnd::FromHandle(m_spv3->hMainHwnd)->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_SHOW_RESULT,(LPARAM)(lp));	
			}
		}

		m_spv3->m_pInterface->Ctrl_Addlog(0, _T("socket end2"), COLOR_BLUE, 200);
		m_spv3->m_pInterface->Ctrl_Addlog(0,_T("网络连接断开"),COLOR_BLUE,200);
		SetEvent(hReceiveThreadDead);

		Close();

		ResetSrv();
	}
	CATCH(CException,e)
	{
		TCHAR error[512] = {0};
		memset(error,0,sizeof(TCHAR)*512);
		e->GetErrorMessage(error,512);
		CString str;
		str.Format(_T("ERROR： %s "),error);
		AfxMessageBox(str,MB_OK);	
		return 0;
	}
	END_CATCH
	return 1;
}
 
BOOL ApiSocket_tcpServer::isBusy()
{
	int Flag = WaitForSingleObject(sock_.hBusy, 0);
	BOOL bBusy = FALSE;
	if(Flag==WAIT_OBJECT_0)
	{
		bBusy = TRUE;
	}
	else
	{
		bBusy = FALSE;
	}
	return bBusy;
}


BOOL ApiSocket_tcpServer::processRsvMsg(CString msg)
{
	msg.MakeUpper();
	m_spv3->m_pInterface->Ctrl_Addlog(0, L"处理消息:"+msg, COLOR_BLACK, 200);
	int StationSize = m_spv3->vecStationWork.size();
	if(StationSize<0||StationSize>15)
	{
		Send(_T(""),0);
		return FALSE;
	}
	for(int i=0;i<StationSize;i++)
	{
		for(int j=0;j<CamType;j++)
		{
			int CamID = j;
			CString strProtocal = m_spv3->vecStationWork[i].msgProtocal[CamID].Receive;
			strProtocal.Trim(_T("T"));
			if((msg.Find(strProtocal) != -1) && (strProtocal.GetLength() > 1))
			{ 
				if(!isSetCurrentStation())
				{
					m_spv3->SetCurrentStation(m_spv3->vecStationWork[i]);//设置station 参数,一个站位只能设置一次。
					SetEvent(hSetCurrentStation);
				}
				//
				/*char cmdMsg[512] = {0};
				char cmd[512] = {0};
				sprintf_s(cmd,512,"Command_%d",m_spv3->m_pInterface->GetSiteNo());
				ImageProc::ConvertWideToChar(msg,cmdMsg);
				RegSetKey("",cmd,cmdMsg);*/
				//

				CString  strMsg;
				strMsg.Format(L"开始测试");
				m_spv3->m_pInterface->Ctrl_Addlog(CamID,strMsg,COLOR_BLUE,200);
			//	m_spv3->m_pInterface->Ctrl_Addlog(CamID,strProtocal,COLOR_BLUE,200); 
				m_spv3->PostMsgControler(CamID,CAM_TEST);//发送消息开始测试 
				if (m_spv3->nMode==0)
				{
					if(j==1) return TRUE;
				}
				else
					return TRUE;
			}
		} 
	}
	return FALSE;
}
 
int ApiSocket_tcpServer::Close()
{
	//int Ui_ID = GetUI_ID();

	sock_.bOnSocket = FALSE;
	EnterCriticalSection(&(m_spv3->m_cs));
	 NetSockLib->CloseSrv();
	LeaveCriticalSection(&(m_spv3->m_cs));	
	sock_.bOnSocket = FALSE;

	CloseHandle(sock_.hBusy);
	 
	WaitForSingleObject(hReceiveThreadDead,5000);
	 
	return 1;
}



int ApiSocket_tcpServer::Create(CString ipAddr,int nPort)
{
	 
	m_spv3->m_pInterface->Ctrl_Addlog(0,ipAddr,COLOR_BLUE,200);
	m_spv3->m_pInterface->Ctrl_Addlog(0,nPort,COLOR_BLUE,200);
	System::String^ ipAddr_ = gcnew System::String(ipAddr);
	NetSockLib->createSrv(ipAddr_,nPort); 	  
	return 1;
}

void ApiSocket_tcpServer::ResetSrv()
{ 	
	CString  strMsg;
	strMsg.Format(L"CONTROL mSG %d 2 ",SOCKET_INIT);
	m_spv3->m_pInterface->Ctrl_Addlog(0,strMsg,COLOR_BLUE,200);

	m_spv3->PostMsgControler(0,SOCKET_INIT);//  	  
}
BOOL ApiSocket_tcpServer::isSetCurrentStation()
{
	int Flag = WaitForSingleObject(hSetCurrentStation, 0);
	BOOL bSetCurrentSattion = FALSE;
	if(Flag==WAIT_OBJECT_0)
	{
		bSetCurrentSattion = TRUE;
	}
	else
	{
		bSetCurrentSattion = FALSE;
	}
	return bSetCurrentSattion;
}

int ApiSocket_tcpServer::isFindHandShakeCmd(CString msg)
{
	msg.MakeUpper();
	CString  strMsg;
	m_spv3->m_pInterface->Ctrl_Addlog(0, L"识别消息类型:", COLOR_BLACK, 200);

	int StationSize = m_spv3->vecStationWork.size();
	if ((msg.Find(m_spv3->HandleRevcive) != -1) && (m_spv3->HandleRevcive.GetLength()>5))
	{
		strMsg.Format(L"CMD_HANDSHAKE:%s ",msg);
		m_spv3->m_pInterface->Ctrl_Addlog(0,strMsg,COLOR_BLUE,200);		
		return CMD_HANDSHAKE;
	}
	else
	{
		BOOL bFindMsg = FALSE;			
		for(int i=0;i<StationSize;i++)
		{
			for(int j=0;j<CamType;j++)
			{
				int CamID = j;
				CString strProtocal = m_spv3->vecStationWork[i].msgProtocal[CamID].Receive;
				strProtocal.Trim(_T("T"));
				if((msg.Find(strProtocal) != -1) && (strProtocal.GetLength() > 1))
				{ 
					bFindMsg = TRUE;
					strMsg.Format(L"CMD_TEST:%s ",msg);
					m_spv3->m_pInterface->Ctrl_Addlog(0,strMsg,COLOR_BLUE,200);		
					return CMD_TEST;
				}
			} 
		}
		if (FALSE == bFindMsg)
		{
			//ResetEvent(sock_.hBusy);
			strMsg.Format(L"CMD_UNKOWN:%s ",msg);
			m_spv3->m_pInterface->Ctrl_Addlog(0,strMsg,COLOR_BLUE,200);		
			return CMD_UNKOWN;
		}
	}
	return CMD_UNKOWN;
}

void ApiSocket_tcpServer::processTestMsg(CString recMsg)
{
	if (isBusy())
	{
		return ;
	}
	int len = recMsg.GetLength();
	if(recMsg.Find(_T("H"))!=-1 || m_spv3->nMode==1)//接收一个处理一个
	{
	//	m_spv3->m_pInterface->Ctrl_Addlog(0, L"Mode1 Test", COLOR_BLACK, 200);
		recMsg.Replace(L" ",L"");
		sock_.recvinfoMsg.clear();
		SetEvent(sock_.hBusy);
		processRsvMsg(recMsg);
		sock_.SendTimes = 0;
		ResetEvent(hSetCurrentStation);		
	}
	else if(( m_spv3->nMode==2 )|| len>=14)//接收两个处理两个,或者同时收到两个
	{
		//m_spv3->m_pInterface->Ctrl_Addlog(0, L"Mode2 Test", COLOR_BLACK, 200);
		recMsg.Replace(L" ",L"");
		sock_.recvinfoMsg.push_back(recMsg);
		if(sock_.recvinfoMsg.size()==2 || len>=14)
		{
			if(len>=14)
			{
				m_spv3->m_pInterface->Ctrl_Addlog(0,recMsg,COLOR_BLUE,200);
				sock_.recvinfoMsg.clear();
				m_spv3->SeparateString(recMsg,'\n',&sock_.recvinfoMsg);

			}
			ResetEvent(hSetCurrentStation);
			SetEvent(sock_.hBusy);
			sock_.SendTimes = 0;
			for(int i=0;i<2;i++)
			{ 								
				processRsvMsg(sock_.recvinfoMsg[i]);
				Sleep(100);
			}
		}
		else if(sock_.recvinfoMsg.size()>2)//刷新消息
		{
			ResetEvent(hSetCurrentStation);
			sock_.recvinfoMsg.clear();
			ResetEvent(sock_.hBusy);
			sock_.SendTimes = 0;
		} 		
	}
}

void ApiSocket_tcpServer::processEvenPacket(CString msg,vector<CString>& infoList)
{		
	msg.Trim(_T(" "));
	if(msg == _T("")) return ;
	infoList.clear();
	int len = msg.GetLength();
	TCHAR TSeperator = 'T';
	TCHAR HSeperator = 'H';
	int iStart = 0;
	TCHAR firstChart ;
	if (msg.GetAt(0) == 'T')
	{
		firstChart = 'T';
		msg.Trim(_T("T"));
	}else if (msg.GetAt(0) == 'H')
	{
		firstChart = 'H';
		msg.Trim(_T("H"));
	}
	for (int i = 0; i < len; i++)
	{
		if (msg.GetAt(i) == TSeperator || 
			msg.GetAt(i) == HSeperator ||
			i == len - 1)
		{
			CString temp = msg.Mid(iStart,i - iStart);
			temp.Trim(_T(" "));
			CString cmd = _T("");
			if (firstChart == 'T')
			{
				cmd = _T("T") + temp;				
			}else if (firstChart == 'H')
			{
				cmd = _T("H") + temp;
			}
			infoList.push_back(cmd);
			iStart = i+1;	
			firstChart = msg.GetAt(i);
		}
	}
}


void ApiSocket_tcpServer::SendNonTestMsg(CString msg)
{
	EnterCriticalSection(&(sock_.m_cs));
	char tmpBuf[1024] = {0};//"T020101";
	ImageProc::ConvertWideToChar(msg,tmpBuf);
    //EnterCriticalSection(&(sock_.m_cs));
	System::String^ sendbuf = gcnew String(msg);
	NetSockLib->SendMsg(sendbuf); 
	m_spv3->m_pInterface->Ctrl_Addlog(0, _T("out:")+msg, COLOR_GREEN, 200);	
	LeaveCriticalSection(&(sock_.m_cs));
}