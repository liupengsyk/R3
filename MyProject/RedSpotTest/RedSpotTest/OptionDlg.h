#pragma once


// OptionDlg �Ի���
#include "resource.h"



class RedSpot_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, RedSpot_TestItem* pTestItem);   // ��׼���캯��
	virtual ~OptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_REDSPOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	RedSpot_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	CString strExpVal;
	CString strGainVal;
	CString strExposureRegHig;
	CString strExposureRegLow;
	CString strGainRegHig;
	CString strGainRegLow;
	int iManualExDelayFrame;
	BOOL bManualExposure;
	int Redspot_Ymax;
	int Redspot_Rmax;
	int Redspot_Gmax;
	int Redspot_Bmax;
	int XBlockNum;
	int YBlockNum;
};
