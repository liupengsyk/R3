// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DParticle.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "DParticle_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, DParticle_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, nLeftSL(0)
	, nRightSL(0)
	, nTopSL(0)
	, nBottomSL(0)
	, nAreaWidth(0)
	, nAreaHeight(0)
	, nDeadUnit(0)
	, nDeadSpec(0)
	, nDefultExposure(0)
	, nDefultGain(0)
	, nDeadType(0)
	, bSaveImage(FALSE)
	, ErrorCode(0)
{
	m_pTestItem = pTestItem;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LeftSL, nLeftSL);
	DDX_Text(pDX, IDC_RightSL, nRightSL);
	DDX_Text(pDX, IDC_TopSL, nTopSL);
	DDX_Text(pDX, IDC_BottomSL, nBottomSL);
	DDX_Text(pDX, IDC_AreaWidth, nAreaWidth);
	DDX_Text(pDX, IDC_AreaHeight, nAreaHeight);
	DDX_Text(pDX, IDC_DeadUnit, nDeadUnit);
	DDX_Text(pDX, IDC_DeadSpec, nDeadSpec);
	DDX_Text(pDX, IDC_DefultExposure, nDefultExposure);
	DDX_Text(pDX, IDC_DefultGain, nDefultGain);
	DDX_Text(pDX, IDC_DeadType, nDeadType);
	DDX_Check(pDX, IDC_SaveImage, bSaveImage);
	DDX_Text(pDX, IDC_ERROR_CODE, ErrorCode);
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