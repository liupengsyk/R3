#pragma once
#include "MyMaskedEdit.h"

// MyCurrentListCtrl

class MyCurrentListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(MyCurrentListCtrl)

public:
	MyCurrentListCtrl();
	virtual ~MyCurrentListCtrl();

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
	void ShowCurrentTab(CurrentTab * pCurrentTab);
	 
	CFont m_font;
	void Save(CurrentTab & m_CurrentTab);
 
protected:
	afx_msg LRESULT OnGetEditText(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult);
};


