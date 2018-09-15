// MyTestItemOptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MyTestItemOptionDlg.h"
#include "afxdialogex.h"
#include "MyTestItem.h"


// MyTestItemOptionDlg �Ի���

IMPLEMENT_DYNAMIC(MyTestItemOptionDlg, CDialogEx)

MyTestItemOptionDlg::MyTestItemOptionDlg(int CamID,CWnd* pParent /*=NULL*/)
	: CDialogEx(MyTestItemOptionDlg::IDD, pParent)
{
	this->CamID = CamID;
}

MyTestItemOptionDlg::~MyTestItemOptionDlg()
{

}

void MyTestItemOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OptionDlgTab, m_MyTabSheet);
}

 

BEGIN_MESSAGE_MAP(MyTestItemOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_APPLY_SETTING, &MyTestItemOptionDlg::OnBnClickedApplySetting)	 
	ON_BN_CLICKED(IDCANCEL, &MyTestItemOptionDlg::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGING, IDC_OptionDlgTab, &MyTestItemOptionDlg::OnSelchangingOptiondlgtab)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// InspectionOptionDlg ��Ϣ�������


BOOL MyTestItemOptionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_MyTabSheet.Init(); 

	//����DLL������ҳ
	vector<MyTestItem>* pArray = pCcmCtrl->Cam[CamID].pTestCtrl->GetMyTestItemArray();

	vector<MyTestItem>::iterator it;
	for(it = pArray->begin(); it != pArray->end(); it++)
	{
		LPCTSTR name = it->pItem->GetName();
		HWND    hDlg = it->pItem->GetOptionDialogHandle();
		m_MyTabSheet.AddPage(name,hDlg);
	}
	m_MyTabSheet.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
 

void MyTestItemOptionDlg::OnBnClickedApplySetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret = pCcmCtrl->Cam[CamID].pTestCtrl->SaveAllOption();
	int i=m_MyTabSheet.GetCurSel();
	pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem[i].pItem->SaveOption();
	if (ret>0)
	{

	}
	CDialogEx::OnOK();
}


 

void MyTestItemOptionDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	

	CDialogEx::OnCancel();
}


void MyTestItemOptionDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	/*	delete this;*/
	CDialogEx::PostNcDestroy();
}

void MyTestItemOptionDlg::OnSelchangingOptiondlgtab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int i=m_MyTabSheet.GetCurSel();
	int ret = pCcmCtrl->Cam[CamID].pTestCtrl->VecTestItem[i].pItem->SaveOption();
	pCcmCtrl->Cam[CamID].pTestCtrl->LoadAllOption();

	if (ret>0)
	{

	}
	*pResult = 0;
}
