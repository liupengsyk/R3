// DlgSquareChartSfr.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "DlgSquareChartSfr.h"
#include "afxdialogex.h"


// CDlgSquareChartSfr 对话框

IMPLEMENT_DYNAMIC(CDlgSquareChartSfr, CDialogEx)

CDlgSquareChartSfr::CDlgSquareChartSfr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSquareChartSfr::IDD, pParent)
{
	m_squareChartSfrPara.imgWidth = _T("0");
	m_squareChartSfrPara.imgHeight = _T("0");
	m_squareChartSfrPara.roiWidth = _T("64");
	m_squareChartSfrPara.roiHeight = _T("64");
	m_squareChartSfrPara.roiMaskWidth = _T("250");
	m_squareChartSfrPara.roiMaskHeight = _T("250");
	m_squareChartSfrPara.roiMaskPhi = _T("-6");

	m_squareChartSfrPara.cornerDiffPara.enable = FALSE;
	m_squareChartSfrPara.cornerDiffPara.diffAbsolute = TRUE;
	m_squareChartSfrPara.cornerDiffPara.spec = _T("1.00");
	m_squareChartSfrPara.cornerDiffPara.specH = _T("1.00");
	m_squareChartSfrPara.cornerDiffPara.specV = _T("1.00");
	m_squareChartSfrPara.cornerDiffPara.diffOffset = _T("0.00");
	m_squareChartSfrPara.cornerDiffPara.diffOffsetH = _T("0.00");
	m_squareChartSfrPara.cornerDiffPara.diffOffsetV = _T("0.00");
	m_squareChartSfrPara.cornerDiffPara.roiNumLU = _T("0");
	m_squareChartSfrPara.cornerDiffPara.roiNumRU = _T("0");
	m_squareChartSfrPara.cornerDiffPara.roiNumLD = _T("0");
	m_squareChartSfrPara.cornerDiffPara.roiNumRD = _T("0");

	m_squareChartSfrPara.roiPara[0].enable = TRUE;
	m_squareChartSfrPara.roiPara[0].cX = _T("1296");
	m_squareChartSfrPara.roiPara[0].cY = _T("972");
	m_squareChartSfrPara.roiPara[0].freq = _T("0.125");
	m_squareChartSfrPara.roiPara[0].sfrOffsetH = _T("0");
	m_squareChartSfrPara.roiPara[0].sfrOffsetV = _T("0");
	m_squareChartSfrPara.roiPara[0].limitH = _T("0.2");
	m_squareChartSfrPara.roiPara[0].limitV = _T("0.2");
	m_squareChartSfrPara.roiPara[0].fov = _T("");

	m_addParaByFov = _T("None");
	m_roiEnable = TRUE;
	m_pX = _T("0");
	m_pY = _T("0");
	m_freq = _T("0.125");
	m_sfrOffsetH = _T("0.0000");
	m_sfrOffsetV = _T("0.0000");
	m_specH = _T("0.0000");
	m_specV = _T("0.0000");

	m_selectedItemNum = -1;
}

CDlgSquareChartSfr::~CDlgSquareChartSfr()
{
}

void CDlgSquareChartSfr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SQUARE_CHART_ROI_PARA, m_listSquareChartRoiPara);

	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_IMG_WIDTH, m_squareChartSfrPara.imgWidth);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_IMG_HEIGHT, m_squareChartSfrPara.imgHeight);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_WIDTH, m_squareChartSfrPara.roiWidth);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_HEIGHT, m_squareChartSfrPara.roiHeight);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_WIDTH, m_squareChartSfrPara.roiMaskWidth);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_HEIGHT, m_squareChartSfrPara.roiMaskHeight);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_PHI, m_squareChartSfrPara.roiMaskPhi);

	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_BY_FOV, m_addParaByFov);
	DDX_Check(pDX, IDC_CHECK_SQUARE_CHAR_SFR_ROI_ENABLE, m_roiEnable);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_PX, m_pX);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_PY, m_pY);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_FREQ, m_freq);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_H, m_sfrOffsetH);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_V, m_sfrOffsetV);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_H, m_specH);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_V, m_specV);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_SFR_ROI_MEMO, m_memo);

	DDX_Check(pDX, IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ENABLE, m_squareChartSfrPara.cornerDiffPara.enable);
	DDX_Check(pDX, IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ABSOLUTE, m_squareChartSfrPara.cornerDiffPara.diffAbsolute);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC, m_squareChartSfrPara.cornerDiffPara.spec);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_H, m_squareChartSfrPara.cornerDiffPara.specH);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_V, m_squareChartSfrPara.cornerDiffPara.specV);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET, m_squareChartSfrPara.cornerDiffPara.diffOffset);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_H, m_squareChartSfrPara.cornerDiffPara.diffOffsetH);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_V, m_squareChartSfrPara.cornerDiffPara.diffOffsetV);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LU, m_squareChartSfrPara.cornerDiffPara.roiNumLU);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RU, m_squareChartSfrPara.cornerDiffPara.roiNumRU);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LD, m_squareChartSfrPara.cornerDiffPara.roiNumLD);
	DDX_Text(pDX, IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RD, m_squareChartSfrPara.cornerDiffPara.roiNumRD);
}


BEGIN_MESSAGE_MAP(CDlgSquareChartSfr, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SQUARE_CHAR_SFR_ADD_ROI_PARA, &CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrAddRoiPara)
	ON_BN_CLICKED(IDOK, &CDlgSquareChartSfr::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SQUARE_CHART_ROI_PARA, &CDlgSquareChartSfr::OnNMClickListSquareChartRoiPara)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SQUARE_CHART_ROI_PARA, &CDlgSquareChartSfr::OnNMDblclkListSquareChartRoiPara)
	ON_BN_CLICKED(IDC_BUTTON_SQUARE_CHAR_SFR_DEL_ALL_ROI_PARA, &CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrDelAllRoiPara)
	ON_BN_CLICKED(IDC_BUTTON_SQUARE_CHAR_SFR_DEL_ROI_PARA, &CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrDelRoiPara)
	ON_BN_CLICKED(IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ENABLE, &CDlgSquareChartSfr::OnBnClickedCheckSquareCharCornerDiffEnable)
	ON_BN_CLICKED(IDC_BUTTON_SQUARE_CHAR_SFR_UPDATE_ROI_PARA, &CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrUpdateRoiPara)
END_MESSAGE_MAP()


// CDlgSquareChartSfr 消息处理程序


BOOL CDlgSquareChartSfr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateListControl();

	((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MEMO))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_WIDTH))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_HEIGHT))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MASK_PHI))->EnableWindow(FALSE);

	Update_Corner_Diff_UI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSquareChartSfr::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PreSubclassWindow();
}


void CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrAddRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if ((m_pX == "") || (m_pY == ""))
	{
		AfxMessageBox(_T("Please input \"pX\" and \"pY\" !"));
		return;
	}
	
	int px = _ttoi(m_pX), py = _ttoi(m_pY), w = _ttoi(m_squareChartSfrPara.imgWidth), h = _ttoi(m_squareChartSfrPara.imgHeight);
	if ((px < 0) || (px > w) || (py < 0) || (py > h))
	{
		AfxMessageBox(_T("\"pX\" or \"pY\" or \"imgWidth\" or \"imgHeight\" failed !"));
		return;
	}

	if (m_addParaByFov == "None")
	{
		CString tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listSquareChartRoiPara.GetItemCount());
		m_listSquareChartRoiPara.InsertItem(m_listSquareChartRoiPara.GetItemCount(), tmpS);
		if (m_roiEnable == TRUE)
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("False"));
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 2, m_pX);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 3, m_pY);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 4, m_freq);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 5, m_sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 6, m_sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 7, m_specH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 8, m_specV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 9, _T(""));
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 10, m_memo);
		m_listSquareChartRoiPara.SetRedraw(TRUE);
	}
	else
	{
		float fov = _ttoi(m_addParaByFov) * 1.0f;
		px = (int)((w / 2.0f) - (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) - (h * fov / 200.0f) + 0.5f);
		CString tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listSquareChartRoiPara.GetItemCount());
		m_listSquareChartRoiPara.InsertItem(m_listSquareChartRoiPara.GetItemCount(), tmpS);
		if (m_roiEnable == TRUE)
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("False"));
		CString t = _T("");
		t.Format(_T("%d"), px);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 2, t);
		t.Format(_T("%d"), py);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 3, t);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 4, m_freq);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 5, m_sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 6, m_sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 7, m_specH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 8, m_specV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 9, m_addParaByFov);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 10, m_memo);

		px = (int)((w / 2.0f) + (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) - (h * fov / 200.0f) + 0.5f);
		tmpS = _T("");
		tmpS.Format(_T("%03d"), m_listSquareChartRoiPara.GetItemCount());
		m_listSquareChartRoiPara.InsertItem(m_listSquareChartRoiPara.GetItemCount(), tmpS);
		if (m_roiEnable == TRUE)
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 2, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 3, tmpS);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 4, m_freq);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 5, m_sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 6, m_sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 7, m_specH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 8, m_specV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 9, m_addParaByFov);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 10, m_memo);

		px = (int)((w / 2.0f) - (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) + (h * fov / 200.0f) + 0.5f);
		tmpS.Format(_T("%03d"), m_listSquareChartRoiPara.GetItemCount());
		m_listSquareChartRoiPara.InsertItem(m_listSquareChartRoiPara.GetItemCount(), tmpS);
		if (m_roiEnable == TRUE)
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 2, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 3, tmpS);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 4, m_freq);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 5, m_sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 6, m_sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 7, m_specH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 8, m_specV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 9, m_addParaByFov);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 10, m_memo);

		px = (int)((w / 2.0f) + (w * fov / 200.0f) + 0.5f);
		py = (int)((h / 2.0f) + (h * fov / 200.0f) + 0.5f);
		tmpS.Format(_T("%03d"), m_listSquareChartRoiPara.GetItemCount());
		m_listSquareChartRoiPara.InsertItem(m_listSquareChartRoiPara.GetItemCount(), tmpS);
		if (m_roiEnable == TRUE)
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 1, _T("False"));
		tmpS.Format(_T("%d"), px);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 2, tmpS);
		tmpS.Format(_T("%d"), py);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 3, tmpS);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 4, m_freq);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 5, m_sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 6, m_sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 7, m_specH);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 8, m_specV);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 9, m_addParaByFov);
		m_listSquareChartRoiPara.SetItemText(m_listSquareChartRoiPara.GetItemCount()-1, 10, m_memo);

		m_addParaByFov = _T("None");
		UpdateData(FALSE);
	}
}


void CDlgSquareChartSfr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_squareChartSfrPara.roiCnt = m_listSquareChartRoiPara.GetItemCount();
	for (int i=0; i<m_squareChartSfrPara.roiCnt; i++)
	{
		m_squareChartSfrPara.roiPara[i].enable = (m_listSquareChartRoiPara.GetItemText(i, 1).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_squareChartSfrPara.roiPara[i].cX = m_listSquareChartRoiPara.GetItemText(i, 2);
		m_squareChartSfrPara.roiPara[i].cY = m_listSquareChartRoiPara.GetItemText(i, 3);
		m_squareChartSfrPara.roiPara[i].freq = m_listSquareChartRoiPara.GetItemText(i, 4);
		m_squareChartSfrPara.roiPara[i].sfrOffsetH = m_listSquareChartRoiPara.GetItemText(i, 5);
		m_squareChartSfrPara.roiPara[i].sfrOffsetV = m_listSquareChartRoiPara.GetItemText(i, 6);
		m_squareChartSfrPara.roiPara[i].limitH = m_listSquareChartRoiPara.GetItemText(i, 7);
		m_squareChartSfrPara.roiPara[i].limitV = m_listSquareChartRoiPara.GetItemText(i, 8);
		m_squareChartSfrPara.roiPara[i].fov = m_listSquareChartRoiPara.GetItemText(i, 9);
		m_squareChartSfrPara.roiPara[i].memo = m_listSquareChartRoiPara.GetItemText(i, 10);
	}

	CDialogEx::OnOK();
}


void CDlgSquareChartSfr::OnNMClickListSquareChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString tmpStr = _T("");
	m_selectedItemNum = pNMItemActivate->iItem;
	if (m_selectedItemNum != -1)
	{
		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 1);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_SQUARE_CHAR_SFR_ROI_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_SQUARE_CHAR_SFR_ROI_ENABLE))->SetCheck(FALSE);

		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 2);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_PX)->SetWindowTextW(tmpStr);
		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 3);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_PY)->SetWindowTextW(tmpStr);
		
		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 4);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_FREQ)->SetWindowTextW(tmpStr);

		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 5);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_H)->SetWindowTextW(tmpStr);
		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 6);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_V)->SetWindowTextW(tmpStr);

		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 7);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_H)->SetWindowTextW(tmpStr);
		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 8);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_V)->SetWindowTextW(tmpStr);

		tmpStr = m_listSquareChartRoiPara.GetItemText(m_selectedItemNum, 10);
		GetDlgItem(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MEMO)->SetWindowTextW(tmpStr);
	}
}


void CDlgSquareChartSfr::OnNMDblclkListSquareChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgSquareChartSfr::UpdateListControl(void)
{
	m_listSquareChartRoiPara.InsertColumn(0, _T("Item"));
	m_listSquareChartRoiPara.SetColumnWidth(0, 40);
	m_listSquareChartRoiPara.InsertColumn(1, _T("Enable"));
	m_listSquareChartRoiPara.SetColumnWidth(1, 50);
	m_listSquareChartRoiPara.InsertColumn(2, _T("pX"));
	m_listSquareChartRoiPara.SetColumnWidth(2, 50);
	m_listSquareChartRoiPara.InsertColumn(3, _T("pY"));
	m_listSquareChartRoiPara.SetColumnWidth(3, 50);
	m_listSquareChartRoiPara.InsertColumn(4, _T("Freq."));
	m_listSquareChartRoiPara.SetColumnWidth(4, 50);
	m_listSquareChartRoiPara.InsertColumn(5, _T("OffsetH"));
	m_listSquareChartRoiPara.SetColumnWidth(5, 60);
	m_listSquareChartRoiPara.InsertColumn(6, _T("OffsetV"));
	m_listSquareChartRoiPara.SetColumnWidth(6, 60);
	m_listSquareChartRoiPara.InsertColumn(7, _T("SpecH"));
	m_listSquareChartRoiPara.SetColumnWidth(7, 50);
	m_listSquareChartRoiPara.InsertColumn(8, _T("SpecV"));
	m_listSquareChartRoiPara.SetColumnWidth(8, 50);
	m_listSquareChartRoiPara.InsertColumn(9, _T("Fov"));
	m_listSquareChartRoiPara.SetColumnWidth(9, 40);
	m_listSquareChartRoiPara.InsertColumn(10, _T("Memo"));
	m_listSquareChartRoiPara.SetColumnWidth(10, 100);

	CString tmpS = _T("");
	for (int i=0; i<m_squareChartSfrPara.roiCnt; i++)
	{
		tmpS.Format(_T("%03d"), i);
		m_listSquareChartRoiPara.InsertItem(i, tmpS);
		if (m_squareChartSfrPara.roiPara[i].enable == TRUE)
			m_listSquareChartRoiPara.SetItemText(i, 1, _T("True"));
		else
			m_listSquareChartRoiPara.SetItemText(i, 1, _T("False"));
		m_listSquareChartRoiPara.SetItemText(i, 2, m_squareChartSfrPara.roiPara[i].cX);
		m_listSquareChartRoiPara.SetItemText(i, 3, m_squareChartSfrPara.roiPara[i].cY);
		m_listSquareChartRoiPara.SetItemText(i, 4, m_squareChartSfrPara.roiPara[i].freq);
		m_listSquareChartRoiPara.SetItemText(i, 5, m_squareChartSfrPara.roiPara[i].sfrOffsetH);
		m_listSquareChartRoiPara.SetItemText(i, 6, m_squareChartSfrPara.roiPara[i].sfrOffsetV);
		m_listSquareChartRoiPara.SetItemText(i, 7, m_squareChartSfrPara.roiPara[i].limitH);
		m_listSquareChartRoiPara.SetItemText(i, 8, m_squareChartSfrPara.roiPara[i].limitV);
		m_listSquareChartRoiPara.SetItemText(i, 9, m_squareChartSfrPara.roiPara[i].fov);
		m_listSquareChartRoiPara.SetItemText(i, 10, m_squareChartSfrPara.roiPara[i].memo);
	}
	m_listSquareChartRoiPara.SetRedraw(TRUE);
}

void CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrDelAllRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	if (AfxMessageBox(_T("Delete all ROI parameter settings ?"), 1) == IDOK)
	{
		for (int i=0; i<64; i++)
		{
			m_squareChartSfrPara.roiPara[i].enable = FALSE;
			m_squareChartSfrPara.roiPara[i].cX = _T("0");
			m_squareChartSfrPara.roiPara[i].cY = _T("0");
			m_squareChartSfrPara.roiPara[i].freq = _T("0.125");
			m_squareChartSfrPara.roiPara[i].sfrOffsetH = _T("0.0000");
			m_squareChartSfrPara.roiPara[i].sfrOffsetV = _T("0.0000");
			m_squareChartSfrPara.roiPara[i].limitH = _T("0.0000");
			m_squareChartSfrPara.roiPara[i].limitV = _T("0.0000");
			m_squareChartSfrPara.roiPara[i].fov = _T("");
			m_squareChartSfrPara.roiPara[i].memo = _T("");
		}
		m_listSquareChartRoiPara.DeleteAllItems();
	}
	m_listSquareChartRoiPara.SetRedraw(TRUE);
}


void CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrDelRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	int markItem = m_listSquareChartRoiPara.GetSelectionMark();
	if (markItem == -1)
		AfxMessageBox(_T("Please select a ROI parameter item .[-1]"));
	else
	{
		CString strT = _T("");
		strT.Format(_T("Delete \"%03d\" ROI parameter settings ?"), markItem);
		if (AfxMessageBox(strT, 1) == IDOK)
		{
			m_listSquareChartRoiPara.DeleteItem(markItem);
			for (int i=markItem; i<m_listSquareChartRoiPara.GetItemCount(); i++)
			{
				strT.Format(_T("%03d"), i);
				m_listSquareChartRoiPara.SetItemText(i, 0, strT);
			}
		}
	}
	m_listSquareChartRoiPara.SetRedraw(TRUE);
}


void CDlgSquareChartSfr::OnBnClickedCheckSquareCharCornerDiffEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	Update_Corner_Diff_UI();
}

void CDlgSquareChartSfr::Update_Corner_Diff_UI()
{
	CButton* pBtn = (CButton*)(GetDlgItem(IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ENABLE));
	if (pBtn == NULL)
		return;
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CButton*)(GetDlgItem(IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LD))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RD))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)(GetDlgItem(IDC_CHECK_SQUARE_CHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_H))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_SPEC_V))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_H))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_OFFSET_V))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_LD))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_SQUARE_CHAR_CORNER_DIFF_RD))->EnableWindow(TRUE);
	}
}


void CDlgSquareChartSfr::OnBnClickedButtonSquareCharSfrUpdateRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp = _T("");
	if (m_selectedItemNum != -1)
	{
		strTmp.Format(_T("Update ROI: %03d parameter ?"), m_selectedItemNum);
		if (AfxMessageBox(strTmp, 1) == IDOK)
		{
			if (((CButton*)GetDlgItem(IDC_CHECK_SQUARE_CHAR_SFR_ROI_ENABLE))->GetCheck() == TRUE)
			{
				m_squareChartSfrPara.roiPara[m_selectedItemNum].enable = TRUE;
				m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("True"));
			}
			else
			{
				m_squareChartSfrPara.roiPara[m_selectedItemNum].enable = FALSE;
				m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("False"));
			}

			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_PX, m_squareChartSfrPara.roiPara[m_selectedItemNum].cX);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_PY, m_squareChartSfrPara.roiPara[m_selectedItemNum].cY);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_FREQ, m_squareChartSfrPara.roiPara[m_selectedItemNum].freq);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_H, m_squareChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetH);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_OFFSET_V, m_squareChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetV);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_H, m_squareChartSfrPara.roiPara[m_selectedItemNum].limitH);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_SPEC_V, m_squareChartSfrPara.roiPara[m_selectedItemNum].limitV);
			GetDlgItemTextW(IDC_EDIT_SQUARE_CHAR_SFR_ROI_MEMO, m_squareChartSfrPara.roiPara[m_selectedItemNum].memo);
			
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 2, m_squareChartSfrPara.roiPara[m_selectedItemNum].cX);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 3, m_squareChartSfrPara.roiPara[m_selectedItemNum].cY);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 4, m_squareChartSfrPara.roiPara[m_selectedItemNum].freq);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 5, m_squareChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetH);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 6, m_squareChartSfrPara.roiPara[m_selectedItemNum].sfrOffsetV);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 7, m_squareChartSfrPara.roiPara[m_selectedItemNum].limitH);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 8, m_squareChartSfrPara.roiPara[m_selectedItemNum].limitV);
			m_listSquareChartRoiPara.SetItemText(m_selectedItemNum, 10, m_squareChartSfrPara.roiPara[m_selectedItemNum].memo);
		}
	}
}
