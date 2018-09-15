// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FF_SFR.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "FF_SFR_TestItem.h"
#include "AutoROI.h"
// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, FF_SFR_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_bTestCenter(FALSE)
	, m_bTestF4(FALSE)
	, m_bTestF6(FALSE)
	, m_bTestF8(FALSE)
	, m_bSaveImage(FALSE)
	, m_ROI_Size(0)
	, m_sfrFreq(0)
	, m_BchekSFRSpec(FALSE)
	, m_BSaveSFR_Acut(FALSE)
	, m_BSFRSaveDataOffset(FALSE)
	, m_B40cmSFRCheckData(FALSE)
	, m_B60cmSFRCheckData(FALSE)
	, m_B120cmSFRCheckData(FALSE)
	, m_StrSpecSFR_FIRST(_T(""))
	, m_StrSpecSFR_SECOND(_T(""))
	, m_BLC(0)
{
	m_pTestItem = pTestItem;
	m_SpcShift_0F = 0.0f;
	m_SpcShift_4F = 0.0f;
	m_SpcShift_6F = 0.0f;
	m_SpcShift_8F = 0.0f;
	m_SpcShift_6F_CTC = 0.0f;
	m_SpcShift_8F_CTC = 0.0f;
	m_sfrFreq = 4;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_CHKENGINER,m_pTestItem->m_Options.bEngineerMode);
	DDX_Check(pDX,IDC_CHKMIDDLE,m_pTestItem->m_Options.m_bMidValue);
	DDX_Text(pDX,IDC_F8,m_pTestItem->m_Options.m_fCTC8);
	DDX_Text(pDX,IDC_F6,m_pTestItem->m_Options.m_fCTC6);
	DDX_Text(pDX,IDC_F8H,m_pTestItem->m_Options.m_fCTC8_H);
	DDX_Text(pDX,IDC_F8V,m_pTestItem->m_Options.m_fCTC8_V);
	DDX_Text(pDX,IDC_F6H,m_pTestItem->m_Options.m_fCTC6_H);
	DDX_Text(pDX,IDC_F6V,m_pTestItem->m_Options.m_fCTC6_V);
	DDX_Text(pDX,IDC_SHAKESKIPFRAME,m_pTestItem->m_Options.m_iShakeSkipFrame);
	DDX_Text(pDX,IDC_STATIONNAME,m_pTestItem->m_Options.m_StationName);
	DDX_Text(pDX,IDC_SFRBURNRATE,m_pTestItem->m_Options.m_iSFRBurnRate);
	DDX_Check(pDX, IDC_TEST_CEN, m_bTestCenter);
	DDX_Check(pDX, IDC_TEST_F4, m_bTestF4);
	DDX_Check(pDX, IDC_TEST_F6, m_bTestF6);
	DDX_Check(pDX, IDC_TEST_F8, m_bTestF8);
	DDX_Text(pDX, IDC_CENH_SPC, m_pTestItem->m_Options.m_CenH_Spec);
	DDX_Text(pDX, IDC_CENV_SPC, m_pTestItem->m_Options.m_CenV_Spec);
	DDX_Text(pDX, IDC_F4H_SPC, m_pTestItem->m_Options.m_F4H_Spec);
	DDX_Text(pDX, IDC_F4V_SPC, m_pTestItem->m_Options.m_F4V_Spec);
	DDX_Text(pDX, IDC_F6H_SPC, m_pTestItem->m_Options.m_F6H_Spec);
	DDX_Text(pDX, IDC_F6V_SPC, m_pTestItem->m_Options.m_F6V_Spec);
	DDX_Text(pDX, IDC_F8H_SPC, m_pTestItem->m_Options.m_F8H_Spec);
	DDX_Text(pDX, IDC_F8V_SPC, m_pTestItem->m_Options.m_F8V_Spec);
	DDX_Text(pDX,IDC_TEST_TIME,m_pTestItem->m_Options.m_Mid_TestTime);
	DDX_Text(pDX,IDC_ERROR_CODE,m_pTestItem->m_Options.ErrorCode);
	DDX_Check(pDX, IDC_CHKENGINER2, m_bSaveImage);
	DDX_Text(pDX, IDC_ROI_SIZE, m_ROI_Size);
	DDX_Check(pDX, IDC_SFR_BurnEN, m_isSFRBurn);
	//  DDX_Check(pDX, IDC_SpecShiftEn_0F, m_SpecShiftEn_0F);
	DDX_Text(pDX, IDC_SpecShift_0F, m_SpcShift_0F);
	DDX_Text(pDX, IDC_SpecShift_4F, m_SpcShift_4F);
	DDX_Text(pDX, IDC_SpecShift_6F, m_SpcShift_6F);
	DDX_Text(pDX, IDC_SpecShift_8F, m_SpcShift_8F);
	DDX_Check(pDX, IDC_SpecShiftEn_0F, m_SpcShiftEn_0F);
	DDX_Check(pDX, IDC_SpecShiftEn_4F, m_SpcShiftEn_4F);
	DDX_Check(pDX, IDC_SpecShiftEn_6F, m_SpcShiftEn_6F);
	DDX_Check(pDX, IDC_SpecShiftEn_8F, m_SpcShiftEn_8F);
	DDX_Text(pDX, IDC_SpecShift_6F_CTC, m_SpcShift_6F_CTC);
	DDX_Text(pDX, IDC_SpecShift_8F_CTC, m_SpcShift_8F_CTC);
	DDX_Check(pDX, IDC_SpecShift_CTC_En_6F, m_SpcShift_CTC_En_6F);
	DDX_Check(pDX, IDC_SpecShift_CTC_En_8F, m_SpcShift_CTC_En_8F);
	DDX_Check(pDX, IDC_SECOND_SFRCHECK, m_EnableSecondCheck);
	DDX_Text(pDX, IDC_TEST_FREQ, m_sfrFreq);
	DDX_Check(pDX, IDC_CHKSFRSPEC, m_BchekSFRSpec);
	DDX_Check(pDX, IDC_SAVEDATA, m_BSaveSFR_Acut);
	DDX_Check(pDX, IDC_SAVEDATAOFFSET, m_BSFRSaveDataOffset);
	DDX_Check(pDX, IDC_40CMSFR, m_B40cmSFRCheckData);
	DDX_Check(pDX, IDC_60CMSFR2, m_B60cmSFRCheckData);
	DDX_Check(pDX, IDC_120CMSFR3, m_B120cmSFRCheckData);
	DDX_Text(pDX, IDC_SPECSFRFIR, m_StrSpecSFR_FIRST);
	DDX_Text(pDX, IDC_SPECSFRSECOND, m_StrSpecSFR_SECOND);
	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDV_MinMaxInt(pDX, m_BLC, 0, 512);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
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

//响应模组关闭消息
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
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
