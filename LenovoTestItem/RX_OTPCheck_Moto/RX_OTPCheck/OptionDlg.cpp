// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RX_OTPCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "RX_OTPCheck_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, RX_OTPCheck_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_uErrorCode = 0;
	m_BufferSize = _T("2000");
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ProjectName, m_ProjectName);

	DDX_Text(pDX, IDC_ErrorCode, m_uErrorCode);
	DDX_Radio(pDX, IDC_RADIO1, m_bModuleSelection);
	DDX_Text(pDX, IDC_BufferSize, m_BufferSize);
	DDV_MaxChars(pDX, m_BufferSize, 4);

	DDX_Radio(pDX, IDC_AF, m_AFStation);
// 	DDX_Radio(pDX, IDC_OTP, m_OTPStation);
// 	DDX_Radio(pDX, IDC_PDAF, m_PDAFStation);
// 	DDX_Radio(pDX,IDC_AEC,m_AECStation);
// 	DDX_Radio(pDX, IDC_Cal, m_CalStation);
// 	DDX_Radio(pDX, IDC_FinalTest, m_FinalTestStation);
	
	DDX_Check(pDX, IDC_SaveEEPROMPDAF,SaveEEPROMPDAF_En);
    

}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_LoadSetting, &OptionDlg::OnBnClickedLoadsetting)

    ON_BN_CLICKED(IDC_CHECK1, &OptionDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_SaveEEPROMPDAF, &OptionDlg::OnBnClickedSaveeeprompdaf)
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


void OptionDlg::OnBnClickedLoadsetting()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
	{
		m_bModuleSelection = FALSE;
	}
	else
	{
		m_bModuleSelection = TRUE;
	}
	UpdateData();
	m_pTestItem->LoadOption();
}


void OptionDlg::OnBnClickedCheck1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}


void OptionDlg::OnBnClickedSaveeeprompdaf()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}
