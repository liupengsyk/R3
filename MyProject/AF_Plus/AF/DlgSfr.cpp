// DlgSfr.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "DlgSfr.h"
#include "afxdialogex.h"


// CDlgSfr 对话框

IMPLEMENT_DYNAMIC(CDlgSfr, CDialogEx)

CDlgSfr::CDlgSfr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSfr::IDD, pParent)
{
	m_xChartSfrPara.imgWidth = _T("0");
	m_xChartSfrPara.imgHeight = _T("0");
	m_xChartSfrPara.roiWidth = _T("64");
	m_xChartSfrPara.roiHeight = _T("64");
	m_xChartSfrPara.roiMaskWidth = _T("250");
	m_xChartSfrPara.roiMaskHeight = _T("250");

	m_xChartSfrPara.cornerDiffPara.enable = FALSE;
	m_xChartSfrPara.cornerDiffPara.diffAbsolute = TRUE;
	m_xChartSfrPara.cornerDiffPara.spec = _T("1.00");
	m_xChartSfrPara.cornerDiffPara.specH = _T("1.00");
	m_xChartSfrPara.cornerDiffPara.specV = _T("1.00");
	m_xChartSfrPara.cornerDiffPara.diffOffset = _T("0.00");
	m_xChartSfrPara.cornerDiffPara.diffOffsetH = _T("0.00");
	m_xChartSfrPara.cornerDiffPara.diffOffsetV = _T("0.00");
	m_xChartSfrPara.cornerDiffPara.roiNumLU = _T("0");
	m_xChartSfrPara.cornerDiffPara.roiNumRU = _T("0");
	m_xChartSfrPara.cornerDiffPara.roiNumLD = _T("0");
	m_xChartSfrPara.cornerDiffPara.roiNumRD = _T("0");

	m_xChartSfrPara.roiPara[0].topEn = TRUE;
	m_xChartSfrPara.roiPara[0].bottomEn = TRUE;
	m_xChartSfrPara.roiPara[0].leftEn = TRUE;
	m_xChartSfrPara.roiPara[0].rightEn = TRUE;
	m_xChartSfrPara.roiPara[0].cX = _T("1296");
	m_xChartSfrPara.roiPara[0].cY = _T("972");
	m_xChartSfrPara.roiPara[0].freq = _T("0.125");
	m_xChartSfrPara.roiPara[0].sfrOffsetH = _T("0");
	m_xChartSfrPara.roiPara[0].sfrOffsetV = _T("0");
	m_xChartSfrPara.roiPara[0].limitH = _T("0.2");
	m_xChartSfrPara.roiPara[0].limitV = _T("0.2");
	m_xChartSfrPara.roiPara[0].fov = _T("");

	m_addParaByFov = _T("None");
	m_roiTopEn = TRUE;
	m_roiBottomEn = TRUE;
	m_roiLeftEn = TRUE;
	m_roiRightEn = TRUE;
	m_pX = _T("0");
	m_pY = _T("0");
	m_freq = _T("0.125");
	m_sfrOffsetH = _T("0.0000");
	m_sfrOffsetV = _T("0.0000");
	m_specH = _T("0.0000");
	m_specV = _T("0.0000");

	m_selectedItemNum = -1;
}

CDlgSfr::~CDlgSfr()
{
}

