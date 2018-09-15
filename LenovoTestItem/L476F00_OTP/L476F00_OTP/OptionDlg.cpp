// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "L476F00_OTP.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "L476F00_OTP_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, L476F00_OTP_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_EEPROMRevision(0)
	, m_HWVersion(0)
	, m_SWVersion(0)
//	, m_DriverICID(0)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_ModuleCode, m_ModuleCode);
	DDX_Text(pDX, IDC_HWVersion, m_HWVersion);
	DDX_Text(pDX, IDC_SWVersion, m_SWVersion);

	DDX_Text(pDX, IDC_EEPROMRevision, m_EEPROMRevision);
	DDX_Text(pDX, IDC_ActuatorID, m_ActuatorID);
	DDX_Text(pDX, IDC_LensID, m_LensID);
	DDX_Text(pDX, IDC_ManuID, m_ManufactureID);
	DDX_Text(pDX, IDC_FactoryID, m_FactoryID);
	DDX_Text(pDX, IDC_ManufactureLine, m_ManufactureLine);
	DDX_Text(pDX, IDC_LPN1, m_LPN1);
	DDX_Text(pDX, IDC_LPN2, m_LPN2);
	DDX_Text(pDX, IDC_LPN3, m_LPN3);
	DDX_Text(pDX, IDC_LPN4, m_LPN4);
	DDX_Text(pDX, IDC_LPN5, m_LPN5);
	DDX_Text(pDX, IDC_LPN6, m_LPN6);
	DDX_Text(pDX, IDC_LPN7, m_LPN7);
	DDX_Text(pDX, IDC_LPN8, m_LPN8);
	//DDX_Text(pDX, IDC_OIS_VERSION, m_OISfirmwareversion);
	//DDX_Text(pDX, IDC_CAMERA_STAGE, m_Camerabuildingstage);
	//DDX_Text(pDX, IDC_PRODUCTION_FACTORY, m_Productionfactory);
	//DDX_Text(pDX, IDC_PRODUCTION_LINE, m_Productionline);




//	DDX_Text(pDX, IDC_DriverICID, m_DriverICID);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
	ON_BN_CLICKED(IDC_BUTTON1, &OptionDlg::OnBnClickedButton1)
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

void OptionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pTestItem->Dump();
}
