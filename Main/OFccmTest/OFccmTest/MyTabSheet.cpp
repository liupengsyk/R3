// MyTabSheet.cpp : ʵ���ļ�
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



// MyTabSheet ��Ϣ�������

BOOL MyTabSheet::AddPage(LPCTSTR title, CDialog *pDialog, UINT IDD)
{
	if ( MAXPAGE == m_nNumOfPages )
		return FALSE;

	//�����Ի���
	pDialog->Create(IDD, this);
	pDialog->ShowWindow(SW_HIDE);

	//������
	m_hPages[m_nNumOfPages] = pDialog->m_hWnd;
	m_Title[m_nNumOfPages]  = title;

	InsertItem(m_nNumOfPages,title);
	SetRect(m_nNumOfPages);
	m_nNumOfPages++; //���+1
	return TRUE;
}

BOOL MyTabSheet::AddPage(LPCTSTR title, HWND hDlg)
{
	if ( MAXPAGE == m_nNumOfPages )
		return FALSE;

	//�޸ķ��
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

//���Ի����ƶ���ָ����λ��
void MyTabSheet::SetRect(int num)
{
	CRect tabRect, itemRect, wndRect;

	GetClientRect(&tabRect);  //�õ��ͻ����ߴ�
	GetItemRect(0, &itemRect); //�õ�ѡ��ߴ�

	//���� window�ڿؼ���λ��
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
	CTabCtrl::OnLButtonDown(nFlags, point); //���з���ǰ�棬�ڲ��ж��Ƿ���ѡ�

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

//ɾ�����е���Ŀ
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
