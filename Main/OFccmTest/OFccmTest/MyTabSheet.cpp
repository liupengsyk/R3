// MyTabSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTabSheet.h"


// MyTabSheet

IMPLEMENT_DYNAMIC(MyTabSheet, CTabCtrl)

MyTabSheet::MyTabSheet()
{
	m_nNumOfPages =0;
	m_nCurrentPage =0;
}

MyTabSheet::~MyTabSheet()
{
}


BEGIN_MESSAGE_MAP(MyTabSheet, CTabCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()



// MyTabSheet 消息处理程序

BOOL MyTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT IDD)
{
	if ( MAXPAGE == m_nNumOfPages )
		return FALSE;

	//创建对话框
	pDialog->Create(IDD, this);
	pDialog->ShowWindow(SW_HIDE);

	//保存句柄
	m_hPages[m_nNumOfPages] = pDialog->m_hWnd;
	m_Title[m_nNumOfPages]  = title;

	InsertItem(m_nNumOfPages,title);
	SetRect(m_nNumOfPages);
	m_nNumOfPages++; //编号+1
	return TRUE;
}

BOOL MyTabSheet::AddPage(LPCTSTR title, HWND hDlg)
{
	if ( MAXPAGE == m_nNumOfPages )
		return FALSE;

	//修改风格
	CWnd::FromHandle(hDlg)->ModifyStyleEx(WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE,0);
	CWnd::FromHandle(hDlg)->ModifyStyle(WS_POPUP|WS_SIZEBOX|WS_THICKFRAME|WS_CAPTION|DS_MODALFRAME,WS_CHILD);

	m_hPages[m_nNumOfPages] = hDlg;
	m_Title[m_nNumOfPages]  = title;

	::SetParent(hDlg,m_hWnd);
	::ShowWindow(hDlg,SW_HIDE);
	InsertItem(m_nNumOfPages,title);
	SetRect(m_nNumOfPages);
	m_nNumOfPages++;
	return TRUE;
}

//将对话框移动到指定的位置
void MyTabSheet::SetRect(int num)
{
	CRect tabRect, itemRect, wndRect;

	GetClientRect(&tabRect);  //得到客户区尺寸
	GetItemRect(0, &itemRect); //得到选项卡尺寸

	//设置 window在控件的位置
	wndRect.left   = itemRect.left;
	wndRect.right  = tabRect.right-2;
	wndRect.top    = itemRect.bottom + 1;
	wndRect.bottom = tabRect.bottom -4; 

	::MoveWindow(m_hPages[num],wndRect.left,wndRect.top,wndRect.Width(),wndRect.Height(),TRUE);

}

int MyTabSheet::SetCurSel(int nItem)
{
	if (nItem < 0 || nItem >= m_nNumOfPages)
		return -1;

	if (m_nCurrentPage != nItem )
	{
		HideCurrentPage();
		m_nCurrentPage = nItem;
		ShowWorkingCurrentPage();
		CTabCtrl::SetCurSel(nItem);
	}
	else
	{
		ShowWorkingCurrentPage();
	}
	return 0;
}


void MyTabSheet::OnLButtonDown(UINT nFlags, CPoint point)
{
	CTabCtrl::OnLButtonDown(nFlags, point); //此行放在前面，内部判断是否点击选项卡

	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nCurrentPage != GetCurFocus())
	{
 		HideCurrentPage();
 		m_nCurrentPage = GetCurFocus();
		HighlightItem(m_nCurrentPage,TRUE);
		ShowWorkingCurrentPage();
	}

}

void MyTabSheet::ShowWorkingCurrentPage()
{
	::ShowWindow(m_hPages[m_nCurrentPage],SW_SHOW);
}

void MyTabSheet::HideCurrentPage()
{
	::ShowWindow(m_hPages[m_nCurrentPage],SW_HIDE);
}

//删除所有的项目
void MyTabSheet::Init()
{
	int num = GetItemCount();
	for(int i=0; i<num; i++)
	{
		DeleteItem(i);
	}
	m_nCurrentPage = 0;
	m_nNumOfPages  = 0;
}
