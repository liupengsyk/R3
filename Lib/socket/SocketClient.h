#pragma once


// #define MAX_BUFFER_LEN 8192

#ifdef YK_CSOCKETCLIENT
#define YK_SOCKET_API	__declspec(dllexport)
#else
#define YK_SOCKET_API	__declspec(dllimport)

#ifdef _DEBUG
#pragma comment(lib,"SocketModelD.lib")
#else
#pragma comment(lib,"SocketModel.lib")
#endif // _DEBUG

#endif


struct IOMsg 
{
	SOCKET	m_sock;
	DWORD	dwByte;
	char*	lpBuffer;
};

class CSocketClient
{
public:
	CSocketClient(void);
	~CSocketClient(void);


	virtual void OnReceive(IOMsg &msg) = 0;
	virtual void OnSendComplete(IOMsg &msg) = 0;
	virtual void OnClose() = 0;

};


YK_SOCKET_API BOOL YK_InitSocketModel();
YK_SOCKET_API BOOL YK_UnInitSocketModel();
YK_SOCKET_API BOOL YK_Connect(CSocketClient* pClient,LPCTSTR ip,UINT nPort);
YK_SOCKET_API BOOL YK_IsConnected(CSocketClient* pClient);
YK_SOCKET_API int  YK_Send(CSocketClient* pClient,const char* lpbuff,DWORD dwByte,BOOL bBlock,int nFlag);
YK_SOCKET_API BOOL YK_Close(CSocketClient* pClient);
