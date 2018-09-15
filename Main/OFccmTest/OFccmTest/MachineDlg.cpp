// MachineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "MachineDlg.h"
#include "MachineInterfaceLib.h"
#include "afxdialogex.h"


// MachineDlg �Ի���

IMPLEMENT_DYNAMIC(MachineDlg, CDialogEx)

MachineDlg::MachineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(MachineDlg::IDD, pParent)
{
	 
}

MachineDlg::~MachineDlg()
{

}

void MachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MachineSetTab, m_MyTabSheet);
}


BEGIN_MESSAGE_MAP(MachineDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MachineDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// MachineDlg ��Ϣ�������
BOOL MachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_MyTabSheet.Init(); 

	//����machine������ҳ
	MachineInterface* pMachine = pCcmCtrl->pMachineCtrl->p_machine;

	LPCTSTR name = pMachine->GetName();
	HWND    hDlg = pMachine->GetDlgHandle();
	if(hDlg!=NULL)
	{
		m_MyTabSheet.AddPage(name,hDlg);

		m_MyTabSheet.SetCurSel(0);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void MachineDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret = pCcmCtrl->pMachineCtrl->SaveMachineSetting();
	if (ret>0)
	{

	}
	CDialogEx::OnOK();
}

void MachineDlg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	/*	delete this;*/
	CDialogEx::PostNcDestroy();
}