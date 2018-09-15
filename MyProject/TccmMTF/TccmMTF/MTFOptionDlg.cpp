// MTFOptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TccmMTF.h"
#include "MTFOptionDlg.h"
#include "afxdialogex.h"
#include "ccmBaseInterface.h"
#include "TestItemInterface.h"
#include "Inspection_MTF.h"

// MTFOptionDlg 对话框

IMPLEMENT_DYNAMIC(MTFOptionDlg, CDialog)

MTFOptionDlg::MTFOptionDlg(CWnd* pParent /*=NULL*/,Inspection_MTF* pInspection)
	: CDialog(MTFOptionDlg::IDD, pParent)
	, m_iUseRaw8Test(0)
{
	this->pInspection = pInspection;

	m_exp_target =    1;
	m_exp_tolerance = 0;
	m_ExposureEn =    FALSE;
	m_HVRatio =       0.0;
	m_Continue   =    FALSE;
	m_DequeSize =     0;
	m_tempInfo = FALSE;
}

MTFOptionDlg::~MTFOptionDlg()
{
}

void MTFOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_MTF_WIDTH, pInspection->sMTF.ROI_width);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI_width, 1, 100);
	DDX_Text(pDX, IDC_MTF_HEIGHT, pInspection->sMTF.ROI_height);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI_height, 1, 100);
	DDX_Check(pDX, IDC_MTF0_CHECK, pInspection->sMTF.ROI[0].Enable);
	DDX_Check(pDX, IDC_MTF10_CHECK, pInspection->sMTF.ROI[10].Enable);
	DDX_Check(pDX, IDC_MTF11_CHECK, pInspection->sMTF.ROI[11].Enable);
	DDX_Check(pDX, IDC_MTF12_CHECK, pInspection->sMTF.ROI[12].Enable);
	DDX_Check(pDX, IDC_MTF13_CHECK, pInspection->sMTF.ROI[13].Enable);
	DDX_Check(pDX, IDC_MTF14_CHECK, pInspection->sMTF.ROI[14].Enable);
	DDX_Check(pDX, IDC_MTF15_CHECK, pInspection->sMTF.ROI[15].Enable);
	DDX_Check(pDX, IDC_MTF2_CHECK, pInspection->sMTF.ROI[2].Enable);
	DDX_Check(pDX, IDC_MTF3_CHECK, pInspection->sMTF.ROI[3].Enable);
	DDX_Check(pDX, IDC_MTF4_CHECK, pInspection->sMTF.ROI[4].Enable);
	DDX_Check(pDX, IDC_MTF5_CHECK, pInspection->sMTF.ROI[5].Enable);
	DDX_Check(pDX, IDC_MTF6_CHECK, pInspection->sMTF.ROI[6].Enable);
	DDX_Check(pDX, IDC_MTF7_CHECK, pInspection->sMTF.ROI[7].Enable);
	DDX_Check(pDX, IDC_MTF8_CHECK, pInspection->sMTF.ROI[8].Enable);
	DDX_Check(pDX, IDC_MTF9_CHECK, pInspection->sMTF.ROI[9].Enable);
	DDX_Check(pDX, IDC_MTF1_CHECK, pInspection->sMTF.ROI[1].Enable);

	DDX_Text(pDX, IDC_MTF0_Limit1, pInspection->sMTF.ROI[0].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF0_Limit2, pInspection->sMTF.ROI[0].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF1_Limit1, pInspection->sMTF.ROI[1].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF1_Limit2, pInspection->sMTF.ROI[1].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF2_Limit1, pInspection->sMTF.ROI[2].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF2_Limit2, pInspection->sMTF.ROI[2].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF3_Limit1, pInspection->sMTF.ROI[3].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF3_Limit2, pInspection->sMTF.ROI[3].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF4_Limit1, pInspection->sMTF.ROI[4].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF4_Limit2, pInspection->sMTF.ROI[4].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF5_Limit1, pInspection->sMTF.ROI[5].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[5].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF5_Limit2, pInspection->sMTF.ROI[5].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[5].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF6_Limit1, pInspection->sMTF.ROI[6].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[6].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF6_Limit2, pInspection->sMTF.ROI[6].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[6].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF7_Limit1, pInspection->sMTF.ROI[7].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[7].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF7_Limit2, pInspection->sMTF.ROI[7].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[7].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF8_Limit1, pInspection->sMTF.ROI[8].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[8].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF8_Limit2, pInspection->sMTF.ROI[8].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[8].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF9_Limit1, pInspection->sMTF.ROI[9].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[9].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF9_Limit2, pInspection->sMTF.ROI[9].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[9].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF10_Limit1, pInspection->sMTF.ROI[10].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[10].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF10_Limit2, pInspection->sMTF.ROI[10].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[10].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF11_Limit1, pInspection->sMTF.ROI[11].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[11].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF11_Limit2, pInspection->sMTF.ROI[11].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[11].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF12_Limit1, pInspection->sMTF.ROI[12].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[12].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF12_Limit2, pInspection->sMTF.ROI[12].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[12].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF13_Limit1, pInspection->sMTF.ROI[13].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[13].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF13_Limit2, pInspection->sMTF.ROI[13].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[13].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF14_Limit1, pInspection->sMTF.ROI[14].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[14].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF14_Limit2, pInspection->sMTF.ROI[14].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[14].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF15_Limit1, pInspection->sMTF.ROI[15].limit1);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[15].limit1, 0, 100);
	DDX_Text(pDX, IDC_MTF15_Limit2, pInspection->sMTF.ROI[15].limit2);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[15].limit2, 0, 100);

	DDX_Text(pDX, IDC_MTF0_x, pInspection->sMTF.ROI[0].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF0_y, pInspection->sMTF.ROI[0].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[0].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF1_x, pInspection->sMTF.ROI[1].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF1_y, pInspection->sMTF.ROI[1].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[1].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF2_x, pInspection->sMTF.ROI[2].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF2_y, pInspection->sMTF.ROI[2].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[2].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF3_x, pInspection->sMTF.ROI[3].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF3_y, pInspection->sMTF.ROI[3].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[3].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF4_x, pInspection->sMTF.ROI[4].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF4_y, pInspection->sMTF.ROI[4].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[4].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF5_x, pInspection->sMTF.ROI[5].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[5].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF5_y, pInspection->sMTF.ROI[5].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[5].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF6_x, pInspection->sMTF.ROI[6].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[6].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF6_y, pInspection->sMTF.ROI[6].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[6].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF7_x, pInspection->sMTF.ROI[7].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[7].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF7_y, pInspection->sMTF.ROI[7].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[7].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF8_x, pInspection->sMTF.ROI[8].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[8].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF8_y, pInspection->sMTF.ROI[8].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[8].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF9_x, pInspection->sMTF.ROI[9].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[9].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF9_y, pInspection->sMTF.ROI[9].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[9].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF10_x, pInspection->sMTF.ROI[10].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[10].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF10_y, pInspection->sMTF.ROI[10].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[10].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF11_x, pInspection->sMTF.ROI[11].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[11].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF11_y, pInspection->sMTF.ROI[11].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[11].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF12_x, pInspection->sMTF.ROI[12].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[12].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF12_y, pInspection->sMTF.ROI[12].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[12].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF13_x, pInspection->sMTF.ROI[13].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[13].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF13_y, pInspection->sMTF.ROI[13].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[13].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF14_x, pInspection->sMTF.ROI[14].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[14].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF14_y, pInspection->sMTF.ROI[14].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[14].cy, 0, 100);

	DDX_Text(pDX, IDC_MTF15_x, pInspection->sMTF.ROI[15].cx);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[15].cx, 0, 100);
	DDX_Text(pDX, IDC_MTF15_y, pInspection->sMTF.ROI[15].cy);
	DDV_MinMaxInt(pDX, pInspection->sMTF.ROI[15].cy, 0, 100);

	DDX_Check(pDX, IDC_CONTINUE, m_Continue);
	DDX_Check(pDX, IDC_CHECKEXPOSURE, m_ExposureEn);
	DDX_Text(pDX, IDC_MTF_EXPTARGET, m_exp_target);
	DDV_MinMaxInt(pDX, m_exp_target, 1, 255);
	DDX_Text(pDX, IDC_MTF_EXPTOLERANCE, m_exp_tolerance);
	DDV_MinMaxInt(pDX, m_exp_tolerance, 0, 255);

	DDX_Text(pDX, IDC_HVRatio, pInspection->sMTF.HVratioLimit);
	DDX_CBIndex(pDX, IDC_DEQUE, pInspection->sMTF.DequeSize);
	DDV_MinMaxInt(pDX, pInspection->sMTF.DequeSize, 0, 10);

	DDX_Text(pDX, IDC_Diff, pInspection->sMTF.CornerDiffLimit);
	DDV_MinMaxInt(pDX, pInspection->sMTF.CornerDiffLimit, 0, 100);

	DDX_Text(pDX, IDC_ROTATEBACK_LIMIT1, pInspection->RotateBack_MTF_LowerLimit);
	DDV_MinMaxInt(pDX, pInspection->RotateBack_MTF_LowerLimit, 0, 100);

	DDX_Text(pDX, IDC_ROTATEBACK_LIMIT2, pInspection->RotateBack_MTF_UpperLimit);
	DDV_MinMaxInt(pDX, pInspection->RotateBack_MTF_UpperLimit, 0, 100);

	DDX_Check(pDX, IDC_ROTATEBACK_EN, pInspection->RotateBack_en);

	DDX_Check(pDX, IDC_Rotate, pInspection->m_bRotate);

	DDX_Text(pDX, IDC_Delay, m_delay);
	DDV_MinMaxInt(pDX, m_delay, 0, 100);

	DDX_Check(pDX, IDC_CHECK_TEMP_INFO, m_tempInfo);
	DDX_Check(pDX, IDC_CHECK_USE_RAW8_TEST, m_iUseRaw8Test);
}


BEGIN_MESSAGE_MAP(MTFOptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &MTFOptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &MTFOptionDlg::OnCtrlKeyNum)
END_MESSAGE_MAP()


// MTFOptionDlg 消息处理程序

LRESULT MTFOptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	BOOL bSaveAndValidate = (BOOL) wParam;

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

void MTFOptionDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

BOOL MTFOptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComboBox* DequeSize = (CComboBox*)GetDlgItem(IDC_DEQUE);

	CString str;
	for (int i = 1; i<= 10 ; i++)
	{
		str.Format(_T("%d"),i);
	   DequeSize->AddString(str);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT MTFOptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{
	pInspection->OnCtrlKeyNum();
	return 1;
}
