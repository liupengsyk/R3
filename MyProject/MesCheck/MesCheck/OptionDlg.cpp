// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MesCheck.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "MesCheck_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, MesCheck_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CAM0, m_pTestItem->bCam0Mes);
	DDX_Check(pDX, IDC_CHECK_CAM1, m_pTestItem->bCam1Mes);
	DDX_Text(pDX, IDC_EDIT_CAM0, m_pTestItem->Cam0MesID);
	DDX_Text(pDX, IDC_EDIT_CAM1, m_pTestItem->Cam1MesID);
	DDX_Check(pDX,IDC_USEBARCODE,m_pTestItem->bUseBarcode);
	DDX_Text(pDX,IDC_BARCODE_LENGTH,m_pTestItem->iBarcodeLength);
	DDX_Text(pDX,IDC_KEYSTRING,m_pTestItem->BarcodeKeyString);
	DDX_Check(pDX, IDC_CHECK_MesBinding, m_pTestItem->bMesBinding);
	DDX_Check(pDX, IDC_CHECK_MesCheck, m_pTestItem->bMesCheck);
	DDX_Check(pDX,IDC_CHECK_MesDelete,m_pTestItem->bMesDelete);
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