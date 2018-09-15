// SFRROISelect.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BoardChartAutoROI.h"
#include "SFRROISelect.h"
#include "afxdialogex.h"

#include "atlimage.h"
// CSFRROISelect �Ի���

#define MAX_FIELD_NUMBER 7

IMPLEMENT_DYNAMIC(CSFRROISelect, CDialog)

CSFRROISelect::CSFRROISelect(CWnd* pParent /*=NULL*/)
	: CDialog(CSFRROISelect::IDD, pParent)
{

}

CSFRROISelect::~CSFRROISelect()
{
}

void CSFRROISelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_MOVE, m_moveOffset);
}


BEGIN_MESSAGE_MAP(CSFRROISelect, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSFRROISelect::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_SelectAll, &CSFRROISelect::OnBnClickedSelectall)
END_MESSAGE_MAP()


// CSFRROISelect ��Ϣ�������
CString CSFRROISelect::GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
	GetModuleFileNameA(NULL, ModulePath, 128);
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy_s(path, ModulePath);	
			break;
		}
	}

	if (!flag)
	{
		strcpy_s(path, "");

	}
	myStr.Format(path);
	return myStr;
}

BOOL CSFRROISelect::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString m_szSetup = GetModulePath()+_T("ofset.ini");
	char section[256] = {0};
	CString tempStringH = _T("");
	CString tempStringV = _T("");

	int  tempdata = 0;

	//���ж������ӳ��Ƿ�ѡ
	tempStringH.Format(_T("H_B1"));
	tempStringV.Format(_T("V_B1"));

	tempdata = GetPrivateProfileInt("SFRROIPosition",tempStringH,0,m_szSetup);
	tempdata += GetPrivateProfileInt("SFRROIPosition",tempStringV,0,m_szSetup);

	if (tempdata==2)
	{
			((CButton*)GetDlgItem(IDC_F0_VIEW))->SetCheck(1);
	}
	else
	{
			((CButton*)GetDlgItem(IDC_F0_VIEW))->SetCheck(0);
	}

	//�����ж����������ӳ��Ƿ�ѡ
	for(int i=0;i<(MAX_FIELD_NUMBER-1);i++)
	{
		tempdata = 0;
		for(int n = 0; n < 4; n++)
		{
			tempStringH.Format(_T("H_B"));
			tempStringV.Format(_T("V_B"));
			sprintf_s(section, "%d",((4*i)+(n+2)));
			tempStringH += section;
			tempStringV += section;

			tempdata += GetPrivateProfileInt("SFRROIPosition",tempStringH,0,m_szSetup);
			tempdata += GetPrivateProfileInt("SFRROIPosition",tempStringV,0,m_szSetup);
		}
	
		if (tempdata==8)
		{
			((CButton*)GetDlgItem(IDC_F3_VIEW+i))->SetCheck(1);
		}
		else
		{
			((CButton*)GetDlgItem(IDC_F3_VIEW+i))->SetCheck(0);
		}
	}

	char Temp[512]={0};

	GetPrivateProfileString("SFRROIPosition","ROI_Size","128",Temp,512,m_szSetup);
	GetDlgItem(IDC_ROI_Size)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","Offset","10",Temp,512,m_szSetup);
	GetDlgItem(IDC_MOVE)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","Threshold1","0.25",Temp,512,m_szSetup);
	GetDlgItem(IDC_throald1)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","Threshold2","0.6",Temp,512,m_szSetup);
	GetDlgItem(IDC_throald2)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","CircleS","1",Temp,512,m_szSetup);
	GetDlgItem(IDC_CircleS)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","CircleRMax","1",Temp,512,m_szSetup);
	GetDlgItem(IDC_CircleRMax)->SetWindowText(Temp);

	GetPrivateProfileString("SFRROIPosition","CircleRMin","1",Temp,512,m_szSetup);
	GetDlgItem(IDC_CircleRMin)->SetWindowText(Temp);

	tempdata = GetPrivateProfileInt("SFRROIPosition","bDebugMode",0,m_szSetup);
	((CButton*)GetDlgItem(IDC_DEBUG_MODE))->SetCheck(tempdata);

	tempdata = GetPrivateProfileInt("SFRROIPosition","SaveErrorImage",0,m_szSetup);
	((CButton*)GetDlgItem(IDC_SAVE_ERROR_IMAGE))->SetCheck(tempdata);

	//m_moveOffset.SetTooltipText("���ѡ���ĵ����б��̫����ѡ���ROI΢�Ƶ�pixel��Ŀ");

