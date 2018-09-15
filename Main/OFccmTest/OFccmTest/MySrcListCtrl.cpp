// MySrcListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MySrcListCtrl.h"


// MySrcListCtrl

IMPLEMENT_DYNAMIC(MySrcListCtrl, CMFCListCtrl)

MySrcListCtrl::MySrcListCtrl()
{

}

MySrcListCtrl::~MySrcListCtrl()
{
}


BEGIN_MESSAGE_MAP(MySrcListCtrl, CMFCListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// MySrcListCtrl 消息处理程序

void MySrcListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_font.CreatePointFont(100,_T("Verdana"));
	SetFont(&m_font);
	DWORD  dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;

	SetExtendedStyle(dwStyle);

	CMFCListCtrl::PreSubclassWindow();
}

COLORREF MySrcListCtrl::OnGetCellBkColor(int nRow, int nColum)
{
	int index = GetSelectionMark();

	if (nRow != index)
	{	
		return RGB(255,255,255);
	}
	return RGB(220,200,200);
}
