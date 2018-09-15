// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AE.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AE_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AE_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_HighLimit = 0;
	m_LowLimit = 0;
	m_Ratio = 0;
	m_BLC = 0;
	m_excount = 0;
	m_SlaveID = _T("0x20");
	m_InitExposure = 0;
	ErrorCode = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_HighLimit, m_HighLimit);
	DDX_Text(pDX, IDC_LowLimit, m_LowLimit);
	DDX_Text(pDX, IDC_RATIO, m_Ratio);
	DDX_Text(pDX, IDC_RATIO5, m_Ratio2);
	DDX_Text(pDX, IDC_RATIO2, m_BLC);
	DDX_Check(pDX, IDC_CHECK1, m_WhiteEnable);
	DDX_Check(pDX,IDC_CHECK2,m_White128Enable);
	DDX_Check(pDX,IDC_CHECK5,m_10bit);
	DDX_Check(pDX,IDC_CHECK6,m_is4Cell);
	DDX_Check(pDX,IDC_CHECK_2PD,m_is2PD);
	DDX_Check(pDX, IDC_CHECK3, m_RemExp);
	DDX_Check(pDX,IDC_CHECK4,m_DNPExp);
	DDX_Text(pDX, IDC_RATIO3, m_excount);
	DDX_Text(pDX, IDC_SlaveID, m_SlaveID);
	DDX_Text(pDX, IDC_COMBO_AE_TARGET, m_aeTargetString);
	DDX_Text(pDX, IDC_InitExposure, m_InitExposure);
	DDX_Text(pDX, IDC_ERROE_CODE, ErrorCode);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_pTestItem->intervalSpec);
	DDX_Text(pDX, IDC_ADDRESSH, m_AddressH);
	DDX_Text(pDX, IDC_ADDRESSL, m_AddressL);
	DDX_Text(pDX,IDC_GainReg1,m_GainReg1);
	DDX_Text(pDX,IDC_GainReg2,m_GainReg2);
	DDX_Text(pDX,IDC_GainVal1,m_GainVal1);
	DDX_Text(pDX,IDC_GainVal2,m_GainVal2);
	DDX_Text(pDX, IDC_Mode, m_mode);
	DDX_Text(pDX,IDC_ModeGain,m_mode_Gain);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDX_Text(pDX,IDC_STARTX,m_StartX);
	DDX_Text(pDX,IDC_STARTY,m_StartY);
	
	DDX_Radio(pDX, IDC_A_Channel, LEDChannel);
	DDX_CBIndex(pDX, IDC_COMBO4, m_LEDType);
	DDX_Text(pDX, IDC_EDIT4, ALight_Channel);
	DDX_Text(pDX, IDC_EDIT5, TL84LightChannel);
	DDX_Text(pDX, IDC_EDIT6, D65LightChannel);
	DDX_Text(pDX, IDC_EDIT2, m_WaitFlag);
	DDX_Check(pDX,IDC_CHECK8,SetLedEN);
	
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

	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("G_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("R_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("B_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("Gr_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("Gb_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->AddString(_T("Y_Target"));
	((CComboBox*)GetDlgItem(IDC_COMBO_AE_TARGET))->SetCurSel(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