void CDlgSfr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_XCHART_ROI_PARA, m_listXChartRoiPara);

	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_IMG_WIDTH, m_xChartSfrPara.imgWidth);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_IMG_HEIGHT, m_xChartSfrPara.imgHeight);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_WIDTH, m_xChartSfrPara.roiWidth);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_HEIGHT, m_xChartSfrPara.roiHeight);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_MASK_WIDTH, m_xChartSfrPara.roiMaskWidth);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_MASK_HEIGHT, m_xChartSfrPara.roiMaskHeight);

	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_BY_FOV, m_addParaByFov);
	DDX_Check(pDX, IDC_CHECK_XCHAR_SFR_ROI_TOP_ENABLE, m_roiTopEn);
	DDX_Check(pDX, IDC_CHECK_XCHAR_SFR_ROI_BOTTOM_ENABLE, m_roiBottomEn);
	DDX_Check(pDX, IDC_CHECK_XCHAR_SFR_ROI_LEFT_ENABLE, m_roiLeftEn);
	DDX_Check(pDX, IDC_CHECK_XCHAR_SFR_ROI_RIGHT_ENABLE, m_roiRightEn);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_PX, m_pX);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_PY, m_pY);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_FREQ, m_freq);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_OFFSET_H, m_sfrOffsetH);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_OFFSET_V, m_sfrOffsetV);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_SPEC_H, m_specH);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_SPEC_V, m_specV);
	DDX_Text(pDX, IDC_EDIT_XCHAR_SFR_ROI_MEMO, m_memo);

	DDX_Check(pDX, IDC_CHECK_XCHAR_CORNER_DIFF_ENABLE, m_xChartSfrPara.cornerDiffPara.enable);
	DDX_Check(pDX, IDC_CHECK_XCHAR_CORNER_DIFF_USEMINVALUE,m_xChartSfrPara.cornerDiffPara.diffMinValueEn);
	DDX_Check(pDX, IDC_CHECK_XCHAR_CORNER_DIFF_ABSOLUTE, m_xChartSfrPara.cornerDiffPara.diffAbsolute);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_SPEC, m_xChartSfrPara.cornerDiffPara.spec);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_H, m_xChartSfrPara.cornerDiffPara.specH);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_V, m_xChartSfrPara.cornerDiffPara.specV);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET, m_xChartSfrPara.cornerDiffPara.diffOffset);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_H, m_xChartSfrPara.cornerDiffPara.diffOffsetH);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_V, m_xChartSfrPara.cornerDiffPara.diffOffsetV);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_LU, m_xChartSfrPara.cornerDiffPara.roiNumLU);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_RU, m_xChartSfrPara.cornerDiffPara.roiNumRU);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_LD, m_xChartSfrPara.cornerDiffPara.roiNumLD);
	DDX_Text(pDX, IDC_EDIT_XCHAR_CORNER_DIFF_RD, m_xChartSfrPara.cornerDiffPara.roiNumRD);
}


BEGIN_MESSAGE_MAP(CDlgSfr, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_XCHAR_SFR_ADD_ROI_PARA, &CDlgSfr::OnBnClickedButtonXcharSfrAddRoiPara)
	ON_BN_CLICKED(IDOK, &CDlgSfr::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_XCHART_ROI_PARA, &CDlgSfr::OnNMClickListXchartRoiPara)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_XCHART_ROI_PARA, &CDlgSfr::OnNMDblclkListXchartRoiPara)
	ON_BN_CLICKED(IDC_BUTTON_XCHAR_SFR_UPDATE_ROI_PARA, &CDlgSfr::OnBnClickedButtonXcharSfrUpdateRoiPara)
	ON_BN_CLICKED(IDC_BUTTON_XCHAR_SFR_DEL_ALL_ROI_PARA, &CDlgSfr::OnBnClickedButtonXcharSfrDelAllRoiPara)
	ON_BN_CLICKED(IDC_BUTTON_XCHAR_SFR_DEL_ROI_PARA, &CDlgSfr::OnBnClickedButtonXcharSfrDelRoiPara)
	ON_BN_CLICKED(IDC_CHECK_XCHAR_CORNER_DIFF_ENABLE, &CDlgSfr::OnBnClickedCheckXcharCornerDiffEnable)
END_MESSAGE_MAP()


// CDlgSfr 消息处理程序


BOOL CDlgSfr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateListControl();

	((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_MEMO))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_MASK_WIDTH))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_MASK_HEIGHT))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_MASK_PHI))->EnableWindow(FALSE);

	Update_Corner_Diff_UI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSfr::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PreSubclassWindow();
}


void CDlgSfr::OnBnClickedButtonXcharSfrAddRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ((m_pX == "") || (m_pY == ""))
	{
		AfxMessageBox(_T("Please input \"pX\" and \"pY\" !"));
		return;
	}
	
	int px = _ttoi(m_pX), py = _ttoi(m_pY), w = _ttoi(m_xChartSfrPara.imgWidth), h = _ttoi(m_xChartSfrPara.imgHeight);
	if ((px < 0) || (px > w) || (py < 0) || (py > h))
	{
		AfxMessageBox(_T("\"pX\" or \"pY\" or \"imgWidth\" or \"imgHeight\" failed !"));
		return;
	}

	if (m_addParaByFov == "None")
	{
		CString tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listXChartRoiPara.GetItemCount());
		m_listXChartRoiPara.InsertItem(m_listXChartRoiPara.GetItemCount(), tmpS);
		if (m_roiTopEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("False"));
		if (m_roiBottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("False"));
		if (m_roiLeftEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("False"));
		if (m_roiRightEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("False"));
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 5, m_pX);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 6, m_pY);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 7, m_freq);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 8, m_sfrOffsetH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 9, m_sfrOffsetV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 10, m_specH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 11, m_specV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 12, _T(""));
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 13, m_memo);
		m_listXChartRoiPara.SetRedraw(TRUE);
	}
	else
	{
		float fov = _ttoi(m_addParaByFov) * 1.0f;
		px = (int)((w / 2.0f) - (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) - (h * fov / 200.0f) + 0.5f);
		CString tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listXChartRoiPara.GetItemCount());
		m_listXChartRoiPara.InsertItem(m_listXChartRoiPara.GetItemCount(), tmpS);
		if (m_roiTopEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("False"));
		if (m_roiBottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("False"));
		if (m_roiLeftEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("False"));
		if (m_roiRightEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("False"));
		CString t = _T("");
		t.Format(_T("%d"), px);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 5, t);
		t.Format(_T("%d"), py);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 6, t);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 7, m_freq);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 8, m_sfrOffsetH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 9, m_sfrOffsetV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 10, m_specH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 11, m_specV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 12, m_addParaByFov);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 13, m_memo);

		px = (int)((w / 2.0f) + (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) - (h * fov / 200.0f) + 0.5f);
		tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listXChartRoiPara.GetItemCount());
		m_listXChartRoiPara.InsertItem(m_listXChartRoiPara.GetItemCount(), tmpS);
		if (m_roiTopEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("False"));
		if (m_roiBottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("False"));
		if (m_roiLeftEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("False"));
		if (m_roiRightEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 5, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 6, tmpS);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 7, m_freq);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 8, m_sfrOffsetH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 9, m_sfrOffsetV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 10, m_specH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 11, m_specV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 12, m_addParaByFov);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 13, m_memo);

		px = (int)((w / 2.0f) - (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) + (h * fov / 200.0f) + 0.5f);
		tmpS.Format(_T("%03d"), m_listXChartRoiPara.GetItemCount());
		m_listXChartRoiPara.InsertItem(m_listXChartRoiPara.GetItemCount(), tmpS);
		if (m_roiTopEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("False"));
		if (m_roiBottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("False"));
		if (m_roiLeftEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("False"));
		if (m_roiRightEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 5, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 6, tmpS);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 7, m_freq);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 8, m_sfrOffsetH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 9, m_sfrOffsetV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 10, m_specH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 11, m_specV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 12, m_addParaByFov);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 13, m_memo);

		px = (int)((w / 2.0f) + (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) + (h * fov / 200.0f) + 0.5f);
		tmpS.Format(_T("%03d"), m_listXChartRoiPara.GetItemCount());
		m_listXChartRoiPara.InsertItem(m_listXChartRoiPara.GetItemCount(), tmpS);
		if (m_roiTopEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 1, _T("False"));
		if (m_roiBottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 2, _T("False"));
		if (m_roiLeftEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 3, _T("False"));
		if (m_roiRightEn == TRUE)
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 4, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 5, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 6, tmpS);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 7, m_freq);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 8, m_sfrOffsetH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 9, m_sfrOffsetV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 10, m_specH);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 11, m_specV);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 12, m_addParaByFov);
		m_listXChartRoiPara.SetItemText(m_listXChartRoiPara.GetItemCount()-1, 13, m_memo);

		m_addParaByFov = _T("None");
		UpdateData(FALSE);
	}
}


void CDlgSfr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_xChartSfrPara.roiCnt = m_listXChartRoiPara.GetItemCount();
	for (int i=0; i<m_xChartSfrPara.roiCnt; i++)
	{
		m_xChartSfrPara.roiPara[i].topEn = (m_listXChartRoiPara.GetItemText(i, 1).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_xChartSfrPara.roiPara[i].bottomEn = (m_listXChartRoiPara.GetItemText(i, 2).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_xChartSfrPara.roiPara[i].leftEn = (m_listXChartRoiPara.GetItemText(i, 3).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_xChartSfrPara.roiPara[i].rightEn = (m_listXChartRoiPara.GetItemText(i, 4).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_xChartSfrPara.roiPara[i].cX = m_listXChartRoiPara.GetItemText(i, 5);
		m_xChartSfrPara.roiPara[i].cY = m_listXChartRoiPara.GetItemText(i, 6);
		m_xChartSfrPara.roiPara[i].freq = m_listXChartRoiPara.GetItemText(i, 7);
		m_xChartSfrPara.roiPara[i].sfrOffsetH = m_listXChartRoiPara.GetItemText(i, 8);
		m_xChartSfrPara.roiPara[i].sfrOffsetV = m_listXChartRoiPara.GetItemText(i, 9);
		m_xChartSfrPara.roiPara[i].limitH = m_listXChartRoiPara.GetItemText(i, 10);
		m_xChartSfrPara.roiPara[i].limitV = m_listXChartRoiPara.GetItemText(i, 11);
		m_xChartSfrPara.roiPara[i].fov = m_listXChartRoiPara.GetItemText(i, 12);
		m_xChartSfrPara.roiPara[i].memo = m_listXChartRoiPara.GetItemText(i, 13);
	}

	CDialogEx::OnOK();
}


void CDlgSfr::OnNMClickListXchartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString tmpStr = _T("");
	m_selectedItemNum = pNMItemActivate->iItem;
	if (m_selectedItemNum != -1)
	{
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 1);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_TOP_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_TOP_ENABLE))->SetCheck(FALSE);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 2);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_BOTTOM_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_BOTTOM_ENABLE))->SetCheck(FALSE);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 3);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_LEFT_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_LEFT_ENABLE))->SetCheck(FALSE);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 4);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_RIGHT_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_RIGHT_ENABLE))->SetCheck(FALSE);

		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 5);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_PX)->SetWindowTextW(tmpStr);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 6);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_PY)->SetWindowTextW(tmpStr);
		
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 7);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_FREQ)->SetWindowTextW(tmpStr);

		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 8);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_OFFSET_H)->SetWindowTextW(tmpStr);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 9);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_OFFSET_V)->SetWindowTextW(tmpStr);

		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 10);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_SPEC_H)->SetWindowTextW(tmpStr);
		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 11);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_SPEC_V)->SetWindowTextW(tmpStr);

		tmpStr = m_listXChartRoiPara.GetItemText(m_selectedItemNum, 13);
		GetDlgItem(IDC_EDIT_XCHAR_SFR_ROI_MEMO)->SetWindowTextW(tmpStr);
	}
}


void CDlgSfr::OnNMDblclkListXchartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgSfr::OnBnClickedButtonXcharSfrUpdateRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp = _T("");
	if (m_selectedItemNum != -1)
	{
		strTmp.Format(_T("Update ROI: %03d parameter ?"), m_selectedItemNum);
		if (AfxMessageBox(strTmp, 1) == IDOK)
		{
			if (((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_TOP_ENABLE))->GetCheck() == TRUE)
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].topEn = TRUE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("True"));
			}
			else
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].topEn = FALSE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("False"));
			}
			if (((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_BOTTOM_ENABLE))->GetCheck() == TRUE)
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].bottomEn = TRUE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 2, _T("True"));
			}
			else
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].bottomEn = FALSE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 2, _T("False"));
			}
			if (((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_LEFT_ENABLE))->GetCheck() == TRUE)
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].leftEn = TRUE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 3, _T("True"));
			}
			else
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].leftEn = FALSE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 3, _T("False"));
			}
			if (((CButton*)GetDlgItem(IDC_CHECK_XCHAR_SFR_ROI_RIGHT_ENABLE))->GetCheck() == TRUE)
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].rightEn = TRUE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 4, _T("True"));
			}
			else
			{
				m_xChartSfrPara.roiPara[m_selectedItemNum].rightEn = FALSE;
				m_listXChartRoiPara.SetItemText(m_selectedItemNum, 4, _T("False"));
			}

			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_PX, m_xChartSfrPara.roiPara[m_selectedItemNum].cX);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_PY, m_xChartSfrPara.roiPara[m_selectedItemNum].cY);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_FREQ, m_xChartSfrPara.roiPara[m_selectedItemNum].freq);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_OFFSET_H, m_xChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetH);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_OFFSET_V, m_xChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetV);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_SPEC_H, m_xChartSfrPara.roiPara[m_selectedItemNum].limitH);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_SPEC_V, m_xChartSfrPara.roiPara[m_selectedItemNum].limitV);
			GetDlgItemTextW(IDC_EDIT_XCHAR_SFR_ROI_MEMO, m_xChartSfrPara.roiPara[m_selectedItemNum].memo);
			
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 5, m_xChartSfrPara.roiPara[m_selectedItemNum].cX);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 6, m_xChartSfrPara.roiPara[m_selectedItemNum].cY);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 7, m_xChartSfrPara.roiPara[m_selectedItemNum].freq);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 8, m_xChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetH);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 9, m_xChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetV);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 10, m_xChartSfrPara.roiPara[m_selectedItemNum].limitH);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 11, m_xChartSfrPara.roiPara[m_selectedItemNum].limitV);
			m_listXChartRoiPara.SetItemText(m_selectedItemNum, 13, m_xChartSfrPara.roiPara[m_selectedItemNum].memo);
		}
	}
}


void CDlgSfr::OnBnClickedButtonXcharSfrDelAllRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(_T("Delete all ROI parameter settings ?"), 1) == IDOK)
	{
		for (int i=0; i<64; i++)
		{
			m_xChartSfrPara.roiPara[i].topEn = FALSE;
			m_xChartSfrPara.roiPara[i].bottomEn = FALSE;
			m_xChartSfrPara.roiPara[i].leftEn = FALSE;
			m_xChartSfrPara.roiPara[i].rightEn = FALSE;
			m_xChartSfrPara.roiPara[i].cX = _T("0");
			m_xChartSfrPara.roiPara[i].cY = _T("0");
			m_xChartSfrPara.roiPara[i].freq = _T("0.125");
			m_xChartSfrPara.roiPara[i].sfrOffsetH = _T("0.0000");
			m_xChartSfrPara.roiPara[i].sfrOffsetV = _T("0.0000");
			m_xChartSfrPara.roiPara[i].limitH = _T("0.0000");
			m_xChartSfrPara.roiPara[i].limitV = _T("0.0000");
			m_xChartSfrPara.roiPara[i].fov = _T("");
			m_xChartSfrPara.roiPara[i].memo = _T("");
		}
		m_listXChartRoiPara.DeleteAllItems();
	}
	m_listXChartRoiPara.SetRedraw(TRUE);
}


void CDlgSfr::OnBnClickedButtonXcharSfrDelRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	int markItem = m_listXChartRoiPara.GetSelectionMark();
	if (markItem == -1)
		AfxMessageBox(_T("Please select a ROI parameter item .[-1]"));
	else
	{
		CString strT = _T("");
		strT.Format(_T("Delete \"%03d\" ROI parameter settings ?"), markItem);
		if (AfxMessageBox(strT, 1) == IDOK)
		{
			m_listXChartRoiPara.DeleteItem(markItem);
			for (int i=markItem; i<m_listXChartRoiPara.GetItemCount(); i++)
			{
				strT.Format(_T("%03d"), i);
				m_listXChartRoiPara.SetItemText(i, 0, strT);
			}
		}
	}
	m_listXChartRoiPara.SetRedraw(TRUE);
}


void CDlgSfr::OnBnClickedCheckXcharCornerDiffEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	Update_Corner_Diff_UI();
}


