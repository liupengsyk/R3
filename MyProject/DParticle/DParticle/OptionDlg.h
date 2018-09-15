#pragma once


// OptionDlg 对话框
#include "resource.h"

class DParticle_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, DParticle_TestItem* pTestItem);   // 标准构造函数
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
	DParticle_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	int nLeftSL;
	int nRightSL;
	int nTopSL;
	int nBottomSL;
	int nAreaWidth;
	int nAreaHeight;
	int nDeadUnit;
	int nDeadSpec;
	int nDefultExposure;
	int nDefultGain;
	int nDeadType;
	BOOL bSaveImage;
	int ErrorCode;
};
