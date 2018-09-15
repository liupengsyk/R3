
// OFccmTestDlg.h : 头文件
//

#pragma once
#include "OFccmControl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "MyRichEdit.h"
#include "MyTestPanel.h"
#include "MyTestItemOptionDlg.h"
#include "MyIICdebug.h" 
#include "MyPictureCtrl.h"
#include "MachineDlg.h"

//MSXML.DLL(C:\windows\system32\msxml.dll)

//#import <msxml4.dll>
 



// COFccmTestDlg 对话框
class COFccmTestDlg : public CDialogEx
{
	// 构造
public:
	COFccmTestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COFccmTestDlg();

	// 对话框数据
	enum { IDD = IDD_OFCCMTEST_DIALOG };

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
	 
	/************句柄变量定义******/
	HWND hTestDlgWnd; 
	 
	MyPictureCtrl* p_DisplayWnd[2];
	HWND hDisplayWnd[2]; 
 
	MyRichEdit LoggerWnd[2];
	HWND hLogger[2];
	MyTestPanel* m_ButtonPanel[2];
	HWND hPanel[2];
 
public:
	void SetWindowPosition(int Num);
	void SetWindowPosition1();	
	void SetWindowPosition1_1();
	void SetWindowPosition1_0();	
	void SetWindowPosition2();
	void SetWindowPosition2_2();
	 
 
	afx_msg LRESULT ShowTextInfo(WPARAM wParam, LPARAM lParam);
	CStatic m_Info[2];

	afx_msg LRESULT UpdateUI(WPARAM wParam, LPARAM lParam);
	 
	void SaveUIStatus(void);
	map<HWND,BOOL> m_WndStatus_Map;
	void InitBtnStatusMap(void);

	BOOL PreTranslateMessage(MSG* pMsg);

	BOOL OnKey_Num(DWORD wParam);
	BOOL OnKey_CtrlNum(DWORD wParam);
	int OnArrowKey(int nDirection);
	void RecoverOldUIStatus(void);
	 
public:
	LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg  LRESULT CreateSettingDlg(WPARAM wParam, LPARAM lParam);
	afx_msg  LRESULT CreateMachineSettingDlg(WPARAM wParam, LPARAM lParam);
	MyTestItemOptionDlg* p_MyTestItemOptionDlg[2];
	MachineDlg* p_MachineDlg;
 

	int DebugMode;
	static int Cam0_UImode;
	static int Cam1_UImode;
	 
	BOOL bOpen;
	int CamID;
	int TotalNum; 

	void Binding(int CamID);
	void OnPlay(int CamID);
	void RestartExe();
 
	int startTime;
	int ClickTime;
	int TestTime;
	BOOL SetCamCode();

	afx_msg void OnBnClickedBaseset();
	afx_msg void OnBnClickedMachinesetting();
	afx_msg void OnEnterDebugMode();
	afx_msg void OnExitDebugMode();
	afx_msg void OnOpenCamera();
	afx_msg void OnCloseCamera();
	afx_msg void OnBaseSetting();
	afx_msg void OnCameraSetting();
	afx_msg void OnTestItemManager();
	afx_msg void OnTestItemSetting();
	afx_msg void OnMachineSetting();
	afx_msg void OnOpenCameraOnly();
	 
	CMenu menu;  
	int MenuInit();
	afx_msg void OnDebugIIC();
	afx_msg void OnEnterImgTest();
	afx_msg void OnLeaveImgTest();
	  
	afx_msg void OnSaveSingleCamImg();
	afx_msg void OnSaveDualCamImg();
	afx_msg void OnSelectCam0();
	afx_msg void OnSelectCam1();
	void UpdateCamSelect();
	afx_msg void OnGoldenSampleRatio();
//	afx_msg void OnGoldenSampleRatio2();
//	afx_msg void OnGoldenSampleRatio3();
//	afx_msg void OnGoldenSampleRatio4();
	afx_msg void OnGoldenSampleRatio5();
	afx_msg void OnReadFuseID();

private:
		BOOL bGetCamCode;
		void lockUsb();
public:
	 
	void ShowTestInfo();
	void ShowWndInfo(CWnd *m_TestInfo,int Index);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnChangeStrbarcode();
	afx_msg void OnChangeStrbarcode2();
	afx_msg void OnClose();
	virtual void OnOK();
	// added by wzx
	void StartTestOpen(int CamId);
	int nStart;   //第一次进行初始化

	afx_msg LRESULT DestroySettingDlg(WPARAM wParam, LPARAM lParam);
};
