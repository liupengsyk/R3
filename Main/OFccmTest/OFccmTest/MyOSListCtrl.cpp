// MyOSListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyOSListCtrl.h"



// MyOSListCtrl

IMPLEMENT_DYNAMIC(MyOSListCtrl, CMFCListCtrl)

MyOSListCtrl::MyOSListCtrl(int CamID)
{
	pCurEdit = NULL;
	m_nRow = -1;
	m_nCol = -1;
	this->CamID = CamID;
}

MyOSListCtrl::~MyOSListCtrl()
{
}


BEGIN_MESSAGE_MAP(MyOSListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &MyOSListCtrl::OnNMDblclk)
	ON_MESSAGE(WM_GETMASKEDEDIT_TEXT, &MyOSListCtrl::OnGetEditText)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, &MyOSListCtrl::OnNMSetfocus)
END_MESSAGE_MAP()



// MyOSListCtrl 消息处理程序

void MyOSListCtrl::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_Edit_Value.CreateEx(WS_EX_CLIENTEDGE,_T("EDIT"),NULL,WS_CHILD|ES_CENTER,CRect(0,0,1,1),this,1);
	m_Edit_PinName.CreateEx(WS_EX_CLIENTEDGE,_T("EDIT"),NULL,WS_CHILD|ES_CENTER,CRect(0,0,1,1),this,1);
	m_Edit_Value.EnableMask(_T("*****"), _T("_____"),_T(' '),_T(".0123456789"));

	m_font.CreatePointFont(100,_T("Verdana"));
	SetFont(&m_font);
	DWORD  dwStyle = GetExtendedStyle();
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_CHECKBOXES;

	SetExtendedStyle(dwStyle);

	InsertColumn(0,_T("ID"),LVCFMT_CENTER,50);
	InsertColumn(1,_T("PinName"),LVCFMT_CENTER,60);
	InsertColumn(2,_T("OpenLimit"),LVCFMT_CENTER,70);
	InsertColumn(3,_T("ShortLimit"),LVCFMT_CENTER,70);

	ShowOStab();

	CMFCListCtrl::PreSubclassWindow();
}


void MyOSListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem!=-1)
	{
		m_nRow=pNMListView->iItem;//m_row为被选中行的行序号
		m_nCol=pNMListView->iSubItem;//m_col为被选中行的列序号
		if(m_nCol==0) return;	//第一列不选中
		if (m_nCol==1)
			pCurEdit = &m_Edit_PinName;
		else
			pCurEdit = &m_Edit_Value;

		CRect rcItem;
		GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rcItem);//取得子项的矩形

		rcItem.bottom+=3;

		CString text = GetItemText(m_nRow, m_nCol);//取得子项的内容
		pCurEdit->SetWindowText(text);//将子项的内容显示到编辑框中
		CRect rect;
		pCurEdit->MoveWindow(&rcItem);//将编辑框移动到子项上面，覆盖在子项上
		pCurEdit->ShowWindow(SW_SHOW);//显示编辑框
		pCurEdit->SetFocus();        //使编辑框取得焦点
		pCurEdit->SetSel(0, -1);      //使光标移到最后面

	}

	*pResult = 0;
}



void MyOSListCtrl::ShowOStab(void)
{
	OSTab * pOStab = pCcmCtrl->Cam[CamID].pConfig->GetOSTab();
	for (int i = 0; i< DTPIN_MAX; i++)
	{
		CString num;
		num.Format(_T("%d"),i+1);
		InsertItem(i,num);
		SetItemText(i,1,pOStab->Pin[i].Name);

		CString limit;
		limit.Format(_T("%.3lf"),pOStab->Pin[i].OpenLimit);
		SetItemText(i, 2, limit);

		limit.Format(_T("%.3lf"),pOStab->Pin[i].ShortLimit);
		SetItemText(i, 3, limit);
	}

	for (int i = 0; i<GetItemCount(); i++)
	{
		if (i<32)
		{
			if (pOStab->PinMask_L & (1 << i))
				SetCheck(i,TRUE);
			else
				SetCheck(i,FALSE);
		}
		else
		{
			if (pOStab->PinMask_H & (1 << i))
				SetCheck(i,TRUE);
			else
				SetCheck(i,FALSE);
		}		
	}
}


void MyOSListCtrl::Save(void)
{
	OSTab * pOStab = pCcmCtrl->Cam[CamID].pConfig->GetOSTab();
	for (int i = 0; i< DTPIN_MAX; i++)
	{
		pOStab->Pin[i].Name = GetItemText(i,1);
		swscanf_s(GetItemText(i,2),_T("%lf"), &pOStab->Pin[i].OpenLimit);
		swscanf_s(GetItemText(i,3),_T("%lf"), &pOStab->Pin[i].ShortLimit);
	}

	for (int i = 0; i<DTPIN_MAX; i++)
	{
		if (i<32)
		{
			if( GetCheck(i) )
				SET_BIT(pOStab->PinMask_L,i);
			else
				CLR_BIT(pOStab->PinMask_L,i);
		}
		else
		{
			int k = i -32;
			if( GetCheck(i) )
				SET_BIT(pOStab->PinMask_H,k);
			else
				CLR_BIT(pOStab->PinMask_H,k);
		}		
	}
}


afx_msg LRESULT MyOSListCtrl::OnGetEditText(WPARAM wParam, LPARAM lParam)
{
	SendMessage(WM_SETFOCUS);
	return 0;
}



void MyOSListCtrl::OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//当list控件获取到焦点时候
	if (pCurEdit!=NULL)
	{
		if(pCurEdit->IsWindowVisible())
		{
			//传递edit值到控件
			CString str;
			pCurEdit->GetWindowTextW(str);
			SetItemText(m_nRow,m_nCol,str);
		}
	}
	m_Edit_PinName.ShowWindow(SW_HIDE);
	m_Edit_Value.ShowWindow(SW_HIDE);
	*pResult = 0;
}

