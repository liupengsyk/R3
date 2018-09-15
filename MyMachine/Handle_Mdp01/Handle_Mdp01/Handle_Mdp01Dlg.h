
// Handle_Mdp01Dlg.h : ͷ�ļ�
//

#pragma once

#include "M_DP01\lib\export.h"
#include "ConfigFileRw.h"
#include "SocketClient.h"
#include <afxsock.h>
#pragma comment(lib,"M_DP01\\lib\\Release\\vcm_plc_dll.lib")
 
#define TEST_MSG 0xEEEF
#define DIS_CON_MSG 0xEE02
  
//����˫��λ����������������վ
struct CameraControl
{
	CString CamStationStartTest[2][5];//CamStationStartTest[i][j],��ʾһ��dualģ��ĵ���camera��j��ʾվλ     ���͵���Ϣ
	CString CamStationTestResultPass[2][5];//CamStationStartTest[i][j],��ʾһ��dualģ��ĵ���camera��j��ʾվλ  ���յ���Ϣ
	CString CamStationTestResultFail[2][5];//CamStationStartTest[i][j],��ʾһ��dualģ��ĵ���camera��j��ʾվλ  ���յ���Ϣ

	BOOL StationTestResult;//�����жϽ��
};
 

// CHandle_Mdp01Dlg �Ի���
class CHandle_Mdp01Dlg : public CDialogEx
{
// ����
public:
	CHandle_Mdp01Dlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CHandle_Mdp01Dlg();	// ��׼���캯��


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HANDLE_MDP01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnKillfocusComboCom();
	BOOL InitCommPort();
	BOOL EnumPortList(CStringArray* str_array);
	BOOL OpenCommPort(CString strComm);
	SocketClient m_SocketClient;

	static UINT WorkThread(void* lparam);
	 

	static UINT ControlerThread(void* lparam);
	int Controler();
	void PostMsgToControl(int Msg,int Station);
	BOOL bControlThreadOn;
	int SendCamMsg(int CamID, int StationID);
	int ControlThreadID;
	int CurrentStationID;
	void WaittingStart();
	void Working();



	void TestStation();

	//dp01 control
	int Home();
	int MoveToNearPos();
	int MoveToFarPos();
	int bMoveCompelete();
	int SpinCarrier(BOOL bVertical);
	int GetStart();
	int StopMove();
	int SetCam0_Pass();
	int SetCam0_Fail();
	 
	int SetLightRed();
	int SetLightGreen();
	int SetLightYellow();
	int SetLightRing();
	int SetAlarm();
 
	 
	BOOL InitDp01();

	int NearPos;
	int FarPos;
	CString CommPort;
	CString IP_PC;
	int Port_PC; 
	 
	BOOL bWorking;	 

	afx_msg void OnBnClickedButtonHome();
	afx_msg void OnBnClickedButtonNearpos();
	afx_msg void OnBnClickedButtonFarpos();
public:
	 CameraControl m_CameraControl;
private:
	ConfigFileRw m_ConfigFileRw;
	
	void CameraControlInit();
	CString CurrentPath;
	void SetCurrentPath(); 
	void LoadOption();
	void SaveOption();	

public:
	afx_msg void OnBnClickedButtonSave();
	//int PC01;
	BOOL ConnectToServer();
	  
private:
	void LogCreate();
	CString logPath;
public:
	void AddLog(CString strLog);
	afx_msg void OnBnClickedButtonOpencomm();
	void LogShow(CString Info);
private:
	CHARFORMAT DEFcf;
	CRichEditCtrl* LogBoard;
public:
 
	afx_msg void OnBnClickedButtonStop();
	BOOL bCommOn;

	BOOL bMoveBack;

	BOOL is90;
};
 