#pragma once
#include "MyMaskedEdit.h"

// MyOSListCtrl

class MyOSListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(MyOSListCtrl)

public:
	MyOSListCtrl(int CamID);
	virtual ~MyOSListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
	MyMaskedEdit m_Edit_Value;
	MyMaskedEdit m_Edit_PinName;
	MyMaskedEdit* pCurEdit;
	int m_nRow;
	int m_nCol;
	virtual void PreSubclassWindow();
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowOStab(void);
	CFont m_font;
	void Save(void);
protected:
	afx_msg LRESULT OnGetEditText(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult);
	int CamID;
	 
};


