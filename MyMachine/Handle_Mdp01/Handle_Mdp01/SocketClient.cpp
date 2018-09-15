#include "stdafx.h"
#include "Handle_Mdp01.h"
#include "SocketClient.h"
 
// SocketClient

SocketClient::SocketClient(void)
{
	WORD SendwVersionRequested = MAKEWORD(1, 1);
	WSADATA sendwsaData;
	//����
	int err = WSAStartup(SendwVersionRequested, &sendwsaData);
	if (err != 0)
		//�ж��Ƿ���ָ���ļ��ذ汾
		if (LOBYTE(sendwsaData.wVersion) != 1 || HIBYTE(sendwsaData.wVersion != 1))
		{
			WSACleanup();
		}

	iCamComplete[0] = 0;
	iCamComplete[1] = 0;
	InitializeCriticalSection(m_cs);
}

SocketClient::~SocketClient()
{

}

void SocketClient::OnReceive(int nErrorCode)
{

	// TODO: Add your specialized code here and/or call the base class

	char buff[1024] = { 0 };
	int bufflen = 1024;
	CString str;
	int nLen = CSocket::Receive(buff, bufflen);
	str.Format(_T("nLen %d"), nLen);
	CString Msg(buff);
 
	switch (nLen)
	{
	case 0:
		TRACE(_T("���յ����ַ�����"));
		break;
	case SOCKET_ERROR:
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("��������"));
			Close();
		}
		break;
	default:
		//������Ϣ
		pDlgS->LogShow(Msg);
		MessageProc(Msg);
		break;
	}

	//p_Main->PostMessage (WM_UPDATEMSG,(WPARAM)wpara,(LPARAM)(CamID));
	CSocket::OnReceive(nErrorCode);
}
void SocketClient::OnClose(int nErrorCode)
{

	if (m_hSocket != INVALID_SOCKET)
		Close();
	CSocket::OnClose(nErrorCode);
	WSACleanup();
}
// Cclient ��Ա����
BOOL SocketClient::Conn(CString ip, UINT nport)
{ 
	if (m_hSocket == INVALID_SOCKET)
	{
		if (!Create())
			return FALSE;
	}
	BOOL isconnected = Connect(ip, nport);
	if (!isconnected)
	{
		return FALSE;
	}
	//AfxBeginThread(isConnectGet,this);
	return isconnected;
}
void  SocketClient::MessageProc(CString Msg)
{ 
	
	int CurentStation = 0;
	EnterCriticalSection(m_cs);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			CString ij;
			if (Msg.Find(pDlgS->m_CameraControl.CamStationTestResultFail[i][j]) != -1)
			{
				 //���͹ر�ģ�飬���ҷ�����Ϣ��dlg ����̨��λ
				if(j==4) iCamComplete[i] = 0;//
				else
				iCamComplete[i] = -1;//
				break;
			}
			else if (Msg.Find(pDlgS->m_CameraControl.CamStationTestResultPass[i][j]) != -1)
			{
				//�ж��Ƿ������һ����������� ���͹ر�ģ�飬���ҷ�����Ϣ��dlg ����̨��λ
				//������ǣ�������һ������ָ��
				iCamComplete[i] = 1;//
				CurentStation = j;				 
				break;
			}
		}
	}
	
	if ((iCamComplete[0] == -1 || iCamComplete[1] == -1 )&& (iCamComplete[0] != 0 && iCamComplete[1] != 0))
	{
		
		pDlgS->PostMsgToControl(TEST_MSG, 4);
		pDlgS->m_CameraControl.StationTestResult = FALSE;
		pDlgS->LogShow(_T("TEST_FAIL"));
		iCamComplete[0] = 0;
		iCamComplete[1] = 0;
	}
	else if(iCamComplete[0] == 1 && iCamComplete[1] == 1)
	{
		if ((CurentStation + 1) < 5)
		{
			pDlgS->PostMsgToControl(TEST_MSG, CurentStation + 1);
			pDlgS->LogShow(_T("TEST_NEXT"));
		}
		iCamComplete[0] = 0;
		iCamComplete[1] = 0;
	}
	LeaveCriticalSection(m_cs);

	//int RevTime = GetTickCount();
	//if (Msg.Find(_T("T0800000000000000")) != -1)
	//{
	//	ConSend = TRUE;
	//}
	//if (RevTime - ConSend>3000)
	//{
	//	pDlgS->PostMsgToControl(TEST_MSG, 5);//��̨��λ
	//	ConSend = FALSE;
	//}

	

}
 
int SocketClient::SendStr(CString Info)
{
	char buff[1024] = { 0 };
	int bufflen = 1024;
	int len = Info.GetLength();
	WideCharToMultiByte(CP_ACP, 0, Info, len, buff, len, NULL, NULL);//T2A,W2A
	buff[len] = '\0';
	Send(buff, len);
	Sleep(500);
	return 0;
}


UINT SocketClient::isConnectGet(void *param)
{
	SocketClient* pSock = static_cast<SocketClient*> (param);
	pSock->isConnectGet();
	pSock = NULL;
	return 0;
}
int SocketClient::isConnectGet()
{
	pDlgS->LogShow(_T("isConnectGet_in"));
	while (1)
	{
		SendStr(_T("H0800000000000000"));
		ConSend = GetTickCount();
		Sleep(1000);
		if (!ConSend)
		{
			pDlgS->LogShow(_T("isConnectGet_out"));
			break;
		}
	}
	return 0;
}

 