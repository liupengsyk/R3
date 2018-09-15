// OptionDlg.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "GetDNPIndex_Moto.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "GetDNPIndex_Moto_TestItem.h"

// OptionDlg ¶Ô»°¿ò

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, GetDNPIndex_Moto_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_DNPDistance = 0;
	m_count = 0;

	m_ColorNum = 0;
	m_ModuleNum = 0;
	m_ColorName = _T("");

	m_CalDiff = 12;

	m_Ev_HighCCT_Min = 0;
	m_Ev_HighCCT_Max = 0;
	m_U_HighCCT_Max = 0;
	m_U_HighCCT_Min = 0;
	m_V_HighCCT_Max = 0;
	m_V_HighCCT_Min = 0;
	m_Tolerance_HighCCT_Max = 0.0;

	m_U_Standard_HighCCT = 0;
	m_V_Standard_HighCCT = 0;

	m_GoldenDiff = 0.0;
	m_CurrentDiff = 0.0;

	memset((void *)(&m_TypVal), 0x00, sizeof(m_TypVal));
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
	DDX_Text(pDX, IDC_ROI_H, m_ROI_H);
	DDX_Text(pDX, IDC_ROI_V, m_ROI_V);
	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDX_Text(pDX, IDC_Multiple, m_Multiple);
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

	DDX_Text(pDX, IDC_CAL_DIFF, m_CalDiff);

	/** 联想的光源点检数据 **/
	DDX_Text(pDX, IDC_TYPICAL_R, m_TypVal.m_R);
	DDX_Text(pDX, IDC_TYPICAL_GR, m_TypVal.m_Gr);
	DDX_Text(pDX, IDC_TYPICAL_GB, m_TypVal.m_Gb);
	DDX_Text(pDX, IDC_TYPICAL_B, m_TypVal.m_B);
	DDX_Text(pDX, IDC_TYPICAL_RG, m_TypVal.m_RGain);
	DDX_Text(pDX, IDC_TYPICAL_BG, m_TypVal.m_BGain);
	DDX_Text(pDX, IDC_TYPICAL_GG, m_TypVal.m_GGain);

	DDX_Text(pDX, IDC_GOLDEN_DIFF, m_GoldenDiff);
	DDX_Text(pDX, IDC_CURRENT_DIFF, m_CurrentDiff);

	DDX_Text(pDX, IDC_EV_MIN, m_Ev_HighCCT_Min);
	DDX_Text(pDX, IDC_EV_MAX, m_Ev_HighCCT_Max);
	DDX_Text(pDX, IDC_U_MIN, m_U_HighCCT_Min);
	DDX_Text(pDX, IDC_U_MAX, m_U_HighCCT_Max);
	DDX_Text(pDX, IDC_V_MIN, m_V_HighCCT_Min);
	DDX_Text(pDX, IDC_V_MAX, m_V_HighCCT_Max);

	DDX_Text(pDX, IDC_CCT_DIFF, m_Tolerance_HighCCT_Max);
	DDX_Text(pDX, IDC_U_STANDARD, m_U_Standard_HighCCT);
	DDX_Text(pDX, IDC_V_STANDARD, m_V_Standard_HighCCT);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_RADIO4, &OptionDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &OptionDlg::OnBnClickedRadio5)
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

void OptionDlg::OnBnClickedRadio4()//动态点检
{
	// TODO: 在此添加控件通知处理程序代码
	MotoLightSrcCfg(TRUE);
	m_MotoLightSrcMode = MOTO_LIGHT_SRC_CAL_MODE_DYNAMIC;
}

void OptionDlg::OnBnClickedRadio5()//静态点检
{
	// TODO: 在此添加控件通知处理程序代码
	MotoLightSrcCfg(FALSE);
	m_MotoLightSrcMode = MOTO_LIGHT_SRC_CAL_MODE_STATIC;
}

void OptionDlg::MotoDynamicLightSrcEnable(BOOL bEnable)
{
	GetDlgItem(IDC_TYPICAL_R)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_GR)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_GB)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_B)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_RG)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_BG)->EnableWindow(bEnable);
	GetDlgItem(IDC_TYPICAL_GG)->EnableWindow(bEnable);

	GetDlgItem(IDC_GOLDEN_DIFF)->EnableWindow(bEnable);
	GetDlgItem(IDC_CURRENT_DIFF)->EnableWindow(bEnable);

	/*GetDlgItem(IDC_EV_MIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_EV_MAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_U_MIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_U_MAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_V_MIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_V_MAX)->EnableWindow(bEnable);

	GetDlgItem(IDC_CCT_DIFF)->EnableWindow(bEnable);
	GetDlgItem(IDC_U_STANDARD)->EnableWindow(bEnable);
	GetDlgItem(IDC_V_STANDARD)->EnableWindow(bEnable);*/
}

void OptionDlg::MotoStaticLightSrcEnable(BOOL bEnable)
{
	GetDlgItem(IDC_RMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GRMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GRMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GBMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GBMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_BMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BMAX)->EnableWindow(bEnable);

	GetDlgItem(IDC_RGMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RGMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_BGMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BGMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GGMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GGMAX)->EnableWindow(bEnable);

	GetDlgItem(IDC_R_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_R_INDEXMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GR_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GR_INDEXMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GB_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GB_INDEXMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_B_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_B_INDEXMAX)->EnableWindow(bEnable);

	GetDlgItem(IDC_RG_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RG_INDEXMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_BG_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_BG_INDEXMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_GG_INDEXMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_GG_INDEXMAX)->EnableWindow(bEnable);

	GetDlgItem(IDC_INDEXERROR)->EnableWindow(bEnable);
	GetDlgItem(IDC_DISTANCE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK5)->EnableWindow(bEnable);
}

void OptionDlg::MotoLightSrcCfg(BOOL bDynamic)
{
	if (TRUE == bDynamic)
	{
		MotoDynamicLightSrcEnable(TRUE);
		MotoStaticLightSrcEnable(FALSE);
	}
	else
	{
		MotoDynamicLightSrcEnable(FALSE);
		MotoStaticLightSrcEnable(TRUE);
	}
}