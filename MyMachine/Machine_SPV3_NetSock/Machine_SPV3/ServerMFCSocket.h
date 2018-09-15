#pragma once
#include "ClientMFCSocket.h"

class Machine_NCO_SPV3;
// ServerMFCSocket ����Ŀ��

//class ServerMFCSocket : public CSocket
class ServerMFCSocket : public ApiSocket
{
public:
	Machine_NCO_SPV3* m_pSpv3;
	ServerMFCSocket();
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
 