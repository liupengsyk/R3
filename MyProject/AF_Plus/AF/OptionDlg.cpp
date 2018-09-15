// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AF_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AF_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;

	m_afPara.errCode = _T("16");
	m_afPara.errInfo = _T("Test Failed");

	m_afPara.enable = TRUE;
	m_afPara.avAlgorithm = _T("MTF");
	m_afPara.frmDelay = _T("3");
	m_afPara.startCode = _T("100");
	m_afPara.endCode = _T("400");

	m_afPara.sfrFullMode = TRUE;
	m_afPara.sfrLastMode = FALSE;
	m_afPara.sfrBmpMode = FALSE;
	m_afPara.sfrDemosaic = FALSE;
	m_afPara.sfrRawBalance = TRUE;
	m_afPara.afAvSpec = _T("0.5");

	m_afPara.stationName = _T("Station1");

	m_afPara.throuthFocusLU = _T("1");
	m_afPara.throuthFocusRU = _T("2");
	m_afPara.throuthFocusLD = _T("3");
	m_afPara.throuthFocusRD = _T("4");
	m_afPara.throuthFocusEnable = 0;
	m_afPara.throuthFocusEnginerModeEnable = 0;

	m_afPara.finalChkEnable = FALSE;
	m_afPara.finalChkAfEn = FALSE;
	m_afPara.finalChkCodeName = _T("None");
	m_afPara.finalChkAfRange = _T("30");
	m_afPara.finalChkCodeDiffSpec = _T("15");

	m_afPara.ae.enable = TRUE;
	m_afPara.ae.overAverage = FALSE;
	m_afPara.ae.over128 = FALSE;
	m_afPara.ae.channel = _T("G");
	m_afPara.ae.roi = _T("5X5");
	m_afPara.ae.blc = _T("0");
	m_afPara.ae.expoNum = _T("20");
	m_afPara.ae.expoInitVal = _T("0x00F0");
	m_afPara.ae.target = _T("128");
	m_afPara.ae.range = _T("5");

	m_bUseFCelltoByear = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_AF_ERROR_CODE, m_afPara.errCode);
	DDX_Text(pDX, IDC_EDIT_AF_ERROR_INFO, m_afPara.errInfo);

	DDX_Text(pDX, IDC_COMBO_AF_PARA_AV_ALGORITHM, m_afPara.avAlgorithm);
	DDX_Text(pDX, IDC_EDIT_AF_PARA_FRAME_DELAY, m_afPara.frmDelay);
	DDX_Text(pDX, IDC_EDIT_AF_PARA_START_CODE, m_afPara.startCode);
	DDX_Text(pDX, IDC_EDIT_AF_PARA_END_CODE, m_afPara.endCode);
	DDX_Text(pDX, IDC_EDIT_AF_PARA_AV_SPEC, m_afPara.afAvSpec);

	DDX_Check(pDX, IDC_CHECK_AF_ENABLE, m_afPara.enable);
	DDX_Check(pDX, IDC_RADIO_AF_SFR_FULL_MODE, m_afPara.sfrFullMode);
	DDX_Check(pDX, IDC_RADIO_AF_SFR_LAST_MODE, m_afPara.sfrLastMode);
	DDX_Check(pDX, IDC_RADIO_AF_BMP_MODE, m_afPara.sfrBmpMode);
	DDX_Check(pDX, IDC_RADIO_AF_SFR_DEMOSAIC, m_afPara.sfrDemosaic);
	DDX_Check(pDX, IDC_RADIO_AF_SFR_RAW_BALANCE, m_afPara.sfrRawBalance);

	DDX_Text(pDX, IDC_EDIT_AF_PARA_STATION_NAME, m_afPara.stationName);

	DDX_Text(pDX, IDC_EDIT_AF_THROUGH_FOCUS_LEFT_UP, m_afPara.throuthFocusLU);
	DDX_Text(pDX, IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_UP, m_afPara.throuthFocusRU);
	DDX_Text(pDX, IDC_EDIT_AF_THROUGH_FOCUS_LEFT_DOWN, m_afPara.throuthFocusLD);
	DDX_Text(pDX, IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_DOWN, m_afPara.throuthFocusRD);
	DDX_Check(pDX, IDC_CHECK_AF_THROUGH_FOCUS_ENABLE, m_afPara.throuthFocusEnable);
	DDX_Check(pDX, IDC_CHECK_AF_THROUGH_FOCUS_ENABLE_ENGINER_MODE, m_afPara.throuthFocusEnginerModeEnable);

	DDX_Check(pDX, IDC_CHECK_FINAL_CHK_ENABLE, m_afPara.finalChkEnable);
	DDX_Check(pDX, IDC_CHECK_FINAL_CHK_AF, m_afPara.finalChkAfEn);
	DDX_Text(pDX, IDC_COMBO_FINAL_CHK_AF_CODE_NAME, m_afPara.finalChkCodeName);
	DDX_Text(pDX, IDC_EDIT_FINAL_CHK_AF_RANGE, m_afPara.finalChkAfRange);
	DDX_Text(pDX, IDC_EDIT_FINAL_CHK_AF_CODE_SPEC, m_afPara.finalChkCodeDiffSpec);

	DDX_Check(pDX, IDC_CHECK_AF_AE_ENABLE, m_afPara.ae.enable);
	DDX_Check(pDX, IDC_CHECK_AF_AE_OVER_AVERAGE, m_afPara.ae.overAverage);
	DDX_Check(pDX, IDC_CHECK_AF_AE_OVER_128, m_afPara.ae.over128);
	DDX_Text(pDX, IDC_COMBO_AF_AE_CHANNEL, m_afPara.ae.channel);
	DDX_Text(pDX, IDC_COMBO_AF_AE_ROI, m_afPara.ae.roi);
	DDX_Text(pDX, IDC_EDIT_AF_AE_BLC, m_afPara.ae.blc);
	DDX_Text(pDX, IDC_EDIT_AF_AE_EXPOSURE_NUMBER, m_afPara.ae.expoNum);
	DDX_Text(pDX, IDC_EDIT_AF_AE_EXPOSURE_INIT_VALUE, m_afPara.ae.expoInitVal);
	DDX_Text(pDX, IDC_EDIT_AF_AE_EXPOSURE_TARGET, m_afPara.ae.target);
	DDX_Text(pDX, IDC_EDIT_AF_AE_EXPOSURE_RANGE, m_afPara.ae.range);

	DDX_Check(pDX,IDC_CHECK1,m_bUseFCelltoByear);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_BUTTON_MTF_PARA, &OptionDlg::OnBnClickedButtonMtfPara)
	ON_BN_CLICKED(IDC_CHECK_AF_AE_ENABLE, &OptionDlg::OnBnClickedCheckAfAeEnable)
	ON_BN_CLICKED(IDC_CHECK_AF_THROUGH_FOCUS_ENABLE, &OptionDlg::OnBnClickedCheckAfThroughFocusEnable)
	ON_BN_CLICKED(IDC_CHECK_AF_ENABLE, &OptionDlg::OnBnClickedCheckAfEnable)
	ON_BN_CLICKED(IDC_CHECK_FINAL_CHK_ENABLE, &OptionDlg::OnBnClickedCheckFinalChkEnable)
	ON_BN_CLICKED(IDC_BUTTON_SFR_SQUARE_CHART_PARA, &OptionDlg::OnBnClickedButtonSfrSquareChartPara)
	ON_BN_CLICKED(IDC_BUTTON_SFR_XCHART_PARA, &OptionDlg::OnBnClickedButtonSfrXchartPara)
	ON_BN_CLICKED(IDC_BUTTON_SFR_MOTO_CHART_PARA, &OptionDlg::OnBnClickedButtonSfrMotoChartPara)
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
	//((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->Clear();
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->AddString(_T("MTF"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->AddString(_T("Contrast"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->AddString(_T("X_CHART_SFR"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->AddString(_T("MOTO_CHART_SFR"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->AddString(_T("SQUARE_CHART_SFR"));
	//((CComboBox*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->SetCurSel(0);

	((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->AddString(_T("InfCode"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->AddString(_T("Middle1Code"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->AddString(_T("Middle2Code"));
	((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->AddString(_T("MacroCode"));

	((CButton*)GetDlgItem(IDC_CHECK_AF_AE_ENABLE))->SetCheck(TRUE);

	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("Y"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("R"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("G"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("B"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("Gr"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->AddString(_T("Gb"));

	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->AddString(_T("3X3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->AddString(_T("5X5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->AddString(_T("7X7"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->AddString(_T("8X8"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->AddString(_T("9X9"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void OptionDlg::OnBnClickedButtonMtfPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_dlgMtf.DoModal() == IDCANCEL)
		return;

	m_pTestItem->SaveMtfPara();
}


void OptionDlg::OnBnClickedCheckAfAeEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	AE_UI_Update();
}


void OptionDlg::AE_UI_Update()
{
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_AF_AE_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CButton*)(GetDlgItem(IDC_CHECK_AF_AE_OVER_AVERAGE)))->EnableWindow(FALSE);
		((CButton*)(GetDlgItem(IDC_CHECK_AF_AE_OVER_128)))->EnableWindow(FALSE);

		((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->EnableWindow(FALSE);
		((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->EnableWindow(FALSE);

		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_BLC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_NUMBER))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_INIT_VALUE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_TARGET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_RANGE))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)(GetDlgItem(IDC_CHECK_AF_AE_OVER_AVERAGE)))->EnableWindow(FALSE);
		((CButton*)(GetDlgItem(IDC_CHECK_AF_AE_OVER_128)))->EnableWindow(TRUE);

		((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_CHANNEL))->EnableWindow(TRUE);
		((CComboBox*)GetDlgItem(IDC_COMBO_AF_AE_ROI))->EnableWindow(TRUE);

		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_BLC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_NUMBER))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_INIT_VALUE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_TARGET))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_AE_EXPOSURE_RANGE))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedCheckAfThroughFocusEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	ThroughFocusUI_Update();
}


void OptionDlg::ThroughFocusUI_Update()
{
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_AF_THROUGH_FOCUS_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_LEFT_UP))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_UP))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_LEFT_DOWN))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_DOWN))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_CHECK_AF_THROUGH_FOCUS_ENABLE_ENGINER_MODE))->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_LEFT_UP))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_UP))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_LEFT_DOWN))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_THROUGH_FOCUS_RIGHT_DOWN))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_CHECK_AF_THROUGH_FOCUS_ENABLE_ENGINER_MODE))->EnableWindow(TRUE);
	}
}


void OptionDlg::OnBnClickedCheckAfEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	AF_UI_Update();
}


void OptionDlg::AF_UI_Update()
{
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_AF_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CEdit*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_FRAME_DELAY))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_START_CODE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_END_CODE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_STATION_NAME))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_AV_SPEC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_BMP_MODE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_FULL_MODE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_DEMOSAIC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_LAST_MODE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_RAW_BALANCE))->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_COMBO_AF_PARA_AV_ALGORITHM))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_FRAME_DELAY))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_START_CODE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_END_CODE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_STATION_NAME))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_AF_PARA_AV_SPEC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_BMP_MODE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_FULL_MODE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_DEMOSAIC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_LAST_MODE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_RADIO_AF_SFR_RAW_BALANCE))->EnableWindow(TRUE);
	}
}

void OptionDlg::OnBnClickedCheckFinalChkEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	FinalCheck_UI_Update();
}

void OptionDlg::FinalCheck_UI_Update()
{
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_FINAL_CHK_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CEdit*)GetDlgItem(IDC_CHECK_FINAL_CHK_AF))->EnableWindow(FALSE);
		((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_FINAL_CHK_AF_RANGE))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_FINAL_CHK_CODE_SPEC))->EnableWindow(FALSE);
	}
	else
	{
		((CEdit*)GetDlgItem(IDC_CHECK_FINAL_CHK_AF))->EnableWindow(TRUE);
		((CComboBox*)GetDlgItem(IDC_COMBO_FINAL_CHK_AF_CODE_NAME))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_FINAL_CHK_AF_RANGE))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_FINAL_CHK_CODE_SPEC))->EnableWindow(TRUE);
	}
}

void OptionDlg::OnBnClickedButtonSfrSquareChartPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_dlgSquareChartSfr.DoModal() == IDCANCEL)
		return;

	m_pTestItem->SaveSquareChartSfrPara();
}


void OptionDlg::OnBnClickedButtonSfrXchartPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_dlgSfr.DoModal() == IDCANCEL)
		return;

	m_pTestItem->SaveXChartSfrPara();
}


void OptionDlg::OnBnClickedButtonSfrMotoChartPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_dlgMotoChartSfr.DoModal() == IDCANCEL)
		return;

	m_pTestItem->SaveMotoChartSfrPara();
}
