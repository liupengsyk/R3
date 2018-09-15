#pragma once

#define MAXPAGE 40

// MyTabSheet

class MyTabSheet : public CTabCtrl
{
	DECLARE_DYNAMIC(MyTabSheet)

public:
	MyTabSheet();
	virtual ~MyTabSheet();
 
public:
	int SetCurSel(int nItem);
	void SetRect(int num);
	BOOL AddPage(LPCTSTR title, CDialog *pDialog, UINT ID);
	BOOL AddPage(LPCTSTR title, HWND hDlg); //DLL ∂‘ª∞øÚº”‘ÿ
	void ShowWorkingCurrentPage();
	void HideCurrentPage();
	void Init();

protected:
	DECLARE_MESSAGE_MAP()

	LPCTSTR m_Title[MAXPAGE];
	HWND m_hPages[MAXPAGE];
	int m_nNumOfPages;
	int m_nCurrentPage;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


