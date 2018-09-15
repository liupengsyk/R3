#pragma once

#include "MyPropertyGridCtrl.h"
#include "MyOSListCtrl.h"
#include "afxcmn.h"
#include "afxwin.h"

// MyCaneraSettingPage �Ի���

class MyCameraSettingPage : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(MyCameraSettingPage)

public:
	MyCameraSettingPage(int CamID);
	virtual ~MyCameraSettingPage();

// �Ի�������
	enum { IDD = IDD_CameraSetPage };

	MyPropertyGridCtrl* pProGridCtrl;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	 
	void SavePage();
	void ApplyPage();
	void InitPage();
 
	BOOL m_CheckOS;//����
	BOOL m_CheckPositiveOS;//����
	BOOL m_CheckSensorpinOS;//����pin����
	MyOSListCtrl* m_OSList;
	afx_msg void OnBnClickedCheckOs();
	int CamID; 
	 
	virtual BOOL OnSetActive();
};
