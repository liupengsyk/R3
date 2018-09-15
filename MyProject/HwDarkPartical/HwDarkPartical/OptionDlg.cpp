// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HwDarkPartical.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "HwDarkPartical_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, HwDarkPartical_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_QuantitySpec = 0.0f;
	m_DnDiff = 0;
	m_AnalogGain = _T("");
	m_InitExposure = _T("");
	m_SensorType = 0;
	m_FrameDelay = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_SaveImageRaw10, m_SaveImgRaw10);
	DDX_Text(pDX, IDC_QuantitySpec, m_QuantitySpec);
	DDX_Text(pDX, IDC_DN_Diff, m_DnDiff);
	DDX_Check(pDX, IDC_SaveDarkParticalInfo, m_SaveDarkParticalInfo);
	DDX_Text(pDX, IDC_AnalogGain, m_AnalogGain);
	DDX_Text(pDX, IDC_InitExposure, m_InitExposure);
	DDX_Radio(pDX, IDC_RADIO1, m_SensorType);
	DDX_Text(pDX, IDC_FrameDelay, m_FrameDelay);
	DDX_Check(pDX, IDC_CheckGain, m_CheckGain);
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

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}