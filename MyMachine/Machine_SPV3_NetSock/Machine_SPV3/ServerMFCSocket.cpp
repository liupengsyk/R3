// ServerMFCSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Machine_NCO_SPV3.h"
#include "ServerMFCSocket.h"



// ServerMFCSocket

ServerMFCSocket::ServerMFCSocket()
{

}


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
}


// ServerMFCSocket 成员函数
 

void ServerMFCSocket::OnAccept(int nErrorCode)
{
	if(m_client)
	{
		m_client->Close();
		delete m_client;
		m_client = NULL;
	}

	m_client = new ClientMFCSocket(m_pSpv3);
	Accept(*m_client);
	//TRACE(_T("连接成功"));

	m_client->sendin(_T("success\r\n"),0);
	m_client->GetPeerName(ip_Addr, nPort);
	m_client->MyRemoteIP = ip_Addr;
	m_client->MyPort = iPort;
	m_client->CamID = CamID;
	CString str;
	str.Format(_T("ip_Addr%s,nPort %d"), ip_Addr, nPort);
	m_client->p_Main = p_Main;

	ASSERT(!ip_Addr.IsEmpty() && nPort != 0);
}


void ServerMFCSocket::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_client != NULL)
	{
		m_client->Close();
		delete m_client;
		m_client = NULL;
		ASSERT(m_client->m_hSocket == INVALID_SOCKET);
	}
	
	CAsyncSocket::OnClose(nErrorCode);
}

