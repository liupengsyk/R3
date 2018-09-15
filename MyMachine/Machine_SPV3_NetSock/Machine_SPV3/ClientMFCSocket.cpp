// ClientMFCSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmDef.h"
#include "Machine_NCO_SPV3.h"
#include "ClientMFCSocket.h"
#include <afxsock.h>
#include "ImageProc.h"


// ClientMFCSocket
ClientMFCSocket::ClientMFCSocket()
{
	SocMsgBuf = _T("");
	Ret = 0.0;
	isconnected = FALSE;
	readComplete = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_connEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	InitializeCriticalSection(&m_cs);
	iWorkStation = 0;

	if(readComplete == NULL || m_connEvent == NULL)
		throw "CreateEvent Fail!";

	WORD SendwVersionRequested = MAKEWORD(2,2);
	WSADATA sendwsaData;
	//启动

	int err = WSAStartup(SendwVersionRequested, &sendwsaData);
	if(err != 0)		 
		//判断是否是指定的加载版本
		if(LOBYTE(sendwsaData.wVersion) !=1 ||HIBYTE(sendwsaData.wVersion !=1))
		{
			WSACleanup();
		}
  m_spv3 =NULL;
}

ClientMFCSocket::ClientMFCSocket(Machine_NCO_SPV3* pSpv3)
{
	SocMsgBuf = _T("");
	m_spv3 = pSpv3;
	Ret = 0.0;
	isconnected = FALSE;
	readComplete = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_connEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	InitializeCriticalSection(&m_cs);
	iWorkStation = 0;

	if(readComplete == NULL || m_connEvent == NULL)
		throw "CreateEvent Fail!";

	//WORD SendwVersionRequested = MAKEWORD(2,2);
	//WSADATA sendwsaData;
	////启动
	//int err = WSAStartup(SendwVersionRequested, &sendwsaData);
	//if(err != 0)		 
	//	//判断是否是指定的加载版本
	//	if(LOBYTE(sendwsaData.wVersion) !=1 ||HIBYTE(sendwsaData.wVersion !=1))
	//	{
	//		WSACleanup();
	//	}
}

ClientMFCSocket::~ClientMFCSocket()
{
	if(readComplete)
    CloseHandle(readComplete);
    if(m_connEvent)
	CloseHandle(m_connEvent);
}

 
int ClientMFCSocket::SocketInit(int CamID)
{
	//CString autoSetup;
	//GetPrivateProfileString(_T("SOCKET"),_T("RemoteIP"),_T("127.0.0.1"),RemoteIP,256,autoSetup);
	//Port=GetPrivateProfileInt(_T("SOCKET"),_T("Port"),5001,autoSetup);
	//指明要加载的SOCKET版本号
	
	hReturnMsg    = CreateEvent(NULL, TRUE, TRUE, NULL);   //消息监视
	SetEvent(hReturnMsg);
	this->CamID = CamID;

	return 0;

}
// ClientMFCSocket 成员函数

void ClientMFCSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	char buff[1024]={0};
	int bufflen=1024;
	//char charCamid = 
	 
	int nLen = CSocket::Receive(buff,bufflen);
	//WPARAM wpara =  = reinterpret_cast<WPARAM>(buff);
	//WPARAM wpara;
	switch(nLen)
	{
	case 0:
		TRACE(_T("接收到空字符串！"));
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("发生错误！"));
			Close();
		}
		break;
	default:
		ResetEvent(hReturnMsg);
		buff[nLen] = 0;
	 

		CString Mess(buff);
		Mess.MakeUpper();
		ReturnMsg = Mess;
		SocMsgBuf +=Mess;
		vector<CString> Msg;
		m_spv3->SeparateString(SocMsgBuf,'\r\n',&Msg);
		for(UINT i=0;i<Msg.size();i++)
		{
			if(Msg[i].GetLength()>6)
			{
				SocMsgBuf.Trim(Msg[i]);
				processRsvMsg(Msg[i]);
			}
			else
			{
				break;
			}
			Sleep(50);
		}
		
		break;
	} 

	//p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)wpara,(LPARAM)(CamID));
	CSocket::OnReceive(nErrorCode);
}

BOOL ClientMFCSocket::processRsvMsg(CString msg)
{
	BOOL ret = FALSE;

	m_spv3->m_pInterface->Ctrl_Addlog(0, msg, COLOR_BLUE, 200); 
	int StationSize = m_spv3->vecStationWork.size();

	if ((msg.Find(m_spv3->HandleRevcive) != -1) && (m_spv3->HandleRevcive.GetLength()))
		m_spv3->PostMegControler(0,0xF1);//发送握手
	else
	{
		for(UINT i=0;i<StationSize;i++)
		{
			for(int j=0;j<4;j++)
			{
				if((msg.Find(m_spv3->vecStationWork[i].msgProtocal[j].Receive) != -1) && (m_spv3->vecStationWork[i].msgProtocal[j].Receive.GetLength() > 1))
				{
					//m_spv3->m_socketCamID = j;
					m_spv3->SetCurrentStation(m_spv3->vecStationWork[i]);//设置station 参数
					m_spv3->PostMegControler(j,0xF0);//发送消息开始测试
					return TRUE;
				}
			} 
		}
	}
 
	return ret;
}

CString ClientMFCSocket::GetReturnMsg()
{
	WaitForSingleObject(hReturnMsg, 50);
	SetEvent(hReturnMsg);
	return ReturnMsg;
}

void ClientMFCSocket::OnClose(int nErrorCode)
{
	 
	if(m_hSocket != INVALID_SOCKET)
		Close();
	isconnected = FALSE;

	//p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)WM_SOCKET_CLOSE,CamID);
	CSocket::OnClose(nErrorCode);
	WSACleanup();
} 


// Cclient 成员函数
BOOL ClientMFCSocket::Conn(CString ip,UINT nport)
{
	if(m_hSocket == INVALID_SOCKET)
	{
		if(!Create())
			return FALSE;
	}
	isconnected = Connect(ip,nport);
	if(!isconnected)
	{ 
		return FALSE;
	}

	return isconnected;
}

BOOL ClientMFCSocket::DisConn()
{
	if(m_hSocket != INVALID_SOCKET)
		Close();
	isconnected = FALSE;
	return TRUE;
}
 

int ClientMFCSocket::sendin(CString cmdSend,int nFlag)
{  
	EnterCriticalSection(&m_cs);
	char buff[1024]={0};
	int bufflen=1024;

	cmdSend += _T("\r\n");
	int len = cmdSend.GetLength();
	WideCharToMultiByte(CP_ACP,0,cmdSend,len,buff,len,NULL,NULL);//T2A,W2A
	buff[len]='\0';
	
	int FlG = Send(buff,len+1,0);
	//int FlG = Send(cmdSend,cmdSend.GetLength());
	//int FlG = SendChunk(cmdSend,cmdSend.GetLength(),0);
	
	if(m_spv3 !=NULL)
	{
		CString str;
		str.Format(_T("port:%d,flg:%d"),MyPort,FlG);
		if(FlG>1)
		{
			m_spv3->m_pInterface->Ctrl_Addlog(0,cmdSend + _T("\t") + MyRemoteIP + str,COLOR_GREEN,200);
		}
		else
		{
			m_spv3->m_pInterface->Ctrl_Addlog(0,cmdSend + _T("\t") + MyRemoteIP + str,COLOR_RED,200);
		}
	}
	int result = WaitForSingleObject(readComplete,100); 
	 
	LeaveCriticalSection(&m_cs);
	 
	return FlG;
 
}

//根据连接返回值，判断是否连接成功
void ClientMFCSocket::OnConnect(int nErrorCode)
{
	if(nErrorCode == 0)
	{
		isconnected = TRUE;
		SetEvent(m_connEvent);
	}
	else
		isconnected = FALSE;

	CSocket::OnConnect(nErrorCode);
}
