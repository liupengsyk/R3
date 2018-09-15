#pragma once
 #include <afxsock.h>
#include "ApiSocket.h"
class Machine_NCO_SPV3;
class ClientMFCSocket : public CSocket
//class ClientMFCSocket : public ApiSocket
{
public:
	ClientMFCSocket();
	ClientMFCSocket(Machine_NCO_SPV3* pSpv3);
	virtual ~ClientMFCSocket(void);
public:
	Machine_NCO_SPV3* m_spv3;
public:
	//CWnd *p_Main;
	int iWorkStation;
	CString MyRemoteIP;
	int MyPort;
	int CamID;
	int SocketInit(int CamID);	
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	 

	BOOL Conn(CString ip,UINT nport);
	BOOL DisConn();
	BOOL isConn(){return isconnected;} 

	double Ret;
	 

	CString m_Cmd;
	BOOL processRsvMsg(CString msg);

	CString GetReturnMsg();
	CString ReturnMsg;
	HANDLE hReturnMsg;
	 
	
private:
	CString SocMsgBuf;
	CRITICAL_SECTION m_cs;
	HANDLE m_connEvent;
	HANDLE readComplete;

public:
	int sendin(CString cmdSend,int nFlag = 0);
	virtual void OnConnect(int nErrorCode);
	BOOL isconnected;
};


