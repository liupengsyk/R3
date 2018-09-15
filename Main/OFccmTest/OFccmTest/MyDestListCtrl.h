#pragma once


// MyDestListCtrl

class MyDestListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(MyDestListCtrl)

public:
	MyDestListCtrl();
	virtual ~MyDestListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CFont m_font;

	virtual void PreSubclassWindow();

	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	int m_iItemDrag;
	int m_iItemDrop;
	CImageList* m_pimageListDrag;

	BOOL m_bDragging;
	CPoint m_ptHotSpot;
	void ListSort();
};


