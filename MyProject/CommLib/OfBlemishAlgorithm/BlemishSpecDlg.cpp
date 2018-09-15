// BlemishSpecDlg.cpp : implementation file
//
#include "stdafx.h"
#include "OfBlemishAlgorithm.h"
#include "BlemishSpecDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlemishSpecDlg dialog

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

CBlemishSpecDlg::CBlemishSpecDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlemishSpecDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlemishSpecDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBlemishSpecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlemishSpecDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlemishSpecDlg, CDialog)
	//{{AFX_MSG_MAP(CBlemishSpecDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlemishSpecDlg message handlers

void CBlemishSpecDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	if (((CButton*)GetDlgItem(IDC_SAVEIMAGE))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec","bSaveImage","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec","bSaveImage","0",m_szSetup);
	}

	if (((CButton*)GetDlgItem(IDC_SAVELOG))->GetCheck() == BST_CHECKED)
	{
		WritePrivateProfileString("Spec","bSaveLog","1",m_szSetup);
	}
	else
	{
		WritePrivateProfileString("Spec","bSaveLog","0",m_szSetup);
	}

	char Temp[512]={0};

	GetDlgItemText(IDC_MEDIANA,Temp,512);
	WritePrivateProfileString("Spec_Cam0","MedianA",Temp,m_szSetup);

	GetDlgItemText(IDC_MEDIANB,Temp,512);
	WritePrivateProfileString("Spec_Cam0","MedianB",Temp,m_szSetup);

	GetDlgItemText(IDC_THRESHOLD,Temp,512);
	WritePrivateProfileString("Spec_Cam0","Threshold",Temp,m_szSetup);

	GetDlgItemText(IDC_PIXELLIMIT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","PixelLimit",Temp,m_szSetup); 
 
	GetDlgItemText(IDC_AXSTART,Temp,512);
	WritePrivateProfileString("Spec_Cam0","ZoneAXStart",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AXEND,Temp,512);
	WritePrivateProfileString("Spec_Cam0","ZoneAXEnd",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AYSTART,Temp,512);
	WritePrivateProfileString("Spec_Cam0","ZoneAYStart",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AYEND,Temp,512);
	WritePrivateProfileString("Spec_Cam0","ZoneAYEnd",Temp,m_szSetup);

	GetDlgItemText(IDC_OFFSET,Temp,512);
	WritePrivateProfileString("Spec_Cam0","offset",Temp, m_szSetup);
	
	GetDlgItemText(IDC_INTENSITY,Temp,512);
	WritePrivateProfileString("Spec_Cam0","IntensityCntSpc",Temp, m_szSetup); 
 
	GetDlgItemText(IDC_CORNERDIFF,Temp,512);
	WritePrivateProfileString("Spec_Cam0","CornerDiff",Temp, m_szSetup);

	GetDlgItemText(IDC_RANGESPC,Temp,512);
	WritePrivateProfileString("Spec_Cam0","RangeSpc",Temp, m_szSetup); 

	GetDlgItemText(IDC_EDGELIMIT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","EdgeLimit",Temp, m_szSetup);
	
	GetDlgItemText(IDC_EDGEPIXELLIMIT,Temp,512);
	WritePrivateProfileString("Spec_Cam0","EdgePixelLimit",Temp, m_szSetup);


	///////////////////////Cam1//////////////////////
	GetDlgItemText(IDC_MEDIANA2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","MedianA",Temp,m_szSetup);
	
	GetDlgItemText(IDC_MEDIANB2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","MedianB",Temp,m_szSetup);
	
	GetDlgItemText(IDC_THRESHOLD2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","Threshold",Temp,m_szSetup);
	
	GetDlgItemText(IDC_PIXELLIMIT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","PixelLimit",Temp,m_szSetup); 
	
	GetDlgItemText(IDC_AXSTART2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","ZoneAXStart",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AXEND2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","ZoneAXEnd",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AYSTART2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","ZoneAYStart",Temp,m_szSetup);
	
	GetDlgItemText(IDC_AYEND2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","ZoneAYEnd",Temp,m_szSetup);
	
	GetDlgItemText(IDC_OFFSET2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","offset",Temp, m_szSetup);
	
	GetDlgItemText(IDC_INTENSITY2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","IntensityCntSpc",Temp, m_szSetup); 
	
	GetDlgItemText(IDC_CORNERDIFF2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","CornerDiff",Temp, m_szSetup);
	
	GetDlgItemText(IDC_RANGESPC2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","RangeSpc",Temp, m_szSetup); 
	
	GetDlgItemText(IDC_EDGELIMIT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","EdgeLimit",Temp, m_szSetup);
	
	GetDlgItemText(IDC_EDGEPIXELLIMIT2,Temp,512);
	WritePrivateProfileString("Spec_Cam1","EdgePixelLimit",Temp, m_szSetup);

	if(GetParent()==0)
	OnOK(); 
}

BOOL CBlemishSpecDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_szSetup=GetModulePath()+"OfblemishTest.ini";

	int bSaveImage=GetPrivateProfileInt("Spec","bSaveImage",0,m_szSetup);
	((CButton*)GetDlgItem(IDC_SAVEIMAGE))->SetCheck(bSaveImage);

	int bSaveLog=GetPrivateProfileInt("Spec","bSaveLog",0,m_szSetup);
	((CButton*)GetDlgItem(IDC_SAVELOG))->SetCheck(bSaveLog);

	char Temp[512]={0};

	GetPrivateProfileString("Spec_Cam0","MedianA","25",Temp,512,m_szSetup);
	GetDlgItem(IDC_MEDIANA)->SetWindowText(Temp);

	GetPrivateProfileString("Spec_Cam0","MedianB","5",Temp,512,m_szSetup);
	GetDlgItem(IDC_MEDIANB)->SetWindowText(Temp);

	GetPrivateProfileString("Spec_Cam0","Threshold","15",Temp,512,m_szSetup);
	GetDlgItem(IDC_THRESHOLD)->SetWindowText(Temp);

	GetPrivateProfileString("Spec_Cam0","PixelLimit","20",Temp,512,m_szSetup);
	GetDlgItem(IDC_PIXELLIMIT)->SetWindowText(Temp);  
	GetPrivateProfileString("Spec_Cam0","ZoneAXStart","0",Temp,512,m_szSetup);
	GetDlgItem(IDC_AXSTART)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","ZoneAXEnd","400",Temp,512,m_szSetup);
	GetDlgItem(IDC_AXEND)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","ZoneAYStart","0",Temp,512,m_szSetup);
	GetDlgItem(IDC_AYSTART)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","ZoneAYEnd","300",Temp,512,m_szSetup);
	GetDlgItem(IDC_AYEND)->SetWindowText(Temp); 

	
	GetPrivateProfileString("Spec_Cam0","offset","7",Temp,512,m_szSetup);
	GetDlgItem(IDC_OFFSET)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","IntensityCntSpc","7",Temp,512,m_szSetup);
	GetDlgItem(IDC_INTENSITY)->SetWindowText(Temp);  

    GetPrivateProfileString("Spec_Cam0","CornerDiff","70",Temp,512,m_szSetup);
	GetDlgItem(IDC_CORNERDIFF)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","RangeSpc","70",Temp,512,m_szSetup);
	GetDlgItem(IDC_RANGESPC)->SetWindowText(Temp); 
	

	GetPrivateProfileString("Spec_Cam0","EdgeLimit","4",Temp,512,m_szSetup);
	GetDlgItem(IDC_EDGELIMIT)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam0","EdgePixelLimit","9",Temp,512,m_szSetup);
	GetDlgItem(IDC_EDGEPIXELLIMIT)->SetWindowText(Temp); 

	////////////////Cam1////////////////////
	
	GetPrivateProfileString("Spec_Cam1","MedianA","25",Temp,512,m_szSetup);
	GetDlgItem(IDC_MEDIANA2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","MedianB","5",Temp,512,m_szSetup);
	GetDlgItem(IDC_MEDIANB2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","Threshold","15",Temp,512,m_szSetup);
	GetDlgItem(IDC_THRESHOLD2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","PixelLimit","20",Temp,512,m_szSetup);
	GetDlgItem(IDC_PIXELLIMIT2)->SetWindowText(Temp);  
	GetPrivateProfileString("Spec_Cam1","ZoneAXStart","0",Temp,512,m_szSetup);
	GetDlgItem(IDC_AXSTART2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","ZoneAXEnd","400",Temp,512,m_szSetup);
	GetDlgItem(IDC_AXEND2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","ZoneAYStart","0",Temp,512,m_szSetup);
	GetDlgItem(IDC_AYSTART2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","ZoneAYEnd","300",Temp,512,m_szSetup);
	GetDlgItem(IDC_AYEND2)->SetWindowText(Temp); 
	
	
	GetPrivateProfileString("Spec_Cam1","offset","7",Temp,512,m_szSetup);
	GetDlgItem(IDC_OFFSET2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","IntensityCntSpc","7",Temp,512,m_szSetup);
	GetDlgItem(IDC_INTENSITY2)->SetWindowText(Temp);  
	
    GetPrivateProfileString("Spec_Cam1","CornerDiff","70",Temp,512,m_szSetup);
	GetDlgItem(IDC_CORNERDIFF2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","RangeSpc","70",Temp,512,m_szSetup);
	GetDlgItem(IDC_RANGESPC2)->SetWindowText(Temp); 
	
	
	GetPrivateProfileString("Spec_Cam1","EdgeLimit","4",Temp,512,m_szSetup);
	GetDlgItem(IDC_EDGELIMIT2)->SetWindowText(Temp);
	
	GetPrivateProfileString("Spec_Cam1","EdgePixelLimit","9",Temp,512,m_szSetup);
	GetDlgItem(IDC_EDGEPIXELLIMIT2)->SetWindowText(Temp); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
 