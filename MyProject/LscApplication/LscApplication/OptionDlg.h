#pragma once


// OptionDlg 对话框
#include "resource.h"

class LscApplication_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, LscApplication_TestItem* pTestItem);   // 标准构造函数
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
	LscApplication_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_QLSC;
	BOOL m_MLSC;
	BOOL m_SLSC;


	int m_SaveFlag;
	BOOL m_LSCMode;
	int m_BLC;
	BOOL m_MLSC_M;
	int m_MTKSize;
	int m_RMax;
	int m_RMin;
	int m_GrMax;
	int m_GrMin;
	int m_GbMax;
	int m_GbMin;
//	CEdit m_BMax;
	int m_BMax;
	int m_BMin;
	int m_LSCDistance;
	int m_xblock;
	int m_yblock;
	int m_OBvalue;
	int Sensor_LSCTarget;
	int Sensor_LSCGroup;
	int Sensor_OBvalue;
	int nLenCRegCount;
	int LSCerror;
	int RGMAX;
	int RGMIN;
	int BGMAX;
	int BGMIN;
	BOOL m_GetDATForS5K4H7YX;
    int m_MaxYDecay;
    int m_MinYDecay;
	int m_GrGbDiff;
	int m_YDecayDiff;
};
