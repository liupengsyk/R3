#pragma once

#include "MyPropertyGridCtrl.h"
#include "MyOSListCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// MyCaneraSettingPage 对话框

class MyCameraSettingPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(MyCameraSettingPage)

public:
	MyCameraSettingPage(int CamID);
	virtual ~MyCameraSettingPage();

// 对话框数据
	enum { IDD = IDD_CameraSetPage };

	MyPropertyGridCtrl* pProGridCtrl;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	 
	void SavePage();
	void ApplyPage();
	void InitPage();
 
	BOOL m_CheckOS;//负向
	BOOL m_CheckPositiveOS;//正向
	BOOL m_CheckSensorpinOS;//两两pin测试
	MyOSListCtrl* m_OSList;
	afx_msg void OnBnClickedCheckOs();
	int CamID; 
	 
	virtual BOOL OnSetActive();
};
