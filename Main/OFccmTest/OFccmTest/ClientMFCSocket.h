#pragma once
   #include <WinSock.h>
//SOCKET 
class ClientMFCSocket : public CSocket
{
public:
	ClientMFCSocket(void);
	virtual ~ClientMFCSocket(void);
public:
	CWnd *p_Main;
	CString MyRemoteIP;
	int MyPort;
	int CamID;
	int SocketInit(int CamID);	
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void CloseCamera(BOOL bResult);

	BOOL Conn(CString ip,UINT nport);
	BOOL DisConn();
	BOOL isConn(){return isconnected;}

	bool isStartTest;

	double Ret;
	int Move(int motor,double pos);
	int MoveTo(int motor,double pos);
	int MoveTowards(int motor,int dir);
	int Stop(int motor);
	int Getin(int nIO);
	int GetState(int motor,int nIO);
	int SetOut(int nIO,BOOL isOn);
	double GetPos(int motor);
	int SetSvon(int motor,BOOL isOn = TRUE);
	int Home(int motor);
	int ClrALM(int motor);
	int SetSpeed(int motor,double start,double max);
private:
	BOOL isconnected;
	CRITICAL_SECTION m_cs;
	HANDLE m_connEvent;
	HANDLE readComplete;

	CString sendin(const void* lpbuff,int nBufLen,int nFlag = 0);
public:
	
	virtual void OnConnect(int nErrorCode);
	void SendInfo(CString info);
	 
	CString ReturnMsg;
	HANDLE hReturnMsg ;
	BOOL GetReturnMsg();
	
};


