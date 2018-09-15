// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HwBPTTest.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "HwBPTTest_TestItem.h"
/*#include <direct.h>*/

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, HwBPTTest_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	// 	DCC_Hig = 0;
	// 	DCC_Low = 0;
	m_StaInfo = 0;
	m_VerInfo = 0;
	m_ExValue = _T("0x00");
	m_GainValue = _T("0x00");

	m_bptErrCode = _T("16");
	m_bptErrInfo = _T("Test Failed");
	m_bptFrmDelayNum = _T("5");

	m_bptBurnStartAddr = _T("0x0000");
	m_bptBurnEndAddr = _T("0x0000");
	m_SensorSelection = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// 	DDX_Check(pDX,IDC_CHECK_12,m_pTestItem->b12m);
	// 	DDX_Check(pDX,IDC_THREEPOSION,m_pTestItem->bThreePosion);
	// 	DDX_Text(pDX,IDC_DCC_HIG,DCC_Hig);
	// 	DDX_Text(pDX,IDC_DCC_LOW,DCC_Low);

	DDX_Text(pDX,IDC_STAINFO,m_StaInfo);
	DDX_Text(pDX,IDC_VERINFO,m_VerInfo);

	DDX_Text(pDX,IDC_EX,m_ExValue);
	DDX_Text(pDX,IDC_GAIN,m_GainValue);

	DDX_Text(pDX, IDC_EDIT_BPT_ERROR_CODE, m_bptErrCode);
	DDX_Text(pDX, IDC_EDIT_BPT_ERROR_INFO, m_bptErrInfo);
	DDX_Text(pDX, IDC_EDIT_FRAME_DELAY_NUMBER, m_bptFrmDelayNum);

	DDX_Text(pDX, IDC_EDIT_BPT_BURN_START_ADDR, m_bptBurnStartAddr);
	DDX_Text(pDX, IDC_EDIT_BPT_BURN_END_ADDR, m_bptBurnEndAddr);
	DDX_Radio(pDX, IDC_RADIO1, m_SensorSelection);
	DDX_Check(pDX, IDC_SavePicture, m_SavePic);
	DDX_Check(pDX, IDC_SAVE_DATA, m_SaveData);
}


BEGIN_MESSAGE_MAP(OptionDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_INSPECITON_OPTION, &OptionDlg::OnUpdateData)
	ON_MESSAGE(WM_INSPCETIONDLL_KEY_CTRLNUM, &OptionDlg::OnCtrlKeyNum)
	ON_MESSAGE(WM_CAMERA_STOP, &OptionDlg::OnCameraStop)
//	ON_BN_CLICKED(IDC_READOTPDATA, &OptionDlg::OnBnClickedReadotpdata)
//	ON_EN_CHANGE(IDC_GAIN, &OptionDlg::OnEnChangeGain)
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
