// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetLEDChannel.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "SetLEDChannel_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, SetLEDChannel_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
	, m_BLC(0)
	, LEDChannel(0)
	, m_LEDCOM(0)
	, m_SleepTime(0)
	, ALight_Channel(0)
	, TL84LightChannel(0)
	, D65LightChannel(0)
	, m_CheckEN(FALSE)
	, Checknumber(0)
	, m_LEDType(0)
{
	m_pTestItem = pTestItem;
	m_BLC = 0;
	m_ARGHigh = 0;
	m_ARGLow = 0;
	m_ABGHigh = 0;
	m_ABGLow = 0;
	m_TLRGHigh = 0;
	m_TLRGLow = 0;
	m_TLBGHigh = 0;
	m_TLBGLow = 0;
	m_DRGHigh = 0;
	m_DRGLow = 0;
	m_DBGHigh = 0;
	m_DBGLow = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_BLC, m_BLC);
	DDX_Radio(pDX, IDC_A_Channel, LEDChannel);
	DDX_Text(pDX, IDC_EDIT2, m_SleepTime);
	DDX_Text(pDX, IDC_EDIT3, ALight_Channel);
	DDX_Text(pDX, IDC_EDIT4, TL84LightChannel);
	DDX_Text(pDX, IDC_EDIT5, D65LightChannel);
	DDX_Check(pDX, IDC_CHECK1, m_CheckEN);
	DDX_Text(pDX, IDC_EDIT6, Checknumber);
	DDX_Text(pDX, IDC_EDIT7, m_ARGHigh);
	DDX_Text(pDX, IDC_EDIT8, m_ARGLow);
	DDX_Text(pDX, IDC_EDIT9, m_ABGHigh);
	DDX_Text(pDX, IDC_EDIT10, m_ABGLow);
	DDX_Text(pDX, IDC_EDIT11, m_TLRGHigh);
	DDX_Text(pDX, IDC_EDIT12, m_TLRGLow);
	DDX_Text(pDX, IDC_EDIT13, m_TLBGHigh);
	DDX_Text(pDX, IDC_EDIT14, m_TLBGLow);
	DDX_Text(pDX, IDC_EDIT15, m_DRGHigh);
	DDX_Text(pDX, IDC_EDIT16, m_DRGLow);
	DDX_Text(pDX, IDC_EDIT17, m_DBGHigh);
	DDX_Text(pDX, IDC_EDIT18, m_DBGLow);
	DDX_CBIndex(pDX, IDC_COMBO1, m_LEDType);
	DDX_Text(pDX, IDC_WaitTime, m_WaitFlag);
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