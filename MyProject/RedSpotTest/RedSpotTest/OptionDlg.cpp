// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RedSpot.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "RedSpot_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, RedSpot_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, iManualExDelayFrame(0)
	, bManualExposure(FALSE)
	, Redspot_Ymax(0)
	, Redspot_Rmax(0)
	, Redspot_Gmax(0)
	, Redspot_Bmax(0)
	, XBlockNum(0)
	, YBlockNum(0)
{
	m_pTestItem = pTestItem;

}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_EXPOSURE_VALUE, strExpVal);
	DDX_Text(pDX, IDC_EDIT_GAIN_VALUE, strGainVal);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_REG_HIGH, strExposureRegHig);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_REG_LOW, strExposureRegLow);
	DDX_Text(pDX, IDC_EDIT_GAIN_REG_HIGH, strGainRegHig);
	DDX_Text(pDX, IDC_EDIT_GAI_REG_LOW, strGainRegLow);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE_DELAY_FRAME, iManualExDelayFrame);
	DDX_Check(pDX, IDC_CHECK_ENABLE_EXPOSURE, bManualExposure);
	DDX_Text(pDX, IDC_EDIT_Y_MAX, Redspot_Ymax);
	DDX_Text(pDX, IDC_EDIT_R_MAX, Redspot_Rmax);
	DDX_Text(pDX, IDC_EDIT_G_MAX, Redspot_Gmax);
	DDX_Text(pDX, IDC_EDIT_B_MAX, Redspot_Bmax);
	DDX_Text(pDX, IDC_EDIT_XBLOCK_NUM, XBlockNum);
	DDX_Text(pDX, IDC_EDIT_YBLOCK_NUM, YBlockNum);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
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


BOOL OptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}