//  �����ӳ�����ˣ�����ȡ����ʾ����ͼƬ��    legertian  2016-12-28
// 	CStatic* pWnd = (CStatic*)GetDlgItem(IDC_PIC); // �õ� Picture Control ���
// 	CImage image;
// 	m_szSetup = GetModulePath()+_T("Chart.png");
// 	image.Load(m_szSetup);
// 	HBITMAP hBmp = image.Detach();
// 	pWnd->SetBitmap(hBmp);

	return TRUE;  
}

void CSFRROISelect::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString m_szSetup = GetModulePath()+_T("ofset.ini");

	CString tempData = _T(""); 
	CString tempStringH = _T("");
	CString tempStringV = _T("");
	char section[256] = {0};

	//���ж������ӳ��Ƿ�ѡ
	tempStringH.Format(_T("H_B1"));
	tempStringV.Format(_T("V_B1"));

	if(((CButton*)GetDlgItem(IDC_F0_VIEW))->GetCheck() == BST_CHECKED)    
	{
		tempData.Format("1");
	}
	else
	{
		tempData.Format("0");
	}
	WritePrivateProfileString("SFRROIPosition",tempStringH,tempData,m_szSetup);
	WritePrivateProfileString("SFRROIPosition",tempStringV,tempData,m_szSetup);

	//�����ж����������ӳ��Ƿ�ѡ
	for(int i=0;i<(MAX_FIELD_NUMBER-1);i++)
	{
		if(((CButton*)GetDlgItem(IDC_F3_VIEW+i))->GetCheck() == BST_CHECKED)    
		{
			tempData.Format("1");
		}
		else
		{
			tempData.Format("0");
		}
	
		for(int n = 0; n < 4; n++)
		{
			tempStringH.Format(_T("H_B"));
			tempStringV.Format(_T("V_B"));
			sprintf_s(section, "%d",((4*i)+(n+2)));
			tempStringH += section;
			tempStringV += section;

			WritePrivateProfileString("SFRROIPosition",tempStringH,tempData,m_szSetup);
			WritePrivateProfileString("SFRROIPosition",tempStringV,tempData,m_szSetup);
		}
	}

	if(((CButton*)GetDlgItem(IDC_DEBUG_MODE))->GetCheck() == BST_CHECKED)    
	{
		WritePrivateProfileString("SFRROIPosition","bDebugMode","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("SFRROIPosition","bDebugMode","0",m_szSetup);
	}

	if(((CButton*)GetDlgItem(IDC_SAVE_ERROR_IMAGE))->GetCheck() == BST_CHECKED)    
	{
		WritePrivateProfileString("SFRROIPosition","SaveErrorImage","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("SFRROIPosition","SaveErrorImage","0",m_szSetup);
	}

	char Temp[512]={0};

	GetDlgItemText(IDC_ROI_Size,Temp,512);
	WritePrivateProfileString("SFRROIPosition","ROI_Size",Temp,m_szSetup);

	GetDlgItemText(IDC_MOVE,Temp,512);
	WritePrivateProfileString("SFRROIPosition","Offset",Temp,m_szSetup);

	GetDlgItemText(IDC_throald1,Temp,512);
	WritePrivateProfileString("SFRROIPosition","Threshold1",Temp,m_szSetup);

	GetDlgItemText(IDC_throald2,Temp,512);
	WritePrivateProfileString("SFRROIPosition","Threshold2",Temp,m_szSetup);

// 	GetDlgItemText(IDC_CircleS,Temp,512);
// 	WritePrivateProfileString("SFRROIPosition","CircleS",Temp,m_szSetup);
// 
// 	GetDlgItemText(IDC_CircleRMax,Temp,512);
// 	WritePrivateProfileString("SFRROIPosition","CircleRMax",Temp,m_szSetup);
// 
// 	GetDlgItemText(IDC_CircleRMin,Temp,512);
// 	WritePrivateProfileString("SFRROIPosition","CircleRMin",Temp,m_szSetup);
}


void CSFRROISelect::OnBnClickedSelectall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempName = _T("");
	GetDlgItemText(IDC_SelectAll,tempName);
	if (tempName == _T("ȫѡ"))
	{
		for(int i=0;i<MAX_FIELD_NUMBER;i++)
		{
			((CButton*)GetDlgItem(IDC_F0_VIEW+i))->SetCheck(TRUE);    
		}
	
		SetDlgItemText(IDC_SelectAll,_T("ȡ��ȫѡ"));
	}
	else
	{
		for(int i=0;i<MAX_FIELD_NUMBER;i++)
		{
			((CButton*)GetDlgItem(IDC_F0_VIEW+i))->SetCheck(FALSE);    
		}
	
		SetDlgItemText(IDC_SelectAll,_T("ȫѡ"));
	}

	UpdateData(FALSE);
}
