
// Handle_Mdp01Dlg.h : 头文件
//

#pragma once

#include "M_DP01\lib\export.h"
#include "ConfigFileRw.h"
#include "SocketClient.h"
#include <afxsock.h>
#pragma comment(lib,"M_DP01\\lib\\Release\\vcm_plc_dll.lib")
 
#define TEST_MSG 0xEEEF
#define DIS_CON_MSG 0xEE02
  
//保留双工位，增加至三个测试站
struct CameraControl
{
	CString CamStationStartTest[2][5];//CamStationStartTest[i][j],表示一个dual模组的单个camera，j表示站位     发送的消息
	CString CamStationTestResultPass[2][5];//CamStationStartTest[i][j],表示一个dual模组的单个camera，j表示站位  接收的消息
	CString CamStationTestResultFail[2][5];//CamStationStartTest[i][j],表示一个dual模组的单个camera，j表示站位  接收的消息

	BOOL StationTestResult;//最终判断结果
};
 

// CHandle_Mdp01Dlg 对话框
class CHandle_Mdp01Dlg : public CDialogEx
{
// 构造
public:
	CHandle_Mdp01Dlg(CWnd* pParent = NULL);	// 标准构造函数
	~CHandle_Mdp01Dlg();	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HANDLE_MDP01_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
 