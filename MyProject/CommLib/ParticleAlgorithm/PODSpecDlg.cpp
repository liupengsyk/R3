// PODSpecDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ParticleAlgorithm.h"
#include "PODSpecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PODSpecDlg dialog
CString GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
	GetModuleFileName(NULL, ModulePath, 128);
	len = strlen(ModulePath);
	for (i = len - 1; i >= 0; i--)
	{
		if (ModulePath[i] == '\\')
		{
			ModulePath[i + 1] = 0;
			flag = true;
			strcpy(path, ModulePath);	
			break;
		}
	}
	
	if (!flag)
	{
		strcpy(path, "");
		
	}
	myStr.Format(path);
	return myStr;
}

PODSpecDlg::PODSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PODSpecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PODSpecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void PODSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PODSpecDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PODSpecDlg, CDialog)
	//{{AFX_MSG_MAP(PODSpecDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PODSpecDlg message handlers

void PODSpecDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_WoundEn))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam0","Wound_Enable","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam0","Wound_Enable","0",m_szSetup);
	}
	
	if (((CButton*)GetDlgItem(IDC_DeadEn))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam0","Dead_Enable","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam0","Dead_Enable","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USERAW))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam0","DeadUnit","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam0","DeadUnit","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USERAW2))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam0","WoundUnit","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam0","WoundUnit","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USELOG))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam0","UseLog","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam0","UseLog","0",m_szSetup);
	}
	char Temp[512]={0};
	GetDlgItemText(IDC_EDIT_AREAHEIGHT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","AreaHeight",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_AREAWIDTH,Temp,512);
	WritePrivateProfileString("Spec_Cam0","AreaWidth",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_SAREAHEIGHT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","smallAreaHeight",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_SAREAWIDTH,Temp,512);
	WritePrivateProfileString("Spec_Cam0","smallAreaWidth",Temp,m_szSetup); 
	GetDlgItemText(IDC_BOTTOM,Temp,512);
	WritePrivateProfileString("Spec_Cam0","BottomSL",Temp,m_szSetup);
	GetDlgItemText(IDC_TOP,Temp,512);
	WritePrivateProfileString("Spec_Cam0","TopSL",Temp,m_szSetup);
	GetDlgItemText(IDC_RIGHT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","RightSL",Temp,m_szSetup);
	GetDlgItemText(IDC_LEFT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","LeftSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundBOTTOM,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WBottomSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundTOP,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WTopSL",Temp,m_szSetup); 
	GetDlgItemText(IDC_WoundRIGHT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WRightSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundLEFT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WLeftSL",Temp,m_szSetup); 
	GetDlgItemText(IDC_WoundSpec_200,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WoundSpc_center",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundSpec150_200,Temp,512);
	WritePrivateProfileString("Spec_Cam0","WoundSpc_edge",Temp,m_szSetup); 
	GetDlgItemText(IDC_DeadSpec,Temp,512);
	WritePrivateProfileString("Spec_Cam0","DeadSpc",Temp,m_szSetup);
	GetDlgItemText(IDC_ClusterType,Temp,512);
	WritePrivateProfileString("Spec_Cam0","ClusterType",Temp,m_szSetup); 

	///////////////////////Cam1////////////////////////////////
	if (((CButton*)GetDlgItem(IDC_WoundEn2))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam1","Wound_Enable","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam1","Wound_Enable","0",m_szSetup);
	}
	
	if (((CButton*)GetDlgItem(IDC_DeadEn2))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam1","Dead_Enable","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam1","Dead_Enable","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USERAW3))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam1","DeadUnit","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam1","DeadUnit","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USERAW4))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam1","WoundUnit","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam1","WoundUnit","0",m_szSetup);
	}
	if (((CButton*)GetDlgItem(IDC_USELOG2))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec_Cam1","UseLog","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec_Cam1","UseLog","0",m_szSetup);
	}

	GetDlgItemText(IDC_EDIT_AREAHEIGHT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","AreaHeight",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_AREAWIDTH2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","AreaWidth",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_SAREAHEIGHT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","smallAreaHeight",Temp,m_szSetup);
	GetDlgItemText(IDC_EDIT_SAREAWIDTH2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","smallAreaWidth",Temp,m_szSetup); 
	GetDlgItemText(IDC_BOTTOM2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","BottomSL",Temp,m_szSetup);
	GetDlgItemText(IDC_TOP2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","TopSL",Temp,m_szSetup);
	GetDlgItemText(IDC_RIGHT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","RightSL",Temp,m_szSetup);
	GetDlgItemText(IDC_LEFT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","LeftSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundBOTTOM2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WBottomSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundTOP2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WTopSL",Temp,m_szSetup); 
	GetDlgItemText(IDC_WoundRIGHT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WRightSL",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundLEFT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WLeftSL",Temp,m_szSetup); 
	GetDlgItemText(IDC_WoundSpec_201,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WoundSpc_center",Temp,m_szSetup);
	GetDlgItemText(IDC_WoundSpec150_201,Temp,512);
	WritePrivateProfileString("Spec_Cam1","WoundSpc_edge",Temp,m_szSetup); 
	GetDlgItemText(IDC_DeadSpec2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","DeadSpc",Temp,m_szSetup);
	GetDlgItemText(IDC_ClusterType2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","ClusterType",Temp,m_szSetup); 
	if(GetParent()==0)
	OnOK(); 
	
}

BOOL PODSpecDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_szSetup=GetModulePath()+"OfPODTest.ini";
	char Temp[512]={0};

	GetPrivateProfileString("Spec_Cam0","AreaHeight","9",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_AREAHEIGHT,Temp);
    GetPrivateProfileString("Spec_Cam0","AreaWidth","9",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_AREAWIDTH,Temp);
	GetPrivateProfileString("Spec_Cam0","smallAreaHeight","3",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_SAREAWIDTH,Temp);
    GetPrivateProfileString("Spec_Cam0","smallAreaWidth","3",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_SAREAHEIGHT,Temp);
	GetPrivateProfileString("Spec_Cam0","BottomSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_BOTTOM,Temp);
    GetPrivateProfileString("Spec_Cam0","TopSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_TOP,Temp);
	GetPrivateProfileString("Spec_Cam0","RightSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_RIGHT,Temp);
    GetPrivateProfileString("Spec_Cam0","LeftSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_LEFT,Temp);
	GetPrivateProfileString("Spec_Cam0","WBottomSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundBOTTOM,Temp);
    GetPrivateProfileString("Spec_Cam0","WTopSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundTOP,Temp);
	GetPrivateProfileString("Spec_Cam0","WRightSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundRIGHT,Temp);
    GetPrivateProfileString("Spec_Cam0","WLeftSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundLEFT,Temp);
	GetPrivateProfileString("Spec_Cam0","WoundSpc_center","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundSpec_200,Temp);
    GetPrivateProfileString("Spec_Cam0","WoundSpc_edge","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundSpec150_200,Temp);
	GetPrivateProfileString("Spec_Cam0","DeadSpc","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_DeadSpec,Temp);
    GetPrivateProfileString("Spec_Cam0","ClusterType","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_ClusterType,Temp);

	int Wound_Enable=GetPrivateProfileInt("Spec_Cam0","Wound_Enable",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_WoundEn))->SetCheck(Wound_Enable);
	int Dead_Enable=GetPrivateProfileInt("Spec_Cam0","Dead_Enable",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_DeadEn))->SetCheck(Dead_Enable);

	int DeadUnit=GetPrivateProfileInt("Spec_Cam0Spec","DeadUnit",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USERAW))->SetCheck(DeadUnit);
	int WoundUnit=GetPrivateProfileInt("Spec_Cam0","WoundUnit",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USERAW2))->SetCheck(WoundUnit);
	int UseLog=GetPrivateProfileInt("Spec_Cam0","UseLog",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USELOG))->SetCheck(UseLog);

	/////////////////Cam1/////////////////////////
	GetPrivateProfileString("Spec_Cam1","AreaHeight","9",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_AREAHEIGHT2,Temp);
    GetPrivateProfileString("Spec_Cam1","AreaWidth","9",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_AREAWIDTH2,Temp);
	GetPrivateProfileString("Spec_Cam1","smallAreaHeight","3",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_SAREAWIDTH2,Temp);
    GetPrivateProfileString("Spec_Cam1","smallAreaWidth","3",Temp,512,m_szSetup);
    SetDlgItemText(IDC_EDIT_SAREAHEIGHT2,Temp);
	GetPrivateProfileString("Spec_Cam1","BottomSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_BOTTOM2,Temp);
    GetPrivateProfileString("Spec_Cam1","TopSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_TOP2,Temp);
	GetPrivateProfileString("Spec_Cam1","RightSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_RIGHT2,Temp);
    GetPrivateProfileString("Spec_Cam1","LeftSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_LEFT2,Temp);
	GetPrivateProfileString("Spec_Cam1","WBottomSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundBOTTOM2,Temp);
    GetPrivateProfileString("Spec_Cam1","WTopSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundTOP2,Temp);
	GetPrivateProfileString("Spec_Cam1","WRightSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundRIGHT2,Temp);
    GetPrivateProfileString("Spec_Cam1","WLeftSL","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundLEFT2,Temp);
	GetPrivateProfileString("Spec_Cam1","WoundSpc_center","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundSpec_201,Temp);
    GetPrivateProfileString("Spec_Cam1","WoundSpc_edge","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_WoundSpec150_201,Temp);
	GetPrivateProfileString("Spec_Cam1","DeadSpc","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_DeadSpec2,Temp);
    GetPrivateProfileString("Spec_Cam1","ClusterType","0",Temp,512,m_szSetup);
    SetDlgItemText(IDC_ClusterType2,Temp);
	
	int Wound_Enable2=GetPrivateProfileInt("Spec_Cam1","Wound_Enable",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_WoundEn2))->SetCheck(Wound_Enable2);
	int Dead_Enable2=GetPrivateProfileInt("Spec_Cam1","Dead_Enable",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_DeadEn2))->SetCheck(Dead_Enable2);
	
	int DeadUnit2=GetPrivateProfileInt("Spec_Cam1","DeadUnit",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USERAW3))->SetCheck(DeadUnit2);
	int WoundUnit2=GetPrivateProfileInt("Spec_Cam1","WoundUnit",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USERAW4))->SetCheck(WoundUnit2);
	int UseLog2=GetPrivateProfileInt("Spec_Cam1","UseLog",1,m_szSetup);
    ((CButton*)GetDlgItem(IDC_USELOG2))->SetCheck(UseLog2);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
