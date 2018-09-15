#pragma once


// MTFOptionDlg �Ի���
#include "resource.h"

class Inspection_MTF;

class MTFOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(MTFOptionDlg)

public:
	MTFOptionDlg(CWnd* pParent, Inspection_MTF* pInspection);   // ��׼���캯��
	virtual ~MTFOptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	Inspection_MTF *pInspection;

	UINT m_exp_target;
	UINT m_exp_tolerance;
	double m_HVRatio;
	BOOL m_ExposureEn;
	int m_DequeSize;
	BOOL m_Continue;
	int m_delay;
	BOOL m_tempInfo;
	int m_iUseRaw8Test;
};
