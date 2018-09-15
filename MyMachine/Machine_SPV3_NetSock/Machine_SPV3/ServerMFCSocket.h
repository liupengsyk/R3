#pragma once
#include "ClientMFCSocket.h"

class Machine_NCO_SPV3;
// ServerMFCSocket 命令目标

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
	 
	CString ip_Addr;//客户端的ip
	UINT nPort;//客户端的端口
	int n_index;
	char tem[128];
	CString hostIP;//服务器ip
	UINT hostPort;//服务器端口

	BOOL isTeam1;
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	
	int CamID;
	int iPort; 
};
 