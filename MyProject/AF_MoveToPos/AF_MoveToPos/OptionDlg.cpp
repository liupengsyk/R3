// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AF_MoveToPos.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "AF_MoveToPos_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, AF_MoveToPos_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_ModuleEFL = _T("");
	m_InfDistance = _T("");
	m_InfRegHigh = _T("");
	m_InfRegLow = _T("");
	m_MacroDistance = _T("");
	m_MacroRegHigh = _T("");
	m_MacroRegLow = _T("");
	m_MoveToPos = _T("");
	m_OtherCode = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AF_MoveCode, m_MoveToPosChecked);
	DDX_Text(pDX, IDC_EDIT_EFL, m_ModuleEFL);
	DDX_Text(pDX, IDC_EDIT_InfDistance, m_InfDistance);
	DDX_Text(pDX, IDC_EDIT_InfRegHigh, m_InfRegHigh);
	DDX_Text(pDX, IDC_EDIT_InfRegLow, m_InfRegLow);
	DDX_Text(pDX, IDC_EDIT_MacroDistance, m_MacroDistance);
	DDX_Text(pDX, IDC_EDIT_MacroRegHigh, m_MacroRegHigh);
	DDX_Text(pDX, IDC_EDIT_MacroRegLow, m_MacroRegLow);
	DDX_Check(pDX, IDC_InfinityCode, m_InfinityPosChecked);
	DDX_Check(pDX, IDC_MacroCode, m_MacroPosChecked);
	DDX_Check(pDX, IDC_MiddleCode, m_MiddlePosChecked);
	DDX_Text(pDX, IDC_MovePos, m_MoveToPos);
	DDX_Check(pDX, IDC_SaveAFCode, m_SaveAFCode);
	DDX_Text(pDX, IDC_EDIT_OtherCode, m_OtherCode);
	DDX_Check(pDX, IDC_OtherPos, m_OtherCode_En);
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