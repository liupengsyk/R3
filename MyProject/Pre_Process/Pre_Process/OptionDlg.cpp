// OptionDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "Pre_Process.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "Pre_Process_TestItem.h"

// OptionDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, Pre_Process_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CalDiff, CalDiff);
	DDX_Text(pDX, IDC_TESTSTATION, TestStation);
	DDX_Text(pDX, IDC_PRODUCTID, ProductID);
	DDX_Text(pDX, IDC_ADDRESS, ProductAdd);
	DDX_Check(pDX, IDC_CHECK1, m_ProductEnable);
	DDX_Text(pDX, IDC_PRODUCTID1, ProductID1);
	DDX_Text(pDX, IDC_ADDRESS1, ProductAdd1);
	DDX_Check(pDX, IDC_CHECK3, m_ProductEnable1);
	DDX_Text(pDX, IDC_SlaveID, m_SlaveID);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
END_MESSAGE_MAP()


// OptionDlg ÏûÏ¢´¦Àí³ÌÐò
void OptionDlg::PostNcDestroy()
{
	// TODO: ÔÚ´ËÌí¼Ó×¨ÓÃ´úÂëºÍ/»òµ÷ÓÃ»ùÀà
	delete this;

	CDialog::PostNcDestroy();
}

//¶Ô»°¿ò´°¿ÚÊý¾Ý¸üÐÂµÄÏìÓ¦º¯Êý
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE ¿Ø¼þµÄÖµ¡ª>±äÁ¿    
	//FALSE ±äÁ¿¡ª>¿Ø¼þµÄÖµ
	BOOL	bSaveAndValidate = (BOOL) wParam; 

	int ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{

	}
	else
	{

	}

	return ret;
}

//Ö÷³ÌÐò°´ÏÂctrl+Êý×Ö¼üµÄÏìÓ¦ÏûÏ¢£¬Í¨¹ý´ËÏûÏ¢½ÓÊÜÍâ½çÖ¸Áî¡£
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

//ÏìÓ¦Ä£×é¹Ø±ÕÏûÏ¢
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{

	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ÔÚ´ËÌí¼Ó¶îÍâµÄ³õÊ¼»¯

	return TRUE;  // return TRUE unless you set the focus to a control
	// Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}