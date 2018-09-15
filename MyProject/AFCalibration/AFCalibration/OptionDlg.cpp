// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AFCalibration.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AFCalibration_TestItem.h"

// OptionDlg 对话框

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AFCalibration_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_strStation(_T(""))
	, m_strModule(_T(""))
	, m_nIntervalTime(0)
    , m_nModuleNum(0)
    , m_strStdPath(_T(""))
    , m_strTestPath(_T(""))
    , m_nStationNum(0)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_STATION, m_strStation);
    DDX_Text(pDX, IDC_EDIT_MODULE, m_strModule);
    DDX_Text(pDX, IDC_EDIT_INTERVAL_HOUR, m_nIntervalTime);
    DDV_MinMaxInt(pDX, m_nIntervalTime, 0, 10000);
    DDX_Text(pDX, IDC_EDIT_MODULE_NUM, m_nModuleNum);
    DDV_MinMaxInt(pDX, m_nModuleNum, 0, 100);
    DDX_Text(pDX, IDC_EDIT_STD_DATA_PATH, m_strStdPath);
    DDX_Text(pDX, IDC_EDIT_TEST_DATA_PATH, m_strTestPath);
    DDX_Control(pDX, IDC_COMBO_STATION_NUM, m_crtlStationNum);
    DDX_CBIndex(pDX, IDC_COMBO_STATION_NUM, m_nStationNum);
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
    m_crtlStationNum.AddString(_T("1"));
    m_crtlStationNum.AddString(_T("2"));
    m_crtlStationNum.AddString(_T("3"));
    m_crtlStationNum.AddString(_T("4"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void OptionDlg::LoadPara()
{	
	//加载设置
    m_strStation    = m_pTestItem->ReadConfigString(_T("Station"), _T("Station1"));
    m_strModule     = m_pTestItem->ReadConfigString(_T("Module"), _T("OGP0757"));
    m_nIntervalTime = m_pTestItem->ReadConfigInt(_T("IntervalTime"), 24);
    m_nModuleNum    = m_pTestItem->ReadConfigInt(_T("ModuleNum"), 3);
    m_nStationNum   = m_pTestItem->ReadConfigInt(_T("StationNum"), 1);
    m_strStdPath    = m_pTestItem->ReadConfigString(_T("StdPath"), _T(""));
    m_strTestPath   = m_pTestItem->ReadConfigString(_T("TestPath"), _T(""));
}

void OptionDlg::SavePara()
{                   
    m_pTestItem->WriteConfigString(_T("Station"), m_strStation);
    m_pTestItem->WriteConfigString(_T("Module"), m_strModule);
    m_pTestItem->WriteConfigInt(_T("IntervalTime"), m_nIntervalTime);
    m_pTestItem->WriteConfigInt(_T("ModuleNum"), m_nModuleNum);
    m_pTestItem->WriteConfigInt(_T("StationNum"), m_nStationNum);
    m_pTestItem->WriteConfigString(_T("StdPath"), m_strStdPath);
    m_pTestItem->WriteConfigString(_T("TestPath"), m_strTestPath);   
}