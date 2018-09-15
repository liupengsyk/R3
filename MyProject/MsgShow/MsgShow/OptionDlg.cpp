// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MsgShow.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "MsgShow_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, MsgShow_TestItem* pTestItem)
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
	DDX_Check(pDX,IDC_bShowInfo,m_pTestItem->bShowInfo);
	DDX_Text(pDX,IDC_InfoSize,m_pTestItem->InfoSize);
	DDX_Text(pDX,IDC_InfoShow,m_pTestItem->InfoShow);
	DDX_Text(pDX,IDC_InfoPosx,m_pTestItem->InfoPos.x);
	DDX_Text(pDX,IDC_InfoPosy,m_pTestItem->InfoPos.y);
	DDX_Text(pDX,IDC_ColorBox,m_pTestItem->InfoColor);
	 

	DDX_Check(pDX,IDC_bDelay,m_pTestItem->bDelay);
	DDX_Text(pDX,IDC_DelayTime,m_pTestItem->DelayTime); 

	DDX_Check(pDX,IDC_bShowMsgDlg,m_pTestItem->bShowMsgDlg);
	DDX_Text(pDX,IDC_MsgInfo,m_pTestItem->MsgInfo);
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

	CComboBox* ColorBox = (CComboBox*)GetDlgItem(IDC_ColorBox);

	ColorBox->AddString(_T("��ɫ"));
	ColorBox->AddString(_T("��ɫ"));
	ColorBox->AddString(_T("��ɫ")); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}