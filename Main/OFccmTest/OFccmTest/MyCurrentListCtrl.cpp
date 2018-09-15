// MyCurrentListCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyCurrentListCtrl.h"


// MyCurrentListCtrl

IMPLEMENT_DYNAMIC(MyCurrentListCtrl, CMFCListCtrl)

MyCurrentListCtrl::MyCurrentListCtrl()
{
	pCurEdit = NULL;
	m_nRow = -1;
	m_nCol = -1;
	
}

MyCurrentListCtrl::~MyCurrentListCtrl()
{
}


BEGIN_MESSAGE_MAP(MyCurrentListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &MyCurrentListCtrl::OnNMDblclk)
	ON_MESSAGE(WM_GETMASKEDEDIT_TEXT, &MyCurrentListCtrl::OnGetEditText)
	ON_NOTIFY_REFLECT(NM_SETFOCUS, &MyCurrentListCtrl::OnNMSetfocus)
END_MESSAGE_MAP()



// MyCurrentListCtrl ��Ϣ�������

void MyCurrentListCtrl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	InsertColumn(1,_T("VolName"),LVCFMT_CENTER,60);
	InsertColumn(2,_T("DownLimit"),LVCFMT_CENTER,70);
	InsertColumn(3,_T("UpLimit"),LVCFMT_CENTER,70);
	
	CMFCListCtrl::PreSubclassWindow();
}



void MyCurrentListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem!=-1)
	{
		m_nRow=pNMListView->iItem;//m_rowΪ��ѡ���е������
		m_nCol=pNMListView->iSubItem;//m_colΪ��ѡ���е������
		if(m_nCol==0) return;	//��һ�в�ѡ��
		if (m_nCol==1)
			pCurEdit = &m_Edit_PinName;
		else
			pCurEdit = &m_Edit_Value;

		CRect rcItem;
		GetSubItemRect(pNMListView->iItem, pNMListView->iSubItem,LVIR_LABEL,rcItem);//ȡ������ľ���

		rcItem.bottom+=3;

		CString text = GetItemText(m_nRow, m_nCol);//ȡ�����������
		pCurEdit->SetWindowText(text);//�������������ʾ���༭����
		CRect rect;
		pCurEdit->MoveWindow(&rcItem);//���༭���ƶ����������棬������������
		pCurEdit->ShowWindow(SW_SHOW);//��ʾ�༭��
		pCurEdit->SetFocus();        //ʹ�༭��ȡ�ý���
		pCurEdit->SetSel(0, -1);      //ʹ����Ƶ������

	}

	*pResult = 0;
}


void MyCurrentListCtrl::ShowCurrentTab(CurrentTab * pCurrentTab)
{
	 
	for (int i = 0; i< DTVOL_MAX; i++)
	{
		CString num;
		num.Format(_T("%d"),i+1);
		InsertItem(i,num);
		SetItemText(i,1,pCurrentTab->Pin[i].Name);

		CString limit;
		limit.Format(_T("%.3lf"),pCurrentTab->Pin[i].CurrentLowLimit);
		SetItemText(i, 2, limit);

		limit.Format(_T("%.3lf"),pCurrentTab->Pin[i].CurrentUpLimit);
		SetItemText(i, 3, limit);
	}

	for (int i = 0; i<GetItemCount(); i++)
	{
		if (i<32)
		{
			if (pCurrentTab->CurrentMask_L & (1 << i))
				SetCheck(i,TRUE);
			else
				SetCheck(i,FALSE);
		}
		else
		{
			if (pCurrentTab->CurrentMask_H & (1 << i))
				SetCheck(i,TRUE);
			else
				SetCheck(i,FALSE);
		}		
	}
}


void MyCurrentListCtrl::Save(CurrentTab & m_CurrentTab)
{
	 
	for (int i = 0; i< DTVOL_MAX; i++)
	{
		m_CurrentTab.Pin[i].Name = GetItemText(i,1);
		swscanf_s(GetItemText(i,2),_T("%lf"), &m_CurrentTab.Pin[i].CurrentLowLimit);
		swscanf_s(GetItemText(i,3),_T("%lf"), &m_CurrentTab.Pin[i].CurrentUpLimit);
	}

	for (int i = 0; i<DTVOL_MAX; i++)
	{
		if (i<32)
		{
			if( GetCheck(i) )
				SET_BIT(m_CurrentTab.CurrentMask_L,i);
			else
				CLR_BIT(m_CurrentTab.CurrentMask_L,i);
		}
		else
		{
			int k = i -32;
			if( GetCheck(i) )
				SET_BIT(m_CurrentTab.CurrentMask_H,k);
			else
				CLR_BIT(m_CurrentTab.CurrentMask_H,k);
		}		
	}
}

 

 LRESULT MyCurrentListCtrl::OnGetEditText(WPARAM wParam, LPARAM lParam)
{
	SendMessage(WM_SETFOCUS);
	return 0;
}



void MyCurrentListCtrl::OnNMSetfocus(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��list�ؼ���ȡ������ʱ��
	if (pCurEdit!=NULL)
	{
		if(pCurEdit->IsWindowVisible())
		{
			//����editֵ���ؼ�
			CString str;
			pCurEdit->GetWindowTextW(str);
			SetItemText(m_nRow,m_nCol,str);
		}
	}
	m_Edit_PinName.ShowWindow(SW_HIDE);
	m_Edit_Value.ShowWindow(SW_HIDE);
	*pResult = 0;
}

