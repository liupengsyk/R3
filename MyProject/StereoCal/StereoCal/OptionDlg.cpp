// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StereoCal.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "StereoCal_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, StereoCal_TestItem* pTestItem)
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
	
	DDX_Text(pDX,IDC_COMBO_Platform,m_pTestItem->Platform);
	DDX_Text(pDX,IDC_EDIT_CFG_NAME,m_pTestItem->ConfigName);
	DDX_Text(pDX,IDC_EDIT_LF,m_pTestItem->leftFarImgPath);
	DDX_Text(pDX,IDC_EDIT_LN,m_pTestItem->leftNearImgPath);
	DDX_Text(pDX,IDC_EDIT_RN,m_pTestItem->rightNearImgPath);
	DDX_Text(pDX,IDC_EDIT_RF,m_pTestItem->rightFarImgPath);
	DDX_Text(pDX,IDC_EDIT_MainCamID,m_pTestItem->MainCamID);

	DDX_Text(pDX,IDC_EDIT_CFG_DIR,m_pTestItem->cfg_dir);

	DDX_Text(pDX,IDC_EDIT_IMAGE_DIR,m_pTestItem->image_dir);

	//

	DDX_Text(pDX,IDC_EDITCAM0EFL,m_pTestItem->m_CamInfo[0].CamEFL);
	DDX_Text(pDX,IDC_EDITCAM1EFL,m_pTestItem->m_CamInfo[1].CamEFL);
	DDX_Text(pDX,IDC_EDITCAM0PIXEL,m_pTestItem->m_CamInfo[0].CamPixelSize);
	DDX_Text(pDX,IDC_EDITCAM1PIXEL,m_pTestItem->m_CamInfo[1].CamPixelSize);

	DDX_Text(pDX,IDC_EDITDOTPITCH,m_pTestItem->m_ChartInfo.objectPitch_x);
	DDX_Text(pDX,IDC_EDITNEARCHARTDIST,m_pTestItem->m_ChartInfo.NearChartDist);
	DDX_Text(pDX,IDC_EDITFARCHARTDIST,m_pTestItem->m_ChartInfo.FarChartDist);
	


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
	CComboBox * Platform = (CComboBox *)GetDlgItem(IDC_COMBO_Platform);
	Platform->AddString(_T("Mtk"));
	Platform->AddString(_T("Qualcomm")); 
	//Ofilm
	Platform->AddString(_T("Ofilm"));  
	Platform->AddString(_T("HwHdc")); 
	Platform->AddString(_T("HwMMI")); 
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}