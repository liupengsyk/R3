// AutoFocusOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoFocus.h"
#include "AutoFocusOptionDlg.h"
#include "afxdialogex.h"
#include "ccmBaseInterface.h"
#include "Inspection_AutoFocus.h"

// AutoFocusOptionDlg 对话框

IMPLEMENT_DYNAMIC(AutoFocusOptionDlg, CDialog)

AutoFocusOptionDlg::AutoFocusOptionDlg(CWnd* pParent, Inspection_AutoFocus* pInspection)
	: CDialog(AutoFocusOptionDlg::IDD, pParent)
{
	//TODO 添加创建对话框
	m_pInspection = pInspection;
	m_StartCode = 0;
	m_ROI_Width = 0;
	m_Resolution_Limit = 0;

	m_ROI_cx = 0;
	m_ROI_cy = 0;

	m_ROI_Height = 0;


	m_FramePitchLimit_1 = 0;
	m_FramePitchLimit_2 = 0;
	m_FramePitchLimit_3 = 0;
	m_FramePitchLimit_4 = 0;
	m_bUseDelayTime = 0;


	m_SearchTime = 1;
	m_StepCode_1 = 1;
	m_StepCode_2 = 1;
	m_StepCode_3 = 1;
	m_StepCode_4 = 1;
	m_Diff_1 = 0.0;
	m_Diff_2 = 0.0;
	m_Diff_3 = 0.0;
	m_Diff_4 = 0.0;
	m_DescendTime_1 = 1;
	m_DescendTime_2 = 1;
	m_DescendTime_3 = 1;	
	m_DescendTime_4 = 1;	
	m_Code_lowerlimit = 0;
	m_Code_upperlimit = 0;

	m_bAutoFocus  = FALSE;

	m_bGetTemperature2 = FALSE;
	m_tempOnOffReg = _T("");
	m_tempSlaveId = _T("");
	m_tempAddrReg = _T("");

	str_ResolutionResult_Alg = _T("");
	str_Alg_Type = _T("");

	m_roiContourLenght = 200;
	m_roiWidth = 80;
	m_roiHeight = 60;
	m_roiFov = 80;

	m_roiSfrFreq = 4;
	m_bUseCruve =0;

	m_bSaveData = 0;
	m_CruveUseStep = 2;
	m_UsePoint = 3;

	m_ErrorCode = 1;
	m_bTimesLog = 0;
	m_Resolution_Limit_Diff = 0;
	m_NGTimes = 0;
}

AutoFocusOptionDlg::~AutoFocusOptionDlg()
{
}

void AutoFocusOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_START_CODE, m_StartCode);
	DDV_MinMaxInt(pDX, m_StartCode, 0, 65535);
	DDX_Text(pDX, IDC_ROI_WIDTH, m_ROI_Width);
	DDV_MinMaxInt(pDX, m_ROI_Width, 0, 100);
	DDX_Text(pDX, IDC_RESOLUTION_LIMIT, m_Resolution_Limit);

	DDX_Text(pDX, IDC_CENTER_Y, m_ROI_cx);
	DDV_MinMaxInt(pDX, m_ROI_cx, 0, 100);
	DDX_Text(pDX, IDC_CENTER_X, m_ROI_cy);
	DDV_MinMaxInt(pDX, m_ROI_cy, 0, 100); 
	DDX_Text(pDX, IDC_ROI_HEIGHT, m_ROI_Height);
	DDV_MinMaxInt(pDX, m_ROI_Height, 0, 100);

	DDX_Text(pDX, IDC_FRAME_PITCH, m_FramePitchLimit_1);
	DDV_MinMaxInt(pDX, m_FramePitchLimit_1, 0, 10);
	DDX_Text(pDX, IDC_FRAME_PITCH2, m_FramePitchLimit_2);
	DDV_MinMaxInt(pDX, m_FramePitchLimit_2, 0, 10);
	DDX_Text(pDX, IDC_FRAME_PITCH4, m_FramePitchLimit_3);
	DDV_MinMaxInt(pDX, m_FramePitchLimit_3, 0, 10);
	DDX_Text(pDX, IDC_FRAME_PITCH5, m_FramePitchLimit_4);
	DDV_MinMaxInt(pDX, m_FramePitchLimit_4, 0, 10);

	DDX_CBIndex(pDX, IDC_SEARCHTIME, m_SearchTime);
	DDV_MinMaxInt(pDX, m_SearchTime, 0, 4);

	DDX_Text(pDX, IDC_STEPCODE_1, m_StepCode_1);
	DDV_MinMaxInt(pDX, m_StepCode_1, 0, 65535);
	DDX_Text(pDX, IDC_STEPCODE_2, m_StepCode_2);
	DDV_MinMaxInt(pDX, m_StepCode_2, 0, 65535);
	DDX_Text(pDX, IDC_STEPCODE_3, m_StepCode_3);
	DDV_MinMaxInt(pDX, m_StepCode_3, 0, 65535);
	DDX_Text(pDX, IDC_STEPCODE_4, m_StepCode_4);
	DDV_MinMaxInt(pDX, m_StepCode_4, 0, 65535);

	DDX_Text(pDX, IDC_DESCENDTIME_1, m_DescendTime_1);
	DDV_MinMaxInt(pDX, m_DescendTime_1, 1, 1000000);
	DDX_Text(pDX, IDC_DESCENDTIME_2, m_DescendTime_2);
	DDV_MinMaxInt(pDX, m_DescendTime_2, 1, 1000000);
	DDX_Text(pDX, IDC_DESCENDTIME_3, m_DescendTime_3);
	DDV_MinMaxInt(pDX, m_DescendTime_3, 1, 1000000);
	DDX_Text(pDX, IDC_DESCENDTIME_4, m_DescendTime_4);
	DDV_MinMaxInt(pDX, m_DescendTime_4, 1, 1000000);

	DDX_Text(pDX, IDC_DIFF_1, m_Diff_1);
	DDV_MinMaxDouble(pDX, m_Diff_1, 0, 100);
	DDX_Text(pDX, IDC_DIFF_2, m_Diff_2);
	DDV_MinMaxDouble(pDX, m_Diff_2, 0, 100);
	DDX_Text(pDX, IDC_DIFF_3, m_Diff_3);
	DDV_MinMaxDouble(pDX, m_Diff_3, 0, 100);
	DDX_Text(pDX, IDC_DIFF_4, m_Diff_4);
	DDV_MinMaxDouble(pDX, m_Diff_4, 0, 100);

	DDX_Text(pDX, IDC_CODE_MAX, m_Code_upperlimit);
	DDV_MinMaxInt(pDX, m_Code_upperlimit, 0, 1024);
	DDX_Text(pDX, IDC_CODE_MIN, m_Code_lowerlimit);
	DDV_MinMaxInt(pDX, m_Code_lowerlimit, 0, 1024);
	//	DDX_Radio(pDX, IDC_RESOLUTIONLIMIT_ALG, m_ResolutionLimit_Alg);
	////	DDX_Check(pDX, IDC_CHECK_POSITION, m_PosPrompt);

	DDX_Check(pDX, IDC_CheckAutoFocus, m_bAutoFocus);

	DDX_Check(pDX, IDC_bGetTemperature, bGetTemperature);
	DDX_Check(pDX, IDC_bBestCodeForOtp, bBestCodeForOtp);

	DDX_Text(pDX, IDC_EDIT_SPV3_BURN_ITEM, m_burnItem);

	DDX_Check(pDX, IDC_bGetTemperature2, m_bGetTemperature2);
	DDX_Text(pDX, IDC_EDIT_TEMP_ONOFF_REG, m_tempOnOffReg);
	DDX_Text(pDX, IDC_EDIT_TEMP_SLAVEID, m_tempSlaveId);
	DDX_Text(pDX, IDC_EDIT_TEMP_ADDR_REG, m_tempAddrReg);

	DDX_Text(pDX, IDC_EDIT_SFR_ROI_CL, m_roiContourLenght);
	DDX_Text(pDX, IDC_EDIT_SFR_ROI_WIDTH, m_roiWidth);
	DDX_Text(pDX, IDC_EDIT_SFR_ROI_HEIGHT, m_roiHeight);
	DDX_Text(pDX, IDC_EDIT_SFR_ROI_FOV, m_roiFov);	 
	DDX_Text(pDX, IDC_EDIT_SFR_FREQ, m_roiSfrFreq);

	DDX_Text(pDX, IDC_COMBO_SOLUTION_FOCUS, str_Alg_Type);
	DDX_Text(pDX, IDC_COMBO_SOLUTION_RESULT, str_ResolutionResult_Alg);

	DDX_Radio(pDX,IDC_RADIO1,m_ImgType);

	DDX_Check(pDX,IDC_bGetTemperature3,m_bSaveData);
	DDX_Check(pDX,IDC_CheckAutoFocus2,m_bUseCruve);

	DDX_Text(pDX,IDC_START_CODE2,m_CruveUseStep);
	DDX_Text(pDX,IDC_START_CODE3,m_UsePoint);

	DDX_Text(pDX,IDC_ROI_HEIGHT2,m_ErrorCode);
	DDX_Check(pDX,IDC_bTimesLog,m_bTimesLog);

	DDX_Check(pDX,IDC_CHECK2,m_bUseDelayTime);
	DDX_Text(pDX, IDC_RESOLUTION_LIMIT_Diff, m_Resolution_Limit_Diff);
	DDX_Text(pDX, IDC_NGTime, m_NGTimes);
}


BEGIN_MESSAGE_MAP(AutoFocusOptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &AutoFocusOptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &AutoFocusOptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &AutoFocusOptionDlg::OnCameraStop)
END_MESSAGE_MAP()


// AutoFocusOptionDlg 消息处理程序


void AutoFocusOptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

LRESULT AutoFocusOptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	BOOL	bSaveAndValidate = (BOOL) wParam;

	BOOL ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{
	}
	else
	{
	}

	return ret;
}

LRESULT AutoFocusOptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{
	m_pInspection->OnCtrlKeyNum();

	return 1;
}

BOOL AutoFocusOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CComboBox* SearchTime = (CComboBox*)GetDlgItem(IDC_SEARCHTIME);
	SearchTime->AddString(_T("1"));
	SearchTime->AddString(_T("2"));
	SearchTime->AddString(_T("3"));
	SearchTime->AddString(_T("4"));

	CComboBox* focus_solution = (CComboBox*)GetDlgItem(IDC_COMBO_SOLUTION_FOCUS);
	focus_solution->AddString(_T("MTF"));
	focus_solution->AddString(_T("差分"));
	focus_solution->AddString(_T("SFR"));
	focus_solution->AddString(_T("QCAF"));
	focus_solution->AddString(L"TenenGradS");
	focus_solution->AddString(L"TenenGrad4");
	focus_solution->AddString(L"TenenGrad2");
	 

	CComboBox* result_solution = (CComboBox*)GetDlgItem(IDC_COMBO_SOLUTION_RESULT);
	result_solution->AddString(_T("MTF"));
	result_solution->AddString(_T("差分"));
	result_solution->AddString(_T("SFR"));
	result_solution->AddString(L"TenenGradS");
	result_solution->AddString(L"TenenGrad4");
	result_solution->AddString(L"TenenGrad2");
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT AutoFocusOptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{
	 
	return 0;
}
