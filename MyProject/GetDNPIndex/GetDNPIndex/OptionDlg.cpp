// OptionDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "GetDNPIndex.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "GetDNPIndex_TestItem.h"

// OptionDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, GetDNPIndex_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_DNPDistance = 0;
	m_count = 0;

	m_ColorNum = 0;
	m_ModuleNum = 0;
	m_ColorName = _T("");
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	for (int i=0;i<20;i++)
	{
		DDX_Text(pDX, IDC_FuseIDEDIT1+i, FuseID[i]);
	}
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
	DDX_Text(pDX, IDC_R_INDEXMIN, R_index_min);
	DDX_Text(pDX, IDC_R_INDEXMAX, R_index_max);
	DDX_Text(pDX, IDC_GR_INDEXMIN, Gr_index_min);
	DDX_Text(pDX, IDC_GR_INDEXMAX, Gr_index_max);
	DDX_Text(pDX, IDC_GB_INDEXMIN, Gb_index_min);
	DDX_Text(pDX, IDC_GB_INDEXMAX, Gb_index_max);
	DDX_Text(pDX, IDC_B_INDEXMIN, B_index_min);
	DDX_Text(pDX, IDC_B_INDEXMAX, B_index_max);
	DDX_Text(pDX, IDC_RG_INDEXMIN, RG_index_min);
	DDX_Text(pDX, IDC_RG_INDEXMAX, RG_index_max);
	DDX_Text(pDX, IDC_BG_INDEXMIN, BG_index_min);
	DDX_Text(pDX, IDC_BG_INDEXMAX, BG_index_max);
	DDX_Text(pDX, IDC_GG_INDEXMIN, GG_index_min);
	DDX_Text(pDX, IDC_GG_INDEXMAX, GG_index_max);
	DDX_Text(pDX, IDC_INDEXERROR, index_error);
	DDX_Text(pDX, IDC_COMBO1, ByteCount);
	DDX_Text(pDX, IDC_COMBO2, A_Multi);
	DDX_Text(pDX, IDC_COMBO3, B_Multi);
	DDX_Check(pDX, IDC_CHECK1, SingleChannelEnable);
	DDX_Check(pDX, IDC_CHECK2, GainEnable);
	DDX_Text(pDX, IDC_RATIO, m_Ratio);
	DDX_Text(pDX, IDC_RATIO2, m_BLC);
	DDX_Text(pDX, IDC_RATIO3, m_index);
	DDX_Text(pDX, IDC_STATION, Station);
	DDX_Check(pDX, IDC_CHECK3, m_Raw8_EN);
	DDX_Text(pDX, IDC_PAGE, page);
	DDX_Text(pDX, IDC_DISTANCE, m_DNPDistance);
	DDX_Check(pDX, IDC_CHECK5, m_GG_GainEn);
	DDX_Check(pDX, IDC_CHECK6, m_flow);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Check(pDX, IDC_CHECK7, m_QTITool);

	DDX_Text(pDX, IDC_EDIT1, m_ColorNum);
	DDX_Text(pDX, IDC_EDIT3, m_ModuleNum);
	DDX_Text(pDX, IDC_EDIT8, m_ColorName);
	DDX_Check(pDX, IDC_CHECK_isSingleModule, m_isSingleModule);
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