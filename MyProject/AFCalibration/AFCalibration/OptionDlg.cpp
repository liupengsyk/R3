// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AFCalibration.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AFCalibration_TestItem.h"

// OptionDlg �Ի���

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


// OptionDlg ��Ϣ�������
void OptionDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this;

	CDialog::PostNcDestroy();
}

//�Ի��򴰿����ݸ��µ���Ӧ����
LRESULT OptionDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	//TURE �ؼ���ֵ��>����    
	//FALSE ������>�ؼ���ֵ
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

//��������ctrl+���ּ�����Ӧ��Ϣ��ͨ������Ϣ�������ָ�
LRESULT OptionDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

//��Ӧģ��ر���Ϣ
LRESULT OptionDlg::OnCameraStop(WPARAM wParam, LPARAM lParam)
{
	return 1;
}

BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_crtlStationNum.AddString(_T("1"));
    m_crtlStationNum.AddString(_T("2"));
    m_crtlStationNum.AddString(_T("3"));
    m_crtlStationNum.AddString(_T("4"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void OptionDlg::LoadPara()
{	
	//��������
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