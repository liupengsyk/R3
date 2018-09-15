#pragma once


// MyTestButton

class MyTestButton : public CMFCButton
{
	DECLARE_DYNAMIC(MyTestButton)

public:
	MyTestButton(int CamID);
	virtual ~MyTestButton();

protected:
	DECLARE_MESSAGE_MAP()
 
public:
	int m_ID;
	CFont Font1;
	CFont Font2;
	CFont Font3;
	CFont Font4;

	int CamID;
	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PostNcDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT ShowStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT AlignWndPos(WPARAM wParam, LPARAM lParam);
	int MaxItemSize;
};


