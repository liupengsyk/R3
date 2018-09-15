#pragma once
#include <stdio.h>
#include <WinSock2.h>
#include <WinSock.h>

 class Machine_NCO_SPV3;

#using "..\\Debug\\SocketNetLib.dll"
 using namespace SocketNetLib;  
 using namespace System;
 

 struct sockStru
 { 
	 HANDLE hBusy;
	 CRITICAL_SECTION m_cs;
	 BOOL bCreateSucess;
	 vector<CString> recvinfoMsg;
	 int SendTimes;
	  
	 BOOL bOnSocket;	
 };
 
  class ApiSocket_tcpServer
{   
public:
	ApiSocket_tcpServer(Machine_NCO_SPV3* m_spv3);
	~ApiSocket_tcpServer();
	gcroot<SocketNetLibProgram^> NetSockLib;
	//Client
public:
	
	int Send(CString Msg,int CamID); 
	int  Close();
	int StartSocket(int nPort);

	vector<CString> GetIpList();
	int nPort;
	CString ipAddr;
private:
	int Receive();
	static UINT ReceiveThread0(void* param);
	
	/*static UINT ReceiveThread2(void* param);
	static UINT ReceiveThread3(void* param);
	*/HANDLE hReceiveThreadDead;
		
	BOOL isBusy();
	//Srv
private:	
	int Create(CString ipAddr,int nPort);
 

	BOOL isSetCurrentStation();
	HANDLE hSetCurrentStation;
	 sockStru sock_;
public:
	 int CamType; 
private:
		 
private:
	BOOL processRsvMsg(CString msg);
	Machine_NCO_SPV3* m_spv3;
	int isFindHandShakeCmd(CString msg);
	void processTestMsg(CString recMsg);
	void processEvenPacket(CString msg,vector<CString>& infoList);
	void SendHandShakeCmd(CString msg);
	void SendNonTestMsg(CString msg);
	void ResetSrv();
	BOOL isReset;
};

