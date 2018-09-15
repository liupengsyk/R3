#pragma once
#include "ClientMFCSocket.h"

// ServerMFCSocket ����Ŀ��

class ServerMFCSocket : public CSocket
{
public:
	ServerMFCSocket(int CamID,int Port,CWnd *p_Main);
	virtual ~ServerMFCSocket();

	ClientMFCSocket* m_client;
	CWnd *p_Main;
	 
	CString ip_Addr;//�ͻ��˵�ip
	UINT nPort;//�ͻ��˵Ķ˿�
	int n_index;
	char tem[128];
	CString hostIP;//������ip
	UINT hostPort;//�������˿�

	BOOL isTeam1;
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	
	int CamID;
	int iPort;

};


