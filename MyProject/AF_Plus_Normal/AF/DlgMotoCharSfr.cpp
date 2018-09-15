// DlgMotoCharSfr.cpp : 实现文件
//

#include "stdafx.h"
#include "AF.h"
#include "DlgMotoCharSfr.h"
#include "afxdialogex.h"


// CDlgMotoCharSfr 对话框

IMPLEMENT_DYNAMIC(CDlgMotoCharSfr, CDialogEx)

CDlgMotoCharSfr::CDlgMotoCharSfr(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMotoCharSfr::IDD, pParent)
	, m_ndiff_value(0)
{
	m_motoChartSfrPara.roiWidth = _T("96");
	m_motoChartSfrPara.roiHeight = _T("96");
	m_motoChartSfrPara.roiMaskWidth = _T("250");
	m_motoChartSfrPara.roiMaskHeight = _T("250");
	m_motoChartSfrPara.roiMaskOffsetX = _T("40");

	m_motoChartSfrPara.cornerDiffPara.enable = FALSE;
	m_motoChartSfrPara.cornerDiffPara.diffAbsolute = TRUE;
	m_motoChartSfrPara.cornerDiffPara.spec = _T("1.00");
	m_motoChartSfrPara.cornerDiffPara.diffOffset = _T("0.00");
	m_motoChartSfrPara.cornerDiffPara.roiNumLU = _T("0");
	m_motoChartSfrPara.cornerDiffPara.roiNumRU = _T("0");
	m_motoChartSfrPara.cornerDiffPara.roiNumLD = _T("0");
	m_motoChartSfrPara.cornerDiffPara.roiNumRD = _T("0");

	m_motoChartSfrPara.roiCnt = 19;
	m_motoChartSfrPara.roiPara[0].enable = TRUE;
	m_motoChartSfrPara.roiPara[0].freq = _T("0.125");
	m_motoChartSfrPara.roiPara[0].sfrOffset = _T("0");
	m_motoChartSfrPara.roiPara[0].limit = _T("0.2");
	m_motoChartSfrPara.roiPara[0].fov = _T("");

	m_roiEnable = TRUE;
	m_freq = _T("0.125");
	m_sfrOffset = _T("0.0000");
	m_spec = _T("0.0000");
	m_memo = _T("");

	m_selectedItemNum = -1;
}

CDlgMotoCharSfr::~CDlgMotoCharSfr()
{
}

void CDlgMotoCharSfr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MOTO_CHART_ROI_PARA, m_listMotoChartRoiPara);

	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_WIDTH, m_motoChartSfrPara.roiWidth);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_HEIGHT, m_motoChartSfrPara.roiHeight);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_WIDTH, m_motoChartSfrPara.roiMaskWidth);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_HEIGHT, m_motoChartSfrPara.roiMaskHeight);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_OFFSETX, m_motoChartSfrPara.roiMaskOffsetX);

	DDX_Check(pDX, IDC_CHECK_MOTO_CHAR_SFR_ROI_ENABLE, m_roiEnable);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_FREQ, m_freq);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_OFFSET, m_sfrOffset);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_SPEC, m_spec);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_SFR_ROI_MEMO, m_memo);

	DDX_Check(pDX, IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ENABLE, m_motoChartSfrPara.cornerDiffPara.enable);
	DDX_Check(pDX, IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ABSOLUTE, m_motoChartSfrPara.cornerDiffPara.diffAbsolute);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC, m_motoChartSfrPara.cornerDiffPara.spec);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_OFFSET, m_motoChartSfrPara.cornerDiffPara.diffOffset);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LU, m_motoChartSfrPara.cornerDiffPara.roiNumLU);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RU, m_motoChartSfrPara.cornerDiffPara.roiNumRU);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LD, m_motoChartSfrPara.cornerDiffPara.roiNumLD);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RD, m_motoChartSfrPara.cornerDiffPara.roiNumRD);
	DDX_Text(pDX, IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC2, m_ndiff_value);
}


BEGIN_MESSAGE_MAP(CDlgMotoCharSfr, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgMotoCharSfr::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MOTO_CHART_ROI_PARA, &CDlgMotoCharSfr::OnNMClickListMotoChartRoiPara)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MOTO_CHART_ROI_PARA, &CDlgMotoCharSfr::OnNMDblclkListMotoChartRoiPara)
	ON_BN_CLICKED(IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ENABLE, &CDlgMotoCharSfr::OnBnClickedCheckMotoCharCornerDiffEnable)
	ON_BN_CLICKED(IDC_BUTTON_MOTO_CHAR_SFR_UPDATE_ROI_PARA, &CDlgMotoCharSfr::OnBnClickedButtonMotoCharSfrUpdateRoiPara)
END_MESSAGE_MAP()


// CDlgMotoCharSfr 消息处理程序


BOOL CDlgMotoCharSfr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateListControl();

	((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_MEMO))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_WIDTH))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_HEIGHT))->EnableWindow(FALSE);
	//((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_MASK_OFFSETX))->EnableWindow(FALSE);

	Update_Corner_Diff_UI();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMotoCharSfr::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PreSubclassWindow();
}


void CDlgMotoCharSfr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_motoChartSfrPara.roiCnt = m_listMotoChartRoiPara.GetItemCount();
	for (int i=0; i<m_motoChartSfrPara.roiCnt; i++)
	{
		m_motoChartSfrPara.roiPara[i].enable = (m_listMotoChartRoiPara.GetItemText(i, 1).MakeLower() == _T("true")) ? TRUE : FALSE;
		m_motoChartSfrPara.roiPara[i].freq = m_listMotoChartRoiPara.GetItemText(i, 2);
		m_motoChartSfrPara.roiPara[i].sfrOffset = m_listMotoChartRoiPara.GetItemText(i, 3);
		m_motoChartSfrPara.roiPara[i].limit = m_listMotoChartRoiPara.GetItemText(i, 4);
		switch (i)
		{
		case 0:
			m_motoChartSfrPara.roiPara[i].fov = _T("00");
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			m_motoChartSfrPara.roiPara[i].fov = _T("80");
			break;
		case 5:
		case 6:
		case 7:
		case 8:
			m_motoChartSfrPara.roiPara[i].fov = _T("60");
			break;
		case 9:
		case 10:
		case 11:
		case 12:
			m_motoChartSfrPara.roiPara[i].fov = _T("40");
			break;
		}
		m_motoChartSfrPara.roiPara[i].memo = m_listMotoChartRoiPara.GetItemText(i, 6);
	}

	CDialogEx::OnOK();
}


void CDlgMotoCharSfr::OnNMClickListMotoChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString tmpStr = _T("");
	m_selectedItemNum = pNMItemActivate->iItem;
	if (m_selectedItemNum != -1)
	{
		tmpStr = m_listMotoChartRoiPara.GetItemText(m_selectedItemNum, 1);
		if (tmpStr.MakeLower() == "true")
			((CButton*)GetDlgItem(IDC_CHECK_MOTO_CHAR_SFR_ROI_ENABLE))->SetCheck(TRUE);
		else
			((CButton*)GetDlgItem(IDC_CHECK_MOTO_CHAR_SFR_ROI_ENABLE))->SetCheck(FALSE);
		
		tmpStr = m_listMotoChartRoiPara.GetItemText(m_selectedItemNum, 2);
		GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_FREQ)->SetWindowTextW(tmpStr);

		tmpStr = m_listMotoChartRoiPara.GetItemText(m_selectedItemNum, 3);
		GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_OFFSET)->SetWindowTextW(tmpStr);

		tmpStr = m_listMotoChartRoiPara.GetItemText(m_selectedItemNum, 4);
		GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_SPEC)->SetWindowTextW(tmpStr);

		tmpStr = m_listMotoChartRoiPara.GetItemText(m_selectedItemNum, 6);
		GetDlgItem(IDC_EDIT_MOTO_CHAR_SFR_ROI_MEMO)->SetWindowTextW(tmpStr);
	}
}


void CDlgMotoCharSfr::OnNMDblclkListMotoChartRoiPara(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgMotoCharSfr::UpdateListControl(void)
{
	m_listMotoChartRoiPara.InsertColumn(0, _T("Item"));
	m_listMotoChartRoiPara.SetColumnWidth(0, 60);
	m_listMotoChartRoiPara.InsertColumn(1, _T("Enable"));
	m_listMotoChartRoiPara.SetColumnWidth(1, 90);
	m_listMotoChartRoiPara.InsertColumn(2, _T("Freq."));
	m_listMotoChartRoiPara.SetColumnWidth(2, 90);
	m_listMotoChartRoiPara.InsertColumn(3, _T("Offset"));
	m_listMotoChartRoiPara.SetColumnWidth(3, 90);
	m_listMotoChartRoiPara.InsertColumn(4, _T("Spec"));
	m_listMotoChartRoiPara.SetColumnWidth(4, 90);
	m_listMotoChartRoiPara.InsertColumn(5, _T("Fov"));
	m_listMotoChartRoiPara.SetColumnWidth(5, 90);
	m_listMotoChartRoiPara.InsertColumn(6, _T("Memo"));
	m_listMotoChartRoiPara.SetColumnWidth(6, 100);

	CString tmpS = _T("");
	for (int i=0; i<m_motoChartSfrPara.roiCnt; i++)
	{
		tmpS.Format(_T("%03d"), i);
		m_listMotoChartRoiPara.InsertItem(i, tmpS);
		if (m_motoChartSfrPara.roiPara[i].enable == TRUE)
			m_listMotoChartRoiPara.SetItemText(i, 1, _T("True"));
		else
			m_listMotoChartRoiPara.SetItemText(i, 1, _T("False"));
		m_listMotoChartRoiPara.SetItemText(i, 2, m_motoChartSfrPara.roiPara[i].freq);
		m_listMotoChartRoiPara.SetItemText(i, 3, m_motoChartSfrPara.roiPara[i].sfrOffset);
		m_listMotoChartRoiPara.SetItemText(i, 4, m_motoChartSfrPara.roiPara[i].limit);
		m_listMotoChartRoiPara.SetItemText(i, 5, m_motoChartSfrPara.roiPara[i].fov);
		m_listMotoChartRoiPara.SetItemText(i, 6, m_motoChartSfrPara.roiPara[i].memo);
	}
	m_listMotoChartRoiPara.SetRedraw(TRUE);
}


void CDlgMotoCharSfr::OnBnClickedCheckMotoCharCornerDiffEnable()
{
	// TODO: 在此添加控件通知处理程序代码
	Update_Corner_Diff_UI();
}


void CDlgMotoCharSfr::Update_Corner_Diff_UI()
{
	CButton* pBtn = (CButton*)(GetDlgItem(IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ENABLE));
	if (pBtn == NULL)
		return;
	BOOL tmpVal = ((CButton*)(GetDlgItem(IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ENABLE)))->GetCheck();
	if (tmpVal == FALSE)
	{
		((CButton*)(GetDlgItem(IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RU))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LD))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RD))->EnableWindow(FALSE);
	}
	else
	{
		((CButton*)(GetDlgItem(IDC_CHECK_MOTO_CHAR_CORNER_DIFF_ABSOLUTE)))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_SPEC))->EnableWindow(TRUE);
		//((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(FALSE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_OFFSET))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RU))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_LD))->EnableWindow(TRUE);
		((CEdit*)GetDlgItem(IDC_EDIT_MOTO_CHAR_CORNER_DIFF_RD))->EnableWindow(TRUE);
	}
}

void CDlgMotoCharSfr::OnBnClickedButtonMotoCharSfrUpdateRoiPara()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp = _T("");
	if (m_selectedItemNum != -1)
	{
		strTmp.Format(_T("Update ROI: %03d parameter ?"), m_selectedItemNum);
		if (AfxMessageBox(strTmp, 1) == IDOK)
		{
			if (((CButton*)GetDlgItem(IDC_CHECK_MOTO_CHAR_SFR_ROI_ENABLE))->GetCheck() == TRUE)
			{
				m_motoChartSfrPara.roiPara[m_selectedItemNum].enable = TRUE;
				m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("True"));
			}
			else
			{
				m_motoChartSfrPara.roiPara[m_selectedItemNum].enable = FALSE;
				m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 1, _T("False"));
			}

			GetDlgItemTextW(IDC_EDIT_MOTO_CHAR_SFR_ROI_FREQ, m_motoChartSfrPara.roiPara[m_selectedItemNum].freq);
			GetDlgItemTextW(IDC_EDIT_MOTO_CHAR_SFR_ROI_OFFSET, m_motoChartSfrPara.roiPara[m_selectedItemNum].sfrOffset);
			GetDlgItemTextW(IDC_EDIT_MOTO_CHAR_SFR_ROI_SPEC, m_motoChartSfrPara.roiPara[m_selectedItemNum].limit);
			GetDlgItemTextW(IDC_EDIT_MOTO_CHAR_SFR_ROI_MEMO, m_motoChartSfrPara.roiPara[m_selectedItemNum].memo);
			
			m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 2, m_motoChartSfrPara.roiPara[m_selectedItemNum].freq);
			m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 3, m_motoChartSfrPara.roiPara[m_selectedItemNum].sfrOffset);
			m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 4, m_motoChartSfrPara.roiPara[m_selectedItemNum].limit);
			m_listMotoChartRoiPara.SetItemText(m_selectedItemNum, 6, m_motoChartSfrPara.roiPara[m_selectedItemNum].memo);
		}
	}
}
