// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AF_SFR.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AF_SFR_TestItem.h"
#include "AutoROI.h"
// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AF_SFR_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_bTestCenter(FALSE)
	, m_bTestF3(FALSE)
	, m_bTestF4(FALSE)
	, m_bTestF5(FALSE)
	, m_bTestF6(FALSE)
	, m_bTestF8(FALSE)
	, m_AFTestMode(0)
	, m_StartOffset(0)
	, m_EndOffset(0)
	, m_FastMode_Step(0)
	, m_UseCode_Name(_T(""))
	, m_bSaveImage(FALSE)
	, m_ROI_Size(0)
{
	m_pTestItem = pTestItem;
	m_SpcShift_3F = 0.0f;
	m_SpcShift_4F = 0.0f;
	m_SpcShift_5F = 0.0f;
	m_SpcShift_0F = 0.0f;
	m_SpcShift_6F = 0.0f;
	m_SpcShift_8F = 0.0f;
	m_CodeDiffMax = 0;
	m_CodeDiffMin = 0;
	m_InfinityStationName = _T("");
	m_MacroStationName = _T("");
	m_SpcShift_5F_CTC = 0.0f;
	m_SpcShift_6F_CTC = 0.0f;
	m_SpcShift_8F_CTC = 0.0f;

	m_sfrFreq = 4;
	m_cornerDiffRelative = FALSE;
	m_cornerDiffAbsolute = TRUE;

	m_smallRange = _T("20");
	m_ContrastStepCount = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX,IDC_CHKENGINER,m_pTestItem->m_Options.bEngineerMode);
	DDX_Check(pDX,IDC_CHKMIDDLE,m_pTestItem->m_Options.m_bMidValue);
	DDX_Check(pDX,IDC_SHAKEEN,m_pTestItem->m_Options.m_bShakeEn);
	DDX_Text(pDX,IDC_STARTCODE,m_pTestItem->m_Options.m_iStartCode);
	DDX_Text(pDX,IDC_ENDCODE,m_pTestItem->m_Options.m_iEndCode);
	DDX_Text(pDX,IDC_BIGSTETP,m_pTestItem->m_Options.m_iStep1);
	DDX_Text(pDX,IDC_MIDDLESTETP2,m_pTestItem->m_Options.m_iStep2);
	DDX_Text(pDX,IDC_SMALLSTEP,m_pTestItem->m_Options.m_iStep3);
	DDX_Text(pDX,IDC_SKIPFRAME,m_pTestItem->m_Options.m_iSkipFrame);
	DDX_Text(pDX,IDC_F8,m_pTestItem->m_Options.m_fCTC8);
	DDX_Text(pDX,IDC_F6,m_pTestItem->m_Options.m_fCTC6);
	DDX_Text(pDX,IDC_F5,m_pTestItem->m_Options.m_fCTC5);
	DDX_Text(pDX,IDC_F8H,m_pTestItem->m_Options.m_fCTC8_H);
	DDX_Text(pDX,IDC_F8V,m_pTestItem->m_Options.m_fCTC8_V);
	DDX_Text(pDX,IDC_F6H,m_pTestItem->m_Options.m_fCTC6_H);
	DDX_Text(pDX,IDC_F6V,m_pTestItem->m_Options.m_fCTC6_V);
	DDX_Text(pDX,IDC_F5H,m_pTestItem->m_Options.m_fCTC5_H);
	DDX_Text(pDX,IDC_F5V,m_pTestItem->m_Options.m_fCTC5_V);
	DDX_Text(pDX,IDC_SHAKESKIPFRAME,m_pTestItem->m_Options.m_iShakeSkipFrame);
	DDX_Text(pDX,IDC_SHAKECODE,m_pTestItem->m_Options.m_iShakeDAC);
	DDX_Text(pDX,IDC_SHAKETIME,m_pTestItem->m_Options.m_iShakeCount);
	DDX_Text(pDX,IDC_MINCODE,m_pTestItem->m_Options.m_iPeakMin);
	DDX_Text(pDX,IDC_MAXCODE,m_pTestItem->m_Options.m_iPeakMax);

	DDX_Text(pDX,IDC_STATIONNAME,m_pTestItem->m_Options.m_StationName);
	DDX_Text(pDX,IDC_SFRBURNRATE,m_pTestItem->m_Options.m_iSFRBurnRate);

	DDX_Check(pDX, IDC_TEST_CEN, m_bTestCenter);
	DDX_Check(pDX, IDC_TEST_F3, m_bTestF3);
	DDX_Check(pDX, IDC_TEST_F4, m_bTestF4);
	DDX_Check(pDX, IDC_TEST_F5, m_bTestF5);
	DDX_Check(pDX, IDC_TEST_F6, m_bTestF6);
	DDX_Check(pDX, IDC_TEST_F8, m_bTestF8);
	DDX_Text(pDX, IDC_CENH_SPC, m_pTestItem->m_Options.m_CenH_Spec);
	DDX_Text(pDX, IDC_CENV_SPC, m_pTestItem->m_Options.m_CenV_Spec);
	DDX_Text(pDX, IDC_F3H_SPC, m_pTestItem->m_Options.m_F3H_Spec);
	DDX_Text(pDX, IDC_F3V_SPC, m_pTestItem->m_Options.m_F3V_Spec);
	DDX_Text(pDX, IDC_F4H_SPC, m_pTestItem->m_Options.m_F4H_Spec);
	DDX_Text(pDX, IDC_F4V_SPC, m_pTestItem->m_Options.m_F4V_Spec);
	DDX_Text(pDX, IDC_F5H_SPC, m_pTestItem->m_Options.m_F5H_Spec);
	DDX_Text(pDX, IDC_F5V_SPC, m_pTestItem->m_Options.m_F5V_Spec);
	DDX_Text(pDX, IDC_F6H_SPC, m_pTestItem->m_Options.m_F6H_Spec);
	DDX_Text(pDX, IDC_F6V_SPC, m_pTestItem->m_Options.m_F6V_Spec);
	DDX_Text(pDX, IDC_F8H_SPC, m_pTestItem->m_Options.m_F8H_Spec);
	DDX_Text(pDX, IDC_F8V_SPC, m_pTestItem->m_Options.m_F8V_Spec);
	DDX_Text(pDX,IDC_TEST_TIME,m_pTestItem->m_Options.m_Mid_TestTime);
	DDX_Text(pDX,IDC_ERROR_CODE,m_pTestItem->m_Options.ErrorCode);
	DDX_Radio(pDX, IDC_NORMAL_MODE, m_AFTestMode);
	DDX_Text(pDX, IDC_START_OFFSET, m_StartOffset);
	DDX_Text(pDX, IDC_END_OFFSET, m_EndOffset);
	DDX_Text(pDX, IDC_FASTMODE_STEP, m_FastMode_Step);
	DDX_Text(pDX, IDC_FQC_NAME, m_UseCode_Name);
	DDX_Check(pDX, IDC_CHKENGINER2, m_bSaveImage);
	DDX_Text(pDX, IDC_ROI_SIZE, m_ROI_Size);
	DDX_Text(pDX, IDC_SpecShift_0F, m_SpcShift_0F);
	DDX_Text(pDX, IDC_SpecShift_3F, m_SpcShift_3F);
	DDX_Text(pDX, IDC_SpecShift_4F, m_SpcShift_4F);
	DDX_Text(pDX, IDC_SpecShift_5F, m_SpcShift_5F);
	DDX_Text(pDX, IDC_SpecShift_6F, m_SpcShift_6F);
	DDX_Text(pDX, IDC_SpecShift_8F, m_SpcShift_8F);
	DDX_Check(pDX, IDC_SpecShiftEn_0F, m_SpcShift_En_0F);
	DDX_Check(pDX, IDC_SpecShiftEn_3F, m_SpcShift_En_3F);
	DDX_Check(pDX, IDC_SpecShiftEn_4F, m_SpcShift_En_4F);
	DDX_Check(pDX, IDC_SpecShiftEn_5F, m_SpcShift_En_5F);
	DDX_Check(pDX, IDC_SpecShiftEn_6F, m_SpcShift_En_6F);
	DDX_Check(pDX, IDC_SpecShiftEn_8F, m_SpcShift_En_8F);
	DDX_Text(pDX, IDC_CodeDiffMax, m_CodeDiffMax);
	DDX_Text(pDX, IDC_CodeDiffMin, m_CodeDiffMin);
	DDX_Text(pDX, IDC_InfinityStationName, m_InfinityStationName);
	DDX_Text(pDX, IDC_MacroStationName, m_MacroStationName);
	DDX_Check(pDX, IDC_CodeDiffCheckEn, m_CodeDiffCheckEn);
	DDX_Text(pDX, IDC_SpecShift_6F_CTC, m_SpcShift_6F_CTC);
	DDX_Text(pDX, IDC_SpecShift_8F_CTC, m_SpcShift_8F_CTC);
	DDX_Check(pDX, IDC_SpecShift_CTC_En_6F, m_SpcShift_CTC_En_6F);
	DDX_Check(pDX, IDC_SpecShift_CTC_En_8F, m_SpcShift_CTC_En_8F);

	DDX_Text(pDX, IDC_SFR_TEST_FREQ, m_sfrFreq);
	DDX_Check(pDX, IDC_RADIO_CORNER_DIFF_RELATIVE, m_cornerDiffRelative);
	DDX_Check(pDX, IDC_RADIO_CORNER_DIFF_ABSOLUTE, m_cornerDiffAbsolute);

	DDX_Radio(pDX,IDC_RADIO1,m_pTestItem->m_Options.m_iAFMethod);
	DDX_Text(pDX,IDC_STARTX,m_pTestItem->m_Options.m_iContrastROIStartX);
	DDX_Text(pDX,IDC_STARTY,m_pTestItem->m_Options.m_iContrastROIStartY);
	DDX_Text(pDX,IDC_ROIWIDTH,m_pTestItem->m_Options.m_iContrastROIWidth);
	DDX_Text(pDX,IDC_ROIHEIGHT,m_pTestItem->m_Options.m_iContrastROIHeight);

	DDX_Text(pDX, IDC_SMALL_RANGE, m_smallRange);
	DDX_Check(pDX, IDC_CTC_LIMIT, m_bCTCLimit);
	DDX_Text(pDX, IDC_CONTRAST_STEP_COUNT, m_ContrastStepCount);
	DDX_Check(pDX, IDC_CHECK_PEAKCODE, m_CheckPeakCode);
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

