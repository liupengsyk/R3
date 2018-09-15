#pragma once


// OptionDlg 对话框
#include "resource.h"

class QualcommGainmap_L4_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, QualcommGainmap_L4_TestItem* pTestItem);   // 标准构造函数
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
	QualcommGainmap_L4_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int m_CFA;
	int m_BLC;
	int m_BIT;
	int m_channel;
	int m_block_Width;
	int m_Block_Height;
	int m_PD_pairs;
	int m_Start_X;
	int m_Start_Y;
	CString m_PD_Block_Right_X;
	CString m_PD_Block_Right_Y;
	CString m_PD_Block_Left_X;
	CString m_PD_Block_Left_Y;
	
	int m_save;
	int m_GainmapMAX;
	int m_GainmapMIN;
	BOOL m_savealldata;
	BOOL m_is2PD;
};
