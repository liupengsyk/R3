#pragma once


// OptionDlg 对话框
#include "resource.h"

class HWBlemish_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, HWBlemish_TestItem* pTestItem);   // 标准构造函数
	virtual ~OptionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	HWBlemish_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString m_P1;
	CString m_P2;
	CString m_P3;
	CString m_P4;
	int m_FrameCount;
	int m_ShowTime;

	int m_DelayTimes;


	BOOL m_LoadImgEn;
	int m_Width;
	int m_Height;
	CString m_ImgPath;
	CString m_ImgName;
	int m_ErrorCode;
};
