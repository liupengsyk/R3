// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Shading49.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "Shading49_Inspection.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, Shading49_Inspection* pInspection)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pInspection = pInspection;

	for (int i = 0 ; i < ROI_NUM; i++)
	{
		m_pInspection->sShading49.ROI[i].cx = 0;
		m_pInspection->sShading49.ROI[i].cy = 0;
	}

	m_pInspection->sShading49.Ydif_limit = 0;
	m_pInspection->sShading49.RGdif_limit = 0;
	m_pInspection->sShading49.BGdif_limit = 0;
	m_pInspection->sShading49.ROI_height = 10;
	m_pInspection->sShading49.ROI_width = 10;

	m_pInspection->sShading49.Ydif_limit = 0;
	m_pInspection->sShading49.ROI_height = 10;
	m_pInspection->sShading49.ROI_width = 10;
	m_pInspection->sShading49.BGdif_limit = 0;
	m_pInspection->sShading49.deltaY_limit = 0;
	m_pInspection->sShading49.RGdif_limit = 0;

	m_pInspection->m_exp_target = 128;
	m_pInspection->m_exp_tolerance = 0;
	m_pInspection->m_ExposureEn = FALSE;
	m_pInspection->m_Continue = FALSE;

	m_pInspection->sShading49.PPdif_limit = 0;

	m_pInspection->sShading49.RG_Max  = 2; 
	m_pInspection->sShading49.BG_Max= 2;  
	m_pInspection->sShading49.bCheckRG_max = FALSE;
	m_pInspection->sShading49.bCheckBG_max = FALSE;
	AvgTimes = 5;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_SHADING0_x, m_pInspection->sShading49.ROI[0].cx );
	DDX_Text(pDX, IDC_SHADING0_y, m_pInspection->sShading49.ROI[0].cy );
	DDX_Text(pDX, IDC_SHADING1_x, m_pInspection->sShading49.ROI[1].cx );
	DDX_Text(pDX, IDC_SHADING1_y, m_pInspection->sShading49.ROI[1].cy );
	DDX_Text(pDX, IDC_SHADING2_x, m_pInspection->sShading49.ROI[2].cx );
	DDX_Text(pDX, IDC_SHADING2_y, m_pInspection->sShading49.ROI[2].cy );
	DDX_Text(pDX, IDC_SHADING3_x, m_pInspection->sShading49.ROI[3].cx );
	DDX_Text(pDX, IDC_SHADING3_y, m_pInspection->sShading49.ROI[3].cy );
	DDX_Text(pDX, IDC_SHADING4_x, m_pInspection->sShading49.ROI[4].cx );
	DDX_Text(pDX, IDC_SHADING4_y, m_pInspection->sShading49.ROI[4].cy );
	DDX_Text(pDX, IDC_SHADING5_x, m_pInspection->sShading49.ROI[5].cx );
	DDX_Text(pDX, IDC_SHADING5_y, m_pInspection->sShading49.ROI[5].cy );
	DDX_Text(pDX, IDC_SHADING6_x, m_pInspection->sShading49.ROI[6].cx );
	DDX_Text(pDX, IDC_SHADING6_y, m_pInspection->sShading49.ROI[6].cy );
	DDX_Text(pDX, IDC_SHADING7_x, m_pInspection->sShading49.ROI[7].cx );
	DDX_Text(pDX, IDC_SHADING7_y, m_pInspection->sShading49.ROI[7].cy );
	DDX_Text(pDX, IDC_SHADING8_x, m_pInspection->sShading49.ROI[8].cx );
	DDX_Text(pDX, IDC_SHADING8_y, m_pInspection->sShading49.ROI[8].cy );
	DDX_Text(pDX, IDC_SHADING9_x, m_pInspection->sShading49.ROI[9].cx );
	DDX_Text(pDX, IDC_SHADING9_y, m_pInspection->sShading49.ROI[9].cy );
	DDX_Text(pDX, IDC_SHADING10_x,m_pInspection->sShading49.ROI[10].cx);
	DDX_Text(pDX, IDC_SHADING10_y,m_pInspection->sShading49.ROI[10].cy);
	DDX_Text(pDX, IDC_SHADING11_x,m_pInspection->sShading49.ROI[11].cx);
	DDX_Text(pDX, IDC_SHADING11_y,m_pInspection->sShading49.ROI[11].cy);
	DDX_Text(pDX, IDC_SHADING12_x,m_pInspection->sShading49.ROI[12].cx);
	DDX_Text(pDX, IDC_SHADING12_y,m_pInspection->sShading49.ROI[12].cy);
	DDX_Text(pDX, IDC_SHADING13_x,m_pInspection->sShading49.ROI[13].cx);
	DDX_Text(pDX, IDC_SHADING13_y,m_pInspection->sShading49.ROI[13].cy);
	DDX_Text(pDX, IDC_SHADING14_x,m_pInspection->sShading49.ROI[14].cx);
	DDX_Text(pDX, IDC_SHADING14_y,m_pInspection->sShading49.ROI[14].cy);
	DDX_Text(pDX, IDC_SHADING15_x,m_pInspection->sShading49.ROI[15].cx);
	DDX_Text(pDX, IDC_SHADING15_y,m_pInspection->sShading49.ROI[15].cy);
	DDX_Text(pDX, IDC_SHADING16_x,m_pInspection->sShading49.ROI[16].cx);
	DDX_Text(pDX, IDC_SHADING16_y,m_pInspection->sShading49.ROI[16].cy);
	DDX_Text(pDX, IDC_SHADING17_x,m_pInspection->sShading49.ROI[17].cx);
	DDX_Text(pDX, IDC_SHADING17_y,m_pInspection->sShading49.ROI[17].cy);
	DDX_Text(pDX, IDC_SHADING18_x,m_pInspection->sShading49.ROI[18].cx);
	DDX_Text(pDX, IDC_SHADING18_y,m_pInspection->sShading49.ROI[18].cy);
	DDX_Text(pDX, IDC_SHADING19_x,m_pInspection->sShading49.ROI[19].cx);
	DDX_Text(pDX, IDC_SHADING19_y,m_pInspection->sShading49.ROI[19].cy);
	DDX_Text(pDX, IDC_SHADING20_x,m_pInspection->sShading49.ROI[20].cx);
	DDX_Text(pDX, IDC_SHADING20_y,m_pInspection->sShading49.ROI[20].cy);
	DDX_Text(pDX, IDC_SHADING21_x,m_pInspection->sShading49.ROI[21].cx);
	DDX_Text(pDX, IDC_SHADING21_y,m_pInspection->sShading49.ROI[21].cy);
	DDX_Text(pDX, IDC_SHADING22_x,m_pInspection->sShading49.ROI[22].cx);
	DDX_Text(pDX, IDC_SHADING22_y,m_pInspection->sShading49.ROI[22].cy);
	DDX_Text(pDX, IDC_SHADING23_x,m_pInspection->sShading49.ROI[23].cx);
	DDX_Text(pDX, IDC_SHADING23_y,m_pInspection->sShading49.ROI[23].cy);
	DDX_Text(pDX, IDC_SHADING24_x,m_pInspection->sShading49.ROI[24].cx);
	DDX_Text(pDX, IDC_SHADING24_y,m_pInspection->sShading49.ROI[24].cy);
	DDX_Text(pDX, IDC_SHADING25_x,m_pInspection->sShading49.ROI[25].cx);
	DDX_Text(pDX, IDC_SHADING25_y,m_pInspection->sShading49.ROI[25].cy);
	DDX_Text(pDX, IDC_SHADING26_x,m_pInspection->sShading49.ROI[26].cx);
	DDX_Text(pDX, IDC_SHADING26_y,m_pInspection->sShading49.ROI[26].cy);
	DDX_Text(pDX, IDC_SHADING27_x,m_pInspection->sShading49.ROI[27].cx);
	DDX_Text(pDX, IDC_SHADING27_y,m_pInspection->sShading49.ROI[27].cy);
	DDX_Text(pDX, IDC_SHADING28_x,m_pInspection->sShading49.ROI[28].cx);
	DDX_Text(pDX, IDC_SHADING28_y,m_pInspection->sShading49.ROI[28].cy);
	DDX_Text(pDX, IDC_SHADING29_x,m_pInspection->sShading49.ROI[29].cx);
	DDX_Text(pDX, IDC_SHADING29_y,m_pInspection->sShading49.ROI[29].cy);
	DDX_Text(pDX, IDC_SHADING30_x,m_pInspection->sShading49.ROI[30].cx);
	DDX_Text(pDX, IDC_SHADING30_y,m_pInspection->sShading49.ROI[30].cy);
	DDX_Text(pDX, IDC_SHADING31_x,m_pInspection->sShading49.ROI[31].cx);
	DDX_Text(pDX, IDC_SHADING31_y,m_pInspection->sShading49.ROI[31].cy);
	DDX_Text(pDX, IDC_SHADING32_x,m_pInspection->sShading49.ROI[32].cx);
	DDX_Text(pDX, IDC_SHADING32_y,m_pInspection->sShading49.ROI[32].cy);
	DDX_Text(pDX, IDC_SHADING33_x,m_pInspection->sShading49.ROI[33].cx);
	DDX_Text(pDX, IDC_SHADING33_y,m_pInspection->sShading49.ROI[33].cy);
	DDX_Text(pDX, IDC_SHADING34_x,m_pInspection->sShading49.ROI[34].cx);
	DDX_Text(pDX, IDC_SHADING34_y,m_pInspection->sShading49.ROI[34].cy);
	DDX_Text(pDX, IDC_SHADING35_x,m_pInspection->sShading49.ROI[35].cx);
	DDX_Text(pDX, IDC_SHADING35_y,m_pInspection->sShading49.ROI[35].cy);
	DDX_Text(pDX, IDC_SHADING36_x,m_pInspection->sShading49.ROI[36].cx);
	DDX_Text(pDX, IDC_SHADING36_y,m_pInspection->sShading49.ROI[36].cy);
	DDX_Text(pDX, IDC_SHADING37_x,m_pInspection->sShading49.ROI[37].cx);
	DDX_Text(pDX, IDC_SHADING37_y,m_pInspection->sShading49.ROI[37].cy);
	DDX_Text(pDX, IDC_SHADING38_x,m_pInspection->sShading49.ROI[38].cx);
	DDX_Text(pDX, IDC_SHADING38_y,m_pInspection->sShading49.ROI[38].cy);
	DDX_Text(pDX, IDC_SHADING39_x,m_pInspection->sShading49.ROI[39].cx);
	DDX_Text(pDX, IDC_SHADING39_y,m_pInspection->sShading49.ROI[39].cy);
	DDX_Text(pDX, IDC_SHADING40_x,m_pInspection->sShading49.ROI[40].cx);
	DDX_Text(pDX, IDC_SHADING40_y,m_pInspection->sShading49.ROI[40].cy);
	DDX_Text(pDX, IDC_SHADING41_x,m_pInspection->sShading49.ROI[41].cx);
	DDX_Text(pDX, IDC_SHADING41_y,m_pInspection->sShading49.ROI[41].cy);
	DDX_Text(pDX, IDC_SHADING42_x,m_pInspection->sShading49.ROI[42].cx);
	DDX_Text(pDX, IDC_SHADING42_y,m_pInspection->sShading49.ROI[42].cy);
	DDX_Text(pDX, IDC_SHADING43_x,m_pInspection->sShading49.ROI[43].cx);
	DDX_Text(pDX, IDC_SHADING43_y,m_pInspection->sShading49.ROI[43].cy);
	DDX_Text(pDX, IDC_SHADING44_x,m_pInspection->sShading49.ROI[44].cx);
	DDX_Text(pDX, IDC_SHADING44_y,m_pInspection->sShading49.ROI[44].cy);
	DDX_Text(pDX, IDC_SHADING45_x,m_pInspection->sShading49.ROI[45].cx);
	DDX_Text(pDX, IDC_SHADING45_y,m_pInspection->sShading49.ROI[45].cy);
	DDX_Text(pDX, IDC_SHADING46_x,m_pInspection->sShading49.ROI[46].cx);
	DDX_Text(pDX, IDC_SHADING46_y,m_pInspection->sShading49.ROI[46].cy);
	DDX_Text(pDX, IDC_SHADING47_x,m_pInspection->sShading49.ROI[47].cx);
	DDX_Text(pDX, IDC_SHADING47_y,m_pInspection->sShading49.ROI[47].cy);
	DDX_Text(pDX, IDC_SHADING48_x,m_pInspection->sShading49.ROI[48].cx);
	DDX_Text(pDX, IDC_SHADING48_y,m_pInspection->sShading49.ROI[48].cy);


	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[0].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[0].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[1].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[1].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[2].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[2].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[3].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[3].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[4].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[4].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[5].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[5].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[6].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[6].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[7].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[7].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[8].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[8].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[9].cx , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[9].cy , 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[10].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[10].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[11].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[11].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[12].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[12].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[13].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[13].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[14].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[14].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[15].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[15].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[16].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[16].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[17].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[17].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[18].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[18].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[19].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[19].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[20].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[20].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[21].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[21].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[22].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[22].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[23].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[23].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[24].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[24].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[25].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[25].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[26].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[26].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[27].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[27].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[28].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[28].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[29].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[29].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[30].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[30].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[31].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[31].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[32].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[32].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[33].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[33].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[34].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[34].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[35].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[35].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[36].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[36].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[37].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[37].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[38].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[38].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[39].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[39].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[40].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[40].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[41].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[41].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[42].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[42].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[43].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[43].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[44].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[44].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[45].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[45].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[46].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[46].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[47].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[47].cy, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[48].cx, 0, 100);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI[48].cy, 0, 100);


	DDX_Text(pDX, IDC_YDIF, m_pInspection->sShading49.Ydif_limit);
	DDV_MinMaxDouble(pDX, m_pInspection->sShading49.Ydif_limit, 0, 100);
	DDX_Text(pDX, IDC_SHADING_HEIGHT, m_pInspection->sShading49.ROI_height);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI_height, 1, 100);
	DDX_Text(pDX, IDC_SHADING_WIDTH, m_pInspection->sShading49.ROI_width);
	DDV_MinMaxUInt(pDX, m_pInspection->sShading49.ROI_width, 1, 100);
	DDX_Text(pDX, IDC_BG, m_pInspection->sShading49.BGdif_limit);
	DDV_MinMaxDouble(pDX, m_pInspection->sShading49.BGdif_limit, 0, 100);
	DDX_Text(pDX, IDC_DELTA_Y, m_pInspection->sShading49.deltaY_limit);
	DDV_MinMaxDouble(pDX, m_pInspection->sShading49.deltaY_limit, 0, 100);
	DDX_Text(pDX, IDC_RG, m_pInspection->sShading49.RGdif_limit);
	DDV_MinMaxDouble(pDX, m_pInspection->sShading49.RGdif_limit, 0, 100);
	DDX_Text(pDX, IDC_SD_EXPTOLERANCE, m_pInspection->m_exp_tolerance);
	DDV_MinMaxUInt(pDX, m_pInspection->m_exp_tolerance, 0, 100);
	DDX_Text(pDX, IDC_SD_EXPTARGET, m_pInspection->m_exp_target);
	DDV_MinMaxUInt(pDX, m_pInspection->m_exp_target, 1, 255);
	DDX_Check(pDX, IDC_CHECK_EXPOUSE,m_pInspection->m_ExposureEn);
	DDX_Check(pDX, IDC_CONTINUE, m_pInspection->m_Continue);

	DDX_Text(pDX, IDC_PPDIF, m_pInspection->sShading49.PPdif_limit);
	DDV_MinMaxDouble(pDX, m_pInspection->sShading49.PPdif_limit, 0, 100);

	DDX_Text(pDX,IDC_RGMax,m_pInspection->sShading49.RG_Max);
	DDX_Text(pDX,IDC_RGMax2,m_pInspection->sShading49.RG_Max2);
	DDX_Text(pDX,IDC_BGMax,m_pInspection->sShading49.BG_Max);

	DDX_Check(pDX,IDC_bCheckRGmax,m_pInspection->sShading49.bCheckRG_max);
	DDX_Check(pDX,IDC_bCheckBGmax,m_pInspection->sShading49.bCheckBG_max);

	DDX_Text(pDX,IDC_AvgTime,AvgTimes);	 
 
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
END_MESSAGE_MAP()


// OptionDlg 消息处理程序
void OptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

//对话框窗口数据更新的响应函数
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE 控件的值—>变量    
	//FALSE 变量—>控件的值
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

//主程序按下ctrl+数字键的响应消息，通过此消息接受外界指令。
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{

	return 1;
}


BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}