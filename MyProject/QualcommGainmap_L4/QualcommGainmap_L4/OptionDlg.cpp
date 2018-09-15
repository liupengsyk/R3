// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QualcommGainmap_L4.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "QualcommGainmap_L4_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, QualcommGainmap_L4_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_CFA(0)
	, m_BLC(0)
	, m_BIT(0)
	, m_channel(0)
	, m_block_Width(0)
	, m_Block_Height(0)
	, m_PD_pairs(0)
	, m_Start_X(0)
	, m_Start_Y(0)
	, m_PD_Block_Right_X(_T(""))
	, m_PD_Block_Left_X(_T(""))
	, m_PD_Block_Left_Y(_T(""))
	, m_PD_Block_Right_Y(_T(""))
	, m_save(0)
	, m_GainmapMAX(0)
	, m_GainmapMIN(0)
	, m_savealldata(FALSE)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CFA, m_CFA);
	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDX_Text(pDX, IDC_BIT, m_BIT);
	DDX_Text(pDX, IDC_CHANNEL, m_channel);
	DDX_Text(pDX, IDC_Block_w, m_block_Width);
	DDX_Text(pDX, IDC_Block_h, m_Block_Height);
	DDX_Text(pDX, IDC_PD_pairs, m_PD_pairs);
	DDX_Text(pDX, IDC_Start_X, m_Start_X);
	DDX_Text(pDX, IDC_Start_Y, m_Start_Y);
	DDX_Text(pDX, IDC_PD_Block_Right_X, m_PD_Block_Right_X);
	DDX_Text(pDX, IDC_PD_Block_Left_X, m_PD_Block_Left_X);
	DDX_Text(pDX, IDC_PD_Block_Left_Y, m_PD_Block_Left_Y);
	DDX_Text(pDX, IDC_PD_Block_Right_Y, m_PD_Block_Right_Y);

	DDX_Text(pDX, IDC_Save, m_save);
	DDX_Text(pDX, IDC_GainmapMAX, m_GainmapMAX);
	DDX_Text(pDX, IDC_GainmapMIN, m_GainmapMIN);

	DDX_Check(pDX, IDC_CHECK1, m_savealldata);
	DDX_Check(pDX, IDC_CHECK_IS2PD, m_is2PD);
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