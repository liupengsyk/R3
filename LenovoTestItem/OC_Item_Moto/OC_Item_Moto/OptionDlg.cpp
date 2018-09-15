// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OC_Item_Moto.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OC_Item_Moto_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OC_Item_Moto_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_Continue(FALSE)
{
	m_pTestItem = pTestItem;

	/** Mono Sensor OC 配置参数 **/
	m_OC_MonoSpec = 0.0f;

	/** Mono Sensor OC 配置参数 **/
	m_OC_BayerSpec_R = 0.0f;
	m_OC_BayerSpec_Gr = 0.0f;
	m_OC_BayerSpec_Gb = 0.0f;
	m_OC_BayerSpec_B = 0.0f;

	/** Mono Sensor OC 配置参数 **/
	m_OC_BayerSpec_Y = 0.0f;

	m_OCSpec_AfterShift = 0.0f;
	m_PixelSize = 0.0f;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_Continue);
	DDX_Check(pDX, IDC_CHECK_OCShift, m_OCSpec_Shift);

	/** Mono Sensor OC 配置参数 **/
	DDX_Text(pDX, IDC_EDIT_OC_MonoSpec, m_OC_MonoSpec);
	DDX_Text(pDX, IDC_EDIT_OCSpec_AfterShift, m_OCSpec_AfterShift);

	/** Mono Sensor OC 配置参数 **/
	DDX_Text(pDX, IDC_EDIT_OC_BayerSpec_R, m_OC_BayerSpec_R);
	DDX_Text(pDX, IDC_EDIT_OC_BayerSpec_Gr, m_OC_BayerSpec_Gr);
	DDX_Text(pDX, IDC_EDIT_OC_BayerSpec_Gb, m_OC_BayerSpec_Gb);
	DDX_Text(pDX, IDC_EDIT_OC_BayerSpec_B, m_OC_BayerSpec_B);

	/** Mono Sensor OC 配置参数 **/
	DDX_Text(pDX, IDC_EDIT_OC_BayerSpec_Y, m_OC_BayerSpec_Y);

	DDX_Text(pDX, IDC_EDIT_PixelSize, m_PixelSize);
	DDX_Radio(pDX, IDC_RADIO_Bayer, m_isMonoSensor);

	DDX_Radio(pDX, IDC_RADIO_Bayer_Y, m_isBayerSingleChannel);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_RADIO_Bayer, &OptionDlg::OnBnClickedRadioBayer)
	ON_BN_CLICKED(IDC_RADIO_Mono, &OptionDlg::OnBnClickedRadioMono)
	ON_BN_CLICKED(IDC_RADIO_Bayer_Y, &OptionDlg::OnBnClickedRadioBayerY)
	ON_BN_CLICKED(IDC_RADIO_Channel, &OptionDlg::OnBnClickedRadioChannel)
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
		m_OC_MonoSpec = m_pTestItem->ReadConfigInt(_T("OCSpecX_hig"),100);
	//	m_OCSpecY_hig=m_pTestItem->ReadConfigInt(_T("OCSpecY_hig"),100);
		m_Continue = m_pTestItem->ReadConfigInt(_T("m_Continue"),0);
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

void OptionDlg::OCModeSel(int OCMode)
{
	switch (OCMode)
	{
	case OC_MODE_BAYER:
		{
			OCMonoModeEnable(FALSE);
			OCBayerModeEnable(TRUE);
		}
		break;

	case OC_MODE_MONO:
		{
			OCMonoModeEnable(TRUE);
			OCBayerModeEnable(FALSE);
		}
		break;

	default:
		break;
	}
}

void OptionDlg::OCBayerModeSel(int OCBayerMode)
{
	switch (OCBayerMode)
	{
	case OC_BAYER_MODE_SINGLE_CHANNEL:
		{
			OCBayerModeSingleChannelEnable(TRUE);
			OCBayerModeYChannelEnable(FALSE);
		}
		break;

	case OC_BAYER_MODE_Y:
		{
			OCBayerModeYChannelEnable(TRUE);
			OCBayerModeSingleChannelEnable(FALSE);
		}
		break;

	default:
		break;
	}
}

void OptionDlg::OCMonoModeEnable(BOOL bEnable)
{
	GetDlgItem(IDC_OC_MonoGrp)->ShowWindow(bEnable);
	GetDlgItem(IDC_OC_Mono_Spec)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_MonoSpec)->ShowWindow(bEnable);
}

void OptionDlg::OCBayerModeEnable(BOOL bEnable)
{
	GetDlgItem(IDC_OC_BayerGrp)->ShowWindow(bEnable);
	GetDlgItem(IDC_RADIO_Bayer_Y)->ShowWindow(bEnable);
	GetDlgItem(IDC_RADIO_Channel)->ShowWindow(bEnable);

	if (TRUE == bEnable)
	{
		OCBayerModeSel(m_isBayerSingleChannel);
	}
	else
	{
		OCBayerModeYChannelEnable(bEnable);
		OCBayerModeSingleChannelEnable(bEnable);
	}
}

void OptionDlg::OCBayerModeSingleChannelEnable(BOOL bEnable)
{
	/** 单通道控件使能 **/
	GetDlgItem(IDC_OC_BayerSpec_R)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_BayerSpec_R)->ShowWindow(bEnable);
	GetDlgItem(IDC_OC_BayerSpec_Gr)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_BayerSpec_Gr)->ShowWindow(bEnable);
	GetDlgItem(IDC_OC_BayerSpec_Gb)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_BayerSpec_Gb)->ShowWindow(bEnable);
	GetDlgItem(IDC_OC_BayerSpec_B)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_BayerSpec_B)->ShowWindow(bEnable);
}

void OptionDlg::OCBayerModeYChannelEnable(BOOL bEnable)
{
	/** Y通道控件使能 **/
	GetDlgItem(IDC_OC_BayerSpec_Y)->ShowWindow(bEnable);
	GetDlgItem(IDC_EDIT_OC_BayerSpec_Y)->ShowWindow(bEnable);
}

void OptionDlg::OnBnClickedRadioBayer()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isMonoSensor = OC_MODE_BAYER;
	OCModeSel(OC_MODE_BAYER);
}


void OptionDlg::OnBnClickedRadioMono()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isMonoSensor = OC_MODE_MONO;
	OCModeSel(OC_MODE_MONO);
}


void OptionDlg::OnBnClickedRadioBayerY()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isBayerSingleChannel = OC_BAYER_MODE_Y;
	OCBayerModeSel(OC_BAYER_MODE_Y);
}


void OptionDlg::OnBnClickedRadioChannel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isBayerSingleChannel = OC_BAYER_MODE_SINGLE_CHANNEL;
	OCBayerModeSel(OC_BAYER_MODE_SINGLE_CHANNEL);
}
