#pragma once


// MySrcListCtrl

class MySrcListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(MySrcListCtrl)

public:
	MySrcListCtrl();
	virtual ~MySrcListCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
public:
	CFont m_font;

	virtual void PreSubclassWindow();

	virtual COLORREF OnGetCellBkColor(int nRow, int nColum);
};


