#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

// GeneralSettingPage 对话框

class GeneralSettingPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(GeneralSettingPage)

public:
	GeneralSettingPage(int CamID);   // 标准构造函数
	virtual ~GeneralSettingPage();

// 对话框数据
	enum { IDD = IDD_GeneralSetingPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	 
	afx_msg BOOL OnInitDialog();
	 

	BOOL bForceShow;
	/*BOOL bUseMes;
	int MesID;*/
	 

public:

	void SavePage();
	  
	void GetbCheckIni();
	  
	BOOL m_bSaveReportEn;
	 
	CString m_strIP;
	int m_iPort;
	BOOL SocketEn;

	int CamID;
	BOOL bUseLocalExp;
	 
	afx_msg void OnBnClickedUpdatareportpath();
	CString strTestMode;	
	CString GetTestMode(int m_TestMode);
	//int m_TestMode;
	int GetTestMode(CString strTestMode);

	CString DevName;
	CString sReportPath;

};
