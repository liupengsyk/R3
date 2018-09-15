// ServerMFCSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "ServerMFCSocket.h"


// ServerMFCSocket

ServerMFCSocket::ServerMFCSocket(int CamID,int Port,CWnd *p_Main)
{
	m_client = NULL;
	ip_Addr = "";
	nPort = 0;
	hostIP = "";
	hostPort = 0;
	isTeam1 = FALSE;
	n_index = -1;
	this->CamID = CamID;
	this->p_Main = p_Main;
	iPort = Port;
}

ServerMFCSocket::~ServerMFCSocket()
{
	p_Main = NULL;
	SAFE_DELETE(m_client); 
}
 

void ServerMFCSocket::OnAccept(int nErrorCode)
{ 
	if(m_client)
	{		 
		m_client->Close();
	} 
	m_client = new ClientMFCSocket;
	m_client->p_Main = p_Main;
	Accept(*m_client);
	TRACE(_T("连接成功"));
	pCcmCtrl->Ctrl_Addlog(CamID,_T("连接成功"),COLOR_GREEN,200);
	pCcmCtrl->InitSocket(CamID,FALSE);
 
	m_client->GetPeerName(ip_Addr,nPort);
	m_client->CamID = CamID;
	CString str;
	str.Format(_T("ip_Addr%s,nPort %d"),ip_Addr,nPort);
	pCcmCtrl->Ctrl_AddToLog(str,CamID);
	pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_GREEN,200);
 

	ASSERT(!ip_Addr.IsEmpty() && nPort != 0);
	 
}

void ServerMFCSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_client != NULL)
	{
		//m_client.KillSocket(m_client.m_hSocket,&m_client);
		m_client->Close();
		//m_client.ShutDown();
		//KillSocket(m_client.m_hSocket,&m_client);
		delete m_client;
		m_client = NULL;
		ASSERT(m_client->m_hSocket == INVALID_SOCKET);
	}
	
	CAsyncSocket::OnClose(nErrorCode);
}
