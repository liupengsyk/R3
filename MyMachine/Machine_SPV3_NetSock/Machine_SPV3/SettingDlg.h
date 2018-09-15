#pragma once
#include "resource.h"
#include <vector>
#include "afxcmn.h"

using namespace std;

// SettingDlg 对话框
class Machine_NCO_SPV3;
 
class SettingDlg : public CDialog
{
	DECLARE_DYNAMIC(SettingDlg)

public:
	SettingDlg(CWnd* pParent ,Machine_NCO_SPV3 *pMachine);   // 标准构造函数
	virtual ~SettingDlg();

// 对话框数据
	enum { IDD = IDD_SettingDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	Machine_NCO_SPV3 *pMachine;

public:  
	   
	// 测试站别列表
	CListCtrl StationList;
	afx_msg void OnBnClickedButtonAddStation();

	afx_msg void OnBnClickedButtonDeleteStation();

	int AddStationCheck(); 

	static int lastPos; 
	afx_msg void OnLvnItemchangedStationList(NMHDR *pNMHDR, LRESULT *pResult);

	void UI_Update();
	CString ipAddr;
	vector<CString> ipvec;
	// 使用error 异常指令，handle需要全程监控并响应该指令
	BOOL error_en;
	// 测试false时继续执行测试流程
	
};
