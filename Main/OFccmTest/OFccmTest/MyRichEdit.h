#pragma once
#include "afxcmn.h"
#include "OFccmDef.h"
class MyRichEdit :
	public CRichEditCtrl
{
public:
	DECLARE_MESSAGE_MAP();
public:
	MyRichEdit(void);
	~MyRichEdit(void);
	afx_msg LRESULT Add(WPARAM wparam,LPARAM lparam);
	afx_msg LRESULT Clearlog(WPARAM wparam,LPARAM lparam);
	
public:
	CHARFORMAT DEFcf;
	void InitFont();
	virtual void PreSubclassWindow();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnAdfCopy();
	
};

