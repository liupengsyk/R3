// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SRNTest.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "SRNTest_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, SRNTest_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;

	m_srnPara.errCode = _T("16");
	m_srnPara.errInfo = _T("Test Failed");

	m_srnPara.specPPmax = _T("30");
	m_srnPara.frmDelayCnt = _T("5");

	m_srnPara.analogGain = _T("0x00");
	m_srnPara.exposureTime = _T("0x00");
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SRN_ERROR_CODE, m_srnPara.errCode);
	DDX_Text(pDX, IDC_EDIT_SRN_ERROR_INFO, m_srnPara.errInfo);
	DDX_Text(pDX, IDC_EDIT_SRN_SPEC_PPMAX, m_srnPara.specPPmax);
	DDX_Text(pDX, IDC_EDIT_SRN_FRAME_DELAY_COUNT, m_srnPara.frmDelayCnt);
	DDX_Text(pDX, IDC_EDIT_SRN_ANALOG_GAIN_VALUE, m_srnPara.analogGain);
	DDX_Text(pDX, IDC_EDIT_SRN_EXPOSURE_TIME_VALUE, m_srnPara.exposureTime);
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