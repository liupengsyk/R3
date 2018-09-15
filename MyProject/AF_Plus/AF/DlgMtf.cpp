// DlgMtf.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "DlgMtf.h"
#include "afxdialogex.h"


// CDlgMtf 对话框

IMPLEMENT_DYNAMIC(CDlgMtf, CDialogEx)

CDlgMtf::CDlgMtf(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMtf::IDD, pParent)
{
	m_mtfPara.roiWidth = _T("10");
	m_mtfPara.roiHeight = _T("10");
	m_mtfPara.cornerDiffSpec = _T("100");

	m_mtfPara.roiPara[0].enable = TRUE;
	m_mtfPara.roiPara[0].cx = _T("50");
	m_mtfPara.roiPara[0].cy = _T("50");
	m_mtfPara.roiPara[0].limit1 = _T("40");
	m_mtfPara.roiPara[0].limit2 = _T("30");

	m_mtfPara.roiPara[1].enable = TRUE;
	m_mtfPara.roiPara[1].cx = _T("20");
	m_mtfPara.roiPara[1].cy = _T("20");
	m_mtfPara.roiPara[1].limit1 = _T("40");
	m_mtfPara.roiPara[1].limit2 = _T("30");

	m_mtfPara.roiPara[2].enable = TRUE;
	m_mtfPara.roiPara[2].cx = _T("80");
	m_mtfPara.roiPara[2].cy = _T("20");
	m_mtfPara.roiPara[2].limit1 = _T("40");
	m_mtfPara.roiPara[2].limit2 = _T("30");

	m_mtfPara.roiPara[3].enable = TRUE;
	m_mtfPara.roiPara[3].cx = _T("20");
	m_mtfPara.roiPara[3].cy = _T("80");
	m_mtfPara.roiPara[3].limit1 = _T("40");
	m_mtfPara.roiPara[3].limit2 = _T("30");

	m_mtfPara.roiPara[4].enable = TRUE;
	m_mtfPara.roiPara[4].cx = _T("80");
	m_mtfPara.roiPara[4].cy = _T("80");
	m_mtfPara.roiPara[4].limit1 = _T("40");
	m_mtfPara.roiPara[4].limit2 = _T("30");
}

CDlgMtf::~CDlgMtf()
{
}

void CDlgMtf::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_MTF_ROI_WIDTH, m_mtfPara.roiWidth);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI_HEIGHT, m_mtfPara.roiHeight);
	DDX_Text(pDX, IDC_EDIT_MTF_CORNER_DIFF, m_mtfPara.cornerDiffSpec);

	DDX_Check(pDX, IDC_CHECK_MTF_00, m_mtfPara.roiPara[0].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI00_X, m_mtfPara.roiPara[0].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI00_Y, m_mtfPara.roiPara[0].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI00_LIMIT1, m_mtfPara.roiPara[0].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI00_LIMIT2, m_mtfPara.roiPara[0].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_01, m_mtfPara.roiPara[1].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI01_X, m_mtfPara.roiPara[1].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI01_Y, m_mtfPara.roiPara[1].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI01_LIMIT1, m_mtfPara.roiPara[1].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI01_LIMIT2, m_mtfPara.roiPara[1].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_02, m_mtfPara.roiPara[2].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI02_X, m_mtfPara.roiPara[2].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI02_Y, m_mtfPara.roiPara[2].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI02_LIMIT1, m_mtfPara.roiPara[2].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI02_LIMIT2, m_mtfPara.roiPara[2].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_03, m_mtfPara.roiPara[3].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI03_X, m_mtfPara.roiPara[3].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI03_Y, m_mtfPara.roiPara[3].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI03_LIMIT1, m_mtfPara.roiPara[3].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI03_LIMIT2, m_mtfPara.roiPara[3].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_04, m_mtfPara.roiPara[4].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI04_X, m_mtfPara.roiPara[4].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI04_Y, m_mtfPara.roiPara[4].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI04_LIMIT1, m_mtfPara.roiPara[4].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI04_LIMIT2, m_mtfPara.roiPara[4].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_05, m_mtfPara.roiPara[5].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI05_X, m_mtfPara.roiPara[5].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI05_Y, m_mtfPara.roiPara[5].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI05_LIMIT1, m_mtfPara.roiPara[5].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI05_LIMIT2, m_mtfPara.roiPara[5].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_06, m_mtfPara.roiPara[6].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI06_X, m_mtfPara.roiPara[6].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI06_Y, m_mtfPara.roiPara[6].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI06_LIMIT1, m_mtfPara.roiPara[6].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI06_LIMIT2, m_mtfPara.roiPara[6].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_07, m_mtfPara.roiPara[7].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI07_X, m_mtfPara.roiPara[7].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI07_Y, m_mtfPara.roiPara[7].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI07_LIMIT1, m_mtfPara.roiPara[7].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI07_LIMIT2, m_mtfPara.roiPara[7].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_08, m_mtfPara.roiPara[8].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI08_X, m_mtfPara.roiPara[8].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI08_Y, m_mtfPara.roiPara[8].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI08_LIMIT1, m_mtfPara.roiPara[8].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI08_LIMIT2, m_mtfPara.roiPara[8].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_09, m_mtfPara.roiPara[9].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI09_X, m_mtfPara.roiPara[9].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI09_Y, m_mtfPara.roiPara[9].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI09_LIMIT1, m_mtfPara.roiPara[9].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI09_LIMIT2, m_mtfPara.roiPara[9].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_10, m_mtfPara.roiPara[10].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI10_X, m_mtfPara.roiPara[10].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI10_Y, m_mtfPara.roiPara[10].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI10_LIMIT1, m_mtfPara.roiPara[10].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI10_LIMIT2, m_mtfPara.roiPara[10].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_11, m_mtfPara.roiPara[11].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI11_X, m_mtfPara.roiPara[11].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI11_Y, m_mtfPara.roiPara[11].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI11_LIMIT1, m_mtfPara.roiPara[11].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI11_LIMIT2, m_mtfPara.roiPara[11].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_12, m_mtfPara.roiPara[12].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI12_X, m_mtfPara.roiPara[12].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI12_Y, m_mtfPara.roiPara[12].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI12_LIMIT1, m_mtfPara.roiPara[12].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI12_LIMIT2, m_mtfPara.roiPara[12].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_13, m_mtfPara.roiPara[13].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI13_X, m_mtfPara.roiPara[13].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI13_Y, m_mtfPara.roiPara[13].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI13_LIMIT1, m_mtfPara.roiPara[13].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI13_LIMIT2, m_mtfPara.roiPara[13].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_14, m_mtfPara.roiPara[14].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI14_X, m_mtfPara.roiPara[14].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI14_Y, m_mtfPara.roiPara[14].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI14_LIMIT1, m_mtfPara.roiPara[14].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI14_LIMIT2, m_mtfPara.roiPara[14].limit2);

	DDX_Check(pDX, IDC_CHECK_MTF_15, m_mtfPara.roiPara[15].enable);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI15_X, m_mtfPara.roiPara[15].cx);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI15_Y, m_mtfPara.roiPara[15].cy);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI15_LIMIT1, m_mtfPara.roiPara[15].limit1);
	DDX_Text(pDX, IDC_EDIT_MTF_ROI15_LIMIT2, m_mtfPara.roiPara[15].limit2);
}


BEGIN_MESSAGE_MAP(CDlgMtf, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMtf::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgMtf 消息处理程序


BOOL CDlgMtf::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMtf::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
