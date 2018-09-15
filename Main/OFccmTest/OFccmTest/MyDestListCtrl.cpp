// MyDestListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyDestListCtrl.h"


// MyDestListCtrl

IMPLEMENT_DYNAMIC(MyDestListCtrl, CMFCListCtrl)

MyDestListCtrl::MyDestListCtrl()
{
	m_bDragging = FALSE;
	m_pimageListDrag = NULL;
}

MyDestListCtrl::~MyDestListCtrl()
{	 
	if (m_pimageListDrag != NULL)
	{
		delete m_pimageListDrag;
		m_pimageListDrag = NULL;
	}
}


BEGIN_MESSAGE_MAP(MyDestListCtrl, CMFCListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &MyDestListCtrl::OnLvnBegindrag)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// MyDestListCtrl 消息处理程序


void MyDestListCtrl::PreSubclassWindow()
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

COLORREF MyDestListCtrl::OnGetCellBkColor(int nRow, int nColum)
{
	int index = GetSelectionMark();

	if (nRow != index)
	{	
		return RGB(255,255,255);
	}
	return RGB(220,200,200);
}




void MyDestListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	if (m_bDragging)
	{
		ASSERT(m_pimageListDrag != NULL);
		m_pimageListDrag-> DragMove(point); // move the image
		int nHit = HitTest(point);
		if (nHit != -1)
		{
			//			SetItemState(m_iItemDrop, 0, LVIS_SELECTED);
			m_iItemDrop = nHit;
			SetSelectionMark(m_iItemDrop);
			//			SetItemState(m_iItemDrop, LVIS_SELECTED, LVIS_SELECTED);   //选中行
		}
		else
		{
			m_iItemDrop = -1;
		}
	}

	CMFCListCtrl::OnMouseMove(nFlags, point);
}




void MyDestListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CPoint ptItem, //
		ptAction, //事件发生的位置 
		ptImage;//移动位图的位置
	NM_LISTVIEW *pnmListView = (NM_LISTVIEW *)pNMHDR;
	ASSERT(!m_bDragging);
	m_bDragging = TRUE;
	//获得选种的项的索引
	m_iItemDrag = pnmListView-> iItem; 
	//获得事件发生的位置
	ptAction = pnmListView-> ptAction;
	//获得项的位置
	GetItemPosition(m_iItemDrag, &ptItem); // ptItem is relative to (0,0) and not the view origin


	ASSERT(m_pimageListDrag == NULL);
	m_pimageListDrag = CreateDragImage(m_iItemDrag, &ptImage);
	IMAGEINFO imageInfo;
	m_pimageListDrag->GetImageInfo(0,&imageInfo);
	m_ptHotSpot = ptAction - ptItem ; // calculate hotspot for the cursor
	m_pimageListDrag-> DragShowNolock(TRUE); // lock updates and show drag image

	m_pimageListDrag-> BeginDrag(0, m_ptHotSpot);

	m_pimageListDrag-> DragEnter(this,ptAction);

	//锁定鼠标
	SetCapture();
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);
	ClipCursor(rect);

	//	AfxMessageBox(_T("drag"));
}


void MyDestListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CMFCListCtrl::OnLButtonUp(nFlags, point);

	if (m_bDragging) // end of the drag operation
	{
		ASSERT(m_pimageListDrag != NULL);
		m_pimageListDrag-> DragLeave(this);
		m_pimageListDrag-> EndDrag();
		delete m_pimageListDrag;
		m_pimageListDrag = NULL;

		m_bDragging = FALSE;

		::ReleaseCapture();
		ClipCursor(NULL);

		if (m_iItemDrop == -1)
		{
			m_iItemDrop = GetItemCount();
		}

		if (m_iItemDrag < m_iItemDrop)
		{
			InsertItem(++m_iItemDrop, GetItemText(m_iItemDrag,0));
			DeleteItem(m_iItemDrag);
		}
		else
		{
			InsertItem(m_iItemDrop, GetItemText(m_iItemDrag,0));
			DeleteItem(++m_iItemDrag);
		}
		ListSort();
		Invalidate();
	}


}

//序号重排
void MyDestListCtrl::ListSort()
{
	int size = GetItemCount();
	for (int i = 0; i < size; i++)
	{
		CString szNum;
		szNum.Format(_T("%d"),i+1);
		SetItemText(i,1,szNum);
	}
}
