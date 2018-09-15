// OptionDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "WBCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "WBCheck_TestItem.h"

// OptionDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, WBCheck_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_before_distance = 0;
	m_count = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SlaveID, m_SlaveID);
	DDX_Text(pDX, IDC_REDIT, R_StartAdd);
	DDX_Text(pDX, IDC_GREDIT, Gr_StartAdd);
	DDX_Text(pDX, IDC_GBEDIT, Gb_StartAdd);
	DDX_Text(pDX, IDC_BEDIT, B_StartAdd);
	DDX_Text(pDX, IDC_RGEDIT, RG_StartAdd);
	DDX_Text(pDX, IDC_BGEDIT, BG_StartAdd);
	DDX_Text(pDX, IDC_GGEDIT, GG_StartAdd);
	DDX_Text(pDX, IDC_RMIN, R_min);
	DDX_Text(pDX, IDC_RMAX, R_max);
	DDX_Text(pDX, IDC_GRMIN, Gr_min);
	DDX_Text(pDX, IDC_GRMAX, Gr_max);
	DDX_Text(pDX, IDC_GBMIN, Gb_min);
	DDX_Text(pDX, IDC_GBMAX, Gb_max);
	DDX_Text(pDX, IDC_BMIN, B_min);
	DDX_Text(pDX, IDC_BMAX, B_max);
	DDX_Text(pDX, IDC_RGMIN, RG_min);
	DDX_Text(pDX, IDC_RGMAX, RG_max);
	DDX_Text(pDX, IDC_BGMIN, BG_min);
	DDX_Text(pDX, IDC_BGMAX, BG_max);
	DDX_Text(pDX, IDC_GGMIN, GG_min);
	DDX_Text(pDX, IDC_GGMAX, GG_max);
	DDX_Text(pDX, IDC_COMBO1, ByteCount);
	DDX_Text(pDX, IDC_COMBO2, A_Multi);
	DDX_Text(pDX, IDC_COMBO3, B_Multi);
	DDX_Check(pDX, IDC_CHECK1, SingleChannelEnable);
	DDX_Check(pDX, IDC_CHECK2, GainEnable);
	DDX_Text(pDX, IDC_RATIO, m_Ratio);
	DDX_Text(pDX, IDC_RATIO2, m_BLC);
	DDX_Text(pDX, IDC_RATIO3, m_index);
	DDX_Text(pDX, IDC_Typical_R, m_TypicalR);
	DDX_Text(pDX, IDC_Typical_Gr, m_TypicalGr);
	DDX_Text(pDX, IDC_Typical_Gb, m_TypicalGb);
	DDX_Text(pDX, IDC_Typical_B, m_TypicalB);
	DDX_Text(pDX, IDC_Typical_RG, m_TypicalRG);
	DDX_Text(pDX, IDC_Typical_BG, m_TypicalBG);
	DDX_Text(pDX, IDC_Typical_GG, m_TypicalGG);
	DDX_Text(pDX, IDC_DISTANCE, m_distance);
	DDX_Text(pDX, IDC_PAGE, page);
	DDX_Check(pDX, IDC_CHECK3, m_Raw8_EN);
	DDX_Check(pDX, IDC_CHECK5, m_GG_GainEn);
	DDX_Text(pDX, IDC_DISTANCE2, m_before_distance);
	DDX_Check(pDX, IDC_CHECK6, m_flow);
	DDX_Text(pDX, IDC_COUNT, m_count);
	//  DDX_Check(pDX, IDC_CHECK7, m_m_QTITool);
	DDX_Check(pDX, IDC_CHECK7, m_QTITool);
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