// RawOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "RawOptionDlg.h"
#include "afxdialogex.h"


// RawOptionDlg 对话框

IMPLEMENT_DYNAMIC(RawOptionDlg, CDialogEx)

RawOptionDlg::RawOptionDlg(CWnd* pParent /*=NULL*/)
	 : CDialogEx(RawOptionDlg::IDD, pParent),
	 m_Raw_Width(0),m_Raw_Height(0), RawBits(0),
	 RawOutForamt(0),m_ShowType(0),m_FileLength(0),
	 strRawOutForamt(_T("")),strm_ShowType(_T(""))
{ 

}

RawOptionDlg::~RawOptionDlg()
{
}

void RawOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_Raw_Width,m_Raw_Width);
	DDX_Text(pDX,IDC_EDIT_Raw_Height,m_Raw_Height);
	DDX_Text(pDX,IDC_EDIT_RawBits,RawBits);
	DDX_Text(pDX,IDC_COMBO_RawOutForamt,strRawOutForamt);
	DDX_Text(pDX,IDC_COMBO_ShowType,strm_ShowType);   
	DDX_Text(pDX,IDC_FileLength,m_FileLength);
}


BEGIN_MESSAGE_MAP(RawOptionDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RawOptionDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RawOptionDlg 消息处理程序

BOOL RawOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox * RawOutForamtBoBox = (CComboBox * )GetDlgItem(IDC_COMBO_RawOutForamt);
	RawOutForamtBoBox->AddString(_T("RG_GB"));
	RawOutForamtBoBox->AddString(_T("GR_BG"));
	RawOutForamtBoBox->AddString(_T("GB_RG"));
	RawOutForamtBoBox->AddString(_T("BG_GR"));

	CComboBox * ShowTypeBoBox = (CComboBox * )GetDlgItem(IDC_COMBO_ShowType);
	ShowTypeBoBox->AddString(_T("RawGray"));
	ShowTypeBoBox->AddString(_T("RawColor"));
	ShowTypeBoBox->AddString(_T("RawtoRGB24")); 
	return TRUE;
}

void RawOptionDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	RawOutForamt = 0;
	if(strRawOutForamt==_T("RG_GB"))
	{
		RawOutForamt = 0;
	}
	else if(strRawOutForamt==_T("GR_BG"))
	{
		RawOutForamt = 1;
	}
	else if(strRawOutForamt==_T("GB_RG"))
	{
		RawOutForamt = 2;
	}
	else if(strRawOutForamt==_T("BG_GR"))
	{
		RawOutForamt = 3;
	}

	if(strm_ShowType == _T("RawGray"))
	{
		m_ShowType = 1;
	}
	else if(strm_ShowType == _T("RawColor"))
	{
		m_ShowType = 2;
	}
	else  
	{
		m_ShowType = 3;
	}
	 

	CDialogEx::OnOK();

}
