// CamCodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "CamCodeDlg.h"
#include "afxdialogex.h"


// CamCodeDlg �Ի���

IMPLEMENT_DYNAMIC(CamCodeDlg, CDialogEx)

CamCodeDlg::CamCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CamCodeDlg::IDD, pParent)
{
	CamCode =_T("");
}

CamCodeDlg::~CamCodeDlg()
{

}

void CamCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_CamCode,CamCode);
	DDX_Text(pDX,IDC_CamCode,CamRunChard);
	
}


BEGIN_MESSAGE_MAP(CamCodeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CamCodeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CamCodeDlg ��Ϣ�������
BOOL CamCodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetDlgItem(IDC_CamCode)->SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CamCodeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
