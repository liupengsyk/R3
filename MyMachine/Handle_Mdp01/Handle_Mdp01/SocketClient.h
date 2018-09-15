#pragma once
 


class SocketClient : public CSocket
{
public:
	SocketClient(void);
	virtual ~SocketClient(void);
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	BOOL Conn(CString ip, UINT nport);
	void MessageProc(CString Msg);
	int SendStr(CString Info); 

	static UINT isConnectGet(void *param);
	int isConnectGet();
	BOOL isCon;
	int ConSend;

	int iCamComplete[2];

	CCriticalSection m_cs;
};
