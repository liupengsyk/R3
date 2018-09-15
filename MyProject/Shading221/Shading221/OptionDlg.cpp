// OptionDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "Shading221.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "Shading221_TestItem.h"

// OptionDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, Shading221_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_BLC = 0;
	m_BGMAX = 0;
	m_BGMin = 0;
	m_RGMAX = 0;
	m_RGMin = 0;
	m_YShading_Diff = 0;
	m_YShading_Max = 0;
	m_YShading_Min = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_BLC, m_BLC);
	DDX_Text(pDX, IDC_EDIT_BGRef_Max, m_BGMAX);
	DDX_Text(pDX, IDC_EDIT_BGRef_Min, m_BGMin);
	DDX_Text(pDX, IDC_EDIT_RGRef_Max, m_RGMAX);
	DDX_Text(pDX, IDC_EDIT_RGRef_Min, m_RGMin);
	DDX_Text(pDX, IDC_EDIT_YShading_Diff, m_YShading_Diff);
	DDX_Text(pDX, IDC_EDIT_YShading_Max, m_YShading_Max);
	DDX_Text(pDX, IDC_EDIT_YShading_Min, m_YShading_Min);
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
