#pragma once


// MyPictureCtrl

class MyPictureCtrl : public CStatic
{
	DECLARE_DYNAMIC(MyPictureCtrl)

public:
	MyPictureCtrl(int CamID);
	virtual ~MyPictureCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	int CamID;
};