void CDlgSfr::UpdateListControl()
{
	m_listXChartRoiPara.InsertColumn(0, _T("Item"));
	m_listXChartRoiPara.SetColumnWidth(0, 40);
	m_listXChartRoiPara.InsertColumn(1, _T("TopEn"));
	m_listXChartRoiPara.SetColumnWidth(1, 50);
	m_listXChartRoiPara.InsertColumn(2, _T("BottomEn"));
	m_listXChartRoiPara.SetColumnWidth(2, 60);
	m_listXChartRoiPara.InsertColumn(3, _T("LeftEn"));
	m_listXChartRoiPara.SetColumnWidth(3, 50);
	m_listXChartRoiPara.InsertColumn(4, _T("RightEn"));
	m_listXChartRoiPara.SetColumnWidth(4, 50);
	m_listXChartRoiPara.InsertColumn(5, _T("pX"));
	m_listXChartRoiPara.SetColumnWidth(5, 50);
	m_listXChartRoiPara.InsertColumn(6, _T("pY"));
	m_listXChartRoiPara.SetColumnWidth(6, 50);
	m_listXChartRoiPara.InsertColumn(7, _T("Freq."));
	m_listXChartRoiPara.SetColumnWidth(7, 50);
	m_listXChartRoiPara.InsertColumn(8, _T("OffsetH"));
	m_listXChartRoiPara.SetColumnWidth(8, 60);
	m_listXChartRoiPara.InsertColumn(9, _T("OffsetV"));
	m_listXChartRoiPara.SetColumnWidth(9, 60);
	m_listXChartRoiPara.InsertColumn(10, _T("SpecH"));
	m_listXChartRoiPara.SetColumnWidth(10, 50);
	m_listXChartRoiPara.InsertColumn(11, _T("SpecV"));
	m_listXChartRoiPara.SetColumnWidth(11, 50);
	m_listXChartRoiPara.InsertColumn(12, _T("Fov"));
	m_listXChartRoiPara.SetColumnWidth(12, 40);
	m_listXChartRoiPara.InsertColumn(13, _T("Memo"));
	m_listXChartRoiPara.SetColumnWidth(13, 100);

	CString tmpS = _T("");
	for (int i=0; i<m_xChartSfrPara.roiCnt; i++)
	{
		tmpS.Format(_T("%03d"), i);
		m_listXChartRoiPara.InsertItem(i, tmpS);
		if (m_xChartSfrPara.roiPara[i].topEn == TRUE)
			m_listXChartRoiPara.SetItemText(i, 1, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(i, 1, _T("False"));
		if (m_xChartSfrPara.roiPara[i].bottomEn == TRUE)
			m_listXChartRoiPara.SetItemText(i, 2, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(i, 2, _T("False"));
		if (m_xChartSfrPara.roiPara[i].leftEn == TRUE)
			m_listXChartRoiPara.SetItemText(i, 3, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(i, 3, _T("False"));
		if (m_xChartSfrPara.roiPara[i].rightEn == TRUE)
			m_listXChartRoiPara.SetItemText(i, 4, _T("True"));
		else
			m_listXChartRoiPara.SetItemText(i, 4, _T("False"));
		m_listXChartRoiPara.SetItemText(i, 5, m_xChartSfrPara.roiPara[i].cX);
		m_listXChartRoiPara.SetItemText(i, 6, m_xChartSfrPara.roiPara[i].cY);
		m_listXChartRoiPara.SetItemText(i, 7, m_xChartSfrPara.roiPara[i].freq);
		m_listXChartRoiPara.SetItemText(i, 8, m_xChartSfrPara.roiPara[i].sfrOffsetH);
		m_listXChartRoiPara.SetItemText(i, 9, m_xChartSfrPara.roiPara[i].sfrOffsetV);
		m_listXChartRoiPara.SetItemText(i, 10, m_xChartSfrPara.roiPara[i].limitH);
		m_listXChartRoiPara.SetItemText(i, 11, m_xChartSfrPara.roiPara[i].limitV);
		m_listXChartRoiPara.SetItemText(i, 12, m_xChartSfrPara.roiPara[i].fov);
		m_listXChartRoiPara.SetItemText(i, 13, m_xChartSfrPara.roiPara[i].memo);
	}
	m_listXChartRoiPara.SetRedraw(TRUE);
}

void CDlgSfr::Update_Corner_Diff_UI()
{
	CButton* pBtn = (CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_ENABLE));
	if (pBtn == NULL)
		return;
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_USEMINVALUE)))->EnableWindow(FALSE);
		((CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_LU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_RU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_LD))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_RD))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_USEMINVALUE)))->EnableWindow(TRUE);
		((CButton*)(GetDlgItem(IDC_CHECK_XCHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_H))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_SPEC_V))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_OFFSET_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_LU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_RU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_LD))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_XCHAR_CORNER_DIFF_RD))->EnableWindow(TRUE);
	}
}