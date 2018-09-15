#pragma once
#include "DeviceInterface.h"

// MyIICdebug 对话框

class MyIICdebug : public CDialogEx
{
	DECLARE_DYNAMIC(MyIICdebug)

public:
	MyIICdebug(int CamID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MyIICdebug();

// 对话框数据
	enum { IDD = IDD_IICdebug };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Address;
	CString m_Value;
	CString m_Register;
	virtual BOOL OnInitDialog();
	CComboBox m_I2CMode;

	CFont font_Edit;
	CFont font_Title;
	CFont font_Combox;
	void InitFont(void);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_StdI2C;
 

	CSliderCtrl m_ExpTimeCtrl;
	CSliderCtrl m_ExpGainCtrl;

	 
	void InitExposureDebug(void);
	void InitColorDebug(void);
	void InitI2C(void);

	afx_msg void OnNMReleasedcaptureSliderExpTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderExpGain(NMHDR *pNMHDR, LRESULT *pResult);
 
	afx_msg void OnCbnSelchangeModeCombo();

	int Type_Reg;
	int Type_Val;
	CString Format_Val;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRead();
	
	 
	SensorTab *pDebugSensorTab;
	int CamID;	 
	int DeviceID;
	void ShowCurrentY();

 
	afx_msg void OnBnClickedWriteafcode();
	afx_msg void OnBnClickedbsaveexp();
	afx_msg void OnBnClickedbtnsaveexp();
 
 
};
