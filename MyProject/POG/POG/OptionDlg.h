#pragma once


// OptionDlg �Ի���
#include "resource.h"

class POG_TestItem;

class OptionDlg : public CDialog
{
	DECLARE_DYNAMIC(OptionDlg)

public:
	OptionDlg(CWnd* pParent, POG_TestItem* pTestItem);   // ��׼���캯��
	virtual ~OptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	afx_msg LRESULT OnUpdateData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCtrlKeyNum(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnCameraStop(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	POG_TestItem* m_pTestItem;
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_pogCamera0saveImage;
	BOOL m_pogCamera0saveLog;
	CString m_pogCamera0medianA;
	CString m_pogCamera0medianB;
	CString m_pogCamera0axStart;
	CString m_pogCamera0ayStart;
	CString m_pogCamera0offset;
	CString m_pogCamera0cornerDiff;
	CString m_pogCamera0edgeLimit;
	CString m_pogCamera0threshold;
	CString m_pogCamera0pixelLimit;
	CString m_pogCamera0axEnd;
	CString m_pogCamera0ayEnd;
	CString m_pogCamera0intensity;
	CString m_pogCamera0rangeSpc;
	CString m_pogCamera0edgePixelLimit;

	BOOL m_pogCamera0Enable;

	BOOL m_pogCamera1saveImage;
	BOOL m_pogCamera1saveLog;
	CString m_pogCamera1medianA;
	CString m_pogCamera1medianB;
	CString m_pogCamera1axStart;
	CString m_pogCamera1ayStart;
	CString m_pogCamera1offset;
	CString m_pogCamera1cornerDiff;
	CString m_pogCamera1edgeLimit;
	CString m_pogCamera1threshold;
	CString m_pogCamera1pixelLimit;
	CString m_pogCamera1axEnd;
	CString m_pogCamera1ayEnd;
	CString m_pogCamera1intensity;
	CString m_pogCamera1rangeSpc;
	CString m_pogCamera1edgePixelLimit;
};
