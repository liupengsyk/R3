// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OC_Item.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "OC_Item_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, OC_Item_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_Continue(FALSE)
{
	m_pTestItem = pTestItem;
	m_OCSpecX_hig = 0;
	m_OCSpecY_hig = 0;
	m_OCSpec_x_AfterShift = 0;
	m_OCSpec_y_AfterShift = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_OCSpecX_hig);
	DDX_Text(pDX, IDC_EDIT3, m_OCSpecY_hig);
	DDX_Check(pDX, IDC_CHECK1, m_Continue);
	DDX_Check(pDX, IDC_CHECK_OCShift, m_OCSpec_Shift);
	DDX_Text(pDX, IDC_EDIT5, m_OCSpec_x_AfterShift);
	DDX_Text(pDX, IDC_EDIT6, m_OCSpec_y_AfterShift);
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
		m_OCSpecX_hig=m_pTestItem->ReadConfigInt(_T("OCSpecX_hig"),100);
		m_OCSpecY_hig=m_pTestItem->ReadConfigInt(_T("OCSpecY_hig"),100);
		m_Continue=m_pTestItem->ReadConfigInt(_T("m_Continue"),0);
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