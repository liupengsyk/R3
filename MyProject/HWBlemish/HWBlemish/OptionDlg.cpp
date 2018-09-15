// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HWBlemish.h"
#include "OptionDlg.h"
#include "afxdialogex.h"
#include "HWBlemish_TestItem.h"

// OptionDlg �Ի���

IMPLEMENT_DYNAMIC(OptionDlg, CDialog)

OptionDlg::OptionDlg(CWnd* pParent, HWBlemish_TestItem* pTestItem)
	: CDialog(OptionDlg::IDD, pParent)
{
	m_pTestItem = pTestItem;
	m_P1 = L"";
	m_P2 = L"";
	m_FrameCount = 0;
	m_P3 = L"";
	m_P4 = L"";
	m_ShowTime = 0;
	m_DelayTimes = 0;

	m_ImgPath = L"";
	m_ImgName = L"";
	m_LoadImgEn = 0;
	m_Width = 0;
	m_Height = 0;
	m_ErrorCode = 0;
}

OptionDlg::~OptionDlg()
{
}

void OptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_FREQUENCY,m_P1);
	DDX_Text(pDX,IDC_EDIT_BRIGHTNESS,m_P2);
	DDX_Text(pDX,IDC_EDIT_FRAMECOUNT,m_FrameCount);
	DDX_Text(pDX,IDC_EDIT_P3,m_P3);
	DDX_Text(pDX,IDC_EDIT_P4,m_P4);
	DDX_Text(pDX,IDC_EDIT_SHOWTIME,m_ShowTime);

	DDX_Text(pDX,IDC_EDIT_SHOWTIME2,m_DelayTimes);

	DDX_Text(pDX,IDC_WIDTH,m_Width);
	DDX_Text(pDX,IDC_HEIGHT,m_Height);
	DDX_Check(pDX,IDC_LOADEN,m_LoadImgEn);
	DDX_Text(pDX,IDC_WIDTH2,m_ImgPath);
	DDX_Text(pDX,IDC_HEIGHT2,m_ImgName);
	DDX_Text(pDX, IDC_ERROE_CODE, m_ErrorCode);
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