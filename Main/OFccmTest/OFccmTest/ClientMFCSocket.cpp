// ClientMFCSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "ClientMFCSocket.h"



// ClientMFCSocket

ClientMFCSocket::ClientMFCSocket(void)
{
	Ret = 0.0;
	isconnected = FALSE;
	readComplete = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_connEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	InitializeCriticalSection(&m_cs);

	hReturnMsg    = CreateEvent(NULL, FALSE, FALSE, NULL);   //消息监视
	//SetEvent(hReturnMsg);
	isStartTest = false;
	if(readComplete == NULL || m_connEvent == NULL)
		throw "CreateEvent Fail!";

	//WORD SendwVersionRequested = MAKEWORD(1,1);
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
	WPARAM wpara;
	switch(nLen)
	{
	case 0:
		TRACE(_T("接收到空字符串！"));
		return;
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			//AfxMessageBox(_T("发生错误！"));
			Close();
		}
		break;
	default:
		buff[nLen] = 0;
	    SetEvent(hReturnMsg);

		CString Mess(buff);
		pCcmCtrl->ReciveData = Mess;
		ReturnMsg=Mess;
		Mess.MakeUpper();
		

		pCcmCtrl->Ctrl_Addlog(CamID,_T("收到消息:")+Mess,COLOR_BLUE,200);
		
		if (Mess.Find(_T("H02000"))!=-1)
		{
			wpara = WM_SOCKET_CLOSE;
		}
		else if(Mess.Find(_T("H01001"))!=-1)
		{
			wpara = WM_SOCKET_UPDATE;
		}
		else if (Mess.Find(_T("H01000"))!=-1||Mess.Compare(_T("OPENCAMERA"))==0)//必须完全
		{
			wpara =WM_SOCKET_OPENCAMERA;
		} 
		else if (Mess.Find(_T("T040100"))!=-1)
		{
			wpara = WM_SOCKET_BINDING;
		}
		else if(Mess.Find(_T("STARTTEST"),0) != -1)
		{
			wpara = WM_SOCKET_STARTTEST;
			isStartTest = true;
		}
		else if(Mess.Find(_T("STARTCHECK"),0) != -1)
		{
			wpara = WM_SOCKET_STARTCHECK;
		}		
		else if (Mess.Find(_T("PASS"),0) != -1) 
		{
			wpara = WM_SOCKET_TESTPASS;
		}
		else if (Mess.Find(_T("FAIL"),0) != -1)
		{
			wpara = WM_SOCKET_TESTFAIL;
		}
		else if (Mess.Find(_T("CCT_A_OK"))!= -1)
		{
			wpara = WM_SOCKET_CCT_A_OK;
		}
		else if (Mess.Find(_T("CCT_TL84_OK"))!= -1)
		{
			wpara = WM_SOCKET_CCT_TL84_OK;
		}
		else if (Mess.Find(_T("CCT_D65_OK"))!= -1)
		{
			wpara = WM_SOCKET_CCT_D65_OK;
		}
		else if (Mess.Find(_T("CCT_D50_OK"))!= -1)
		{
			wpara = WM_SOCKET_CCT_D50_OK;
		}
		else if (Mess.Find(L"H0100")!=-1) //wzx added
		{
			wpara = WM_AUTOCALIBRATION;
		}
		else if (Mess.Find(L"H0200")!=-1)
		{
			wpara = WM_AUTOGETAVE;
// 			PostMessage(WM_AUTOGETAVE,0,0);
// 			AddString(BLUE,"取平均值并导入");
		}
		else
		{ 
			return;
		} 
		
		//pCcmCtrl->C trl_AddLog(Mess,CamID); 
	} 
	if(p_Main!= NULL)
		p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)wpara,(LPARAM)(CamID));
	pCcmCtrl->Ctrl_AddToLog(_T("POSTMsg"),CamID);
	CSocket::OnReceive(nErrorCode);
}
void ClientMFCSocket::OnClose(int nErrorCode)
{
	//以下伆删
	//char buff[1024]={0};
	//int bufflen=1024;
	
	//strcpy(buff,"H02000");
	//CString Info = _T("H02000");
	//int len = Info.GetLength();
	//WideCharToMultiByte(CP_ACP,0,Info,len,buff,len,NULL,NULL);//T2A,W2A
	//buff[len]='\0';
	if(m_hSocket != INVALID_SOCKET)
		Close();
	isconnected = FALSE;

	if(p_Main!= NULL)
		p_Main->PostMessage(WM_UPDATEMSG,(WPARAM)WM_SOCKET_CLOSE,CamID);
	CSocket::OnClose(nErrorCode);
	//WSACleanup();
}


void ClientMFCSocket::SendInfo(CString info)
{
	char buff[1024]={0};
	int bufflen=1024; 
	int len = info.GetLength();
	//char *bufj = CT2A(info);
	WideCharToMultiByte(CP_ACP,0,info,len,buff,len,NULL,NULL);//T2A,W2A

	 Send(buff,strlen(buff));
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



int ClientMFCSocket::Move(int motor,double pos)
{
	CStringA cmd;
	cmd.Format(("move m%d %f\n"),motor+1,pos); 
	CString str =  sendin(cmd,cmd.GetLength()+1);
	//pCcmCtrl->Ctrl_Addlog(CamID,cmd,COLOR_BLUE,200);
	 
	str.Format(_T("motor = %d,Flag=%s"),motor+1,str);
	pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);	 
	pCcmCtrl->Ctrl_AddToLog(str,CamID);
	return 1;
}
int ClientMFCSocket::MoveTo(int motor,double pos)
{
	CString cmd;
	cmd.Format(_T("moveto m%d %f\n"),motor+1,pos);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::MoveTowards(int motor,int dir)
{
	CString cmd;
	cmd.Format(_T("movetowards m%d %d\n"),motor+1,dir);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::Stop(int motor)
{
	CString cmd;
	cmd.Format(_T("stop m%d"),motor+1);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
double ClientMFCSocket::GetPos(int motor)
{
	CStringA cmd;
	cmd.Format(("getpos m%d\n"),motor+1);
	CString m_cmd;
	m_cmd.Format(_T("getpos m%d"),motor+1);
	CString Info = sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	pCcmCtrl->Ctrl_Addlog(CamID,m_cmd,COLOR_BLUE,200);
	//pCcmCtrl->Ctrl_AddToLog(Info,CamID);
	double pos = _ttof(Info);	
	return pos;
}
int ClientMFCSocket::Getin(int nIO)
{
	CString cmd;
	cmd.Format(_T("getin %d\n"),nIO);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::GetState(int motor,int nIO)
{
	CString cmd;
	cmd.Format(_T("getstate m%d %d\n"),motor+1,nIO);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::SetOut(int nIO,BOOL isOn)
{
	CString cmd;
	if(isOn)
		cmd.Format(_T("setout %d\n"),nIO);
	else
		cmd.Format(_T("rstout %d\n"),nIO);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);

	return 1;
}

int ClientMFCSocket::SetSvon(int motor,BOOL isOn)
{
	CString cmd;
	cmd.Format(_T("svon m%d %d\n"),motor+1,isOn);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::Home(int motor)
{
	CString cmd;
	cmd.Format(_T("home m%d\n"),motor+1);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::ClrALM(int motor)
{
	CString cmd;
	cmd.Format(_T("clralm m%d\n"),motor+1);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}
int ClientMFCSocket::SetSpeed(int motor,double start,double max)
{
	CString cmd;
	cmd.Format(_T("setspeed m%d\n %f,%f"),motor+1,start,max);
	sendin(cmd.GetBuffer(255),cmd.GetLength()+1);
	return 1;
}


CString ClientMFCSocket::sendin(const void* lpbuff,int nBufLen,int nFlag)
{
	EnterCriticalSection(&m_cs);
	CString str; 
	ResetEvent(hReturnMsg);
	Sleep(100);
	int Flag =  Send(lpbuff,nBufLen,nFlag);	
	Sleep(100);
	if(Flag!=-1)
	{
		pCcmCtrl->Ctrl_Addlog(CamID,_T("发送成功"),COLOR_BLUE,200);
		if( GetReturnMsg())
		pCcmCtrl->Ctrl_Addlog(CamID,_T("接收成功"),COLOR_BLUE,200);
		else
		pCcmCtrl->Ctrl_Addlog(CamID,_T("接收延时"),COLOR_RED,200);

	}
	else
	{
		str  = _T("SendFail");
		pCcmCtrl->Ctrl_Addlog(CamID,_T("发送失败"),COLOR_RED,200);
	} 
	LeaveCriticalSection(&m_cs);
	return ReturnMsg;
	 
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

	CAsyncSocket::OnConnect(nErrorCode);
}
 

BOOL ClientMFCSocket::GetReturnMsg()
{
	int Flag = WaitForSingleObject(hReturnMsg, 2000);
	BOOL bReturnMsg = FALSE;
	if(Flag==WAIT_OBJECT_0)
	{
		bReturnMsg = TRUE;
	}
	else
	{
		bReturnMsg = FALSE;
	}
	return bReturnMsg;
}