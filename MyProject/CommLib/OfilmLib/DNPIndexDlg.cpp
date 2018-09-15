// DNPIndexDlg.cpp : implementation file
//

#include "stdafx.h"
// #include "..\Include\DTHSDemo.h"
// #include "..\Function_Dialog\DTHSDemoDlg.h"
#include "DNPIndexDlg.h"
#include "OfilmLib.h"
#include "TestDef.h"
/*#include "..\Include\Common.h"*/
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDNPIndexDlg dialog


CDNPIndexDlg::CDNPIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDNPIndexDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDNPIndexDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	CString Temp;
	Temp.Format("构造函数被执行:pParent=%d,m_Parent=%d",(int)pParent,(int)m_pParent);
	OutputDebugString(Temp);
}


void CDNPIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDNPIndexDlg)
	DDX_Control(pDX, IDC_LIST3, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDNPIndexDlg, CDialog)
	//{{AFX_MSG_MAP(CDNPIndexDlg)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDD_AVERAGE, OnAverage)
	ON_BN_CLICKED(IDD_WRITEINTOINI, OnWriteintoSpec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDNPIndexDlg message handlers

void CDNPIndexDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default 
 	m_pParent->PostMessage( WM_DEL_DNPINDEX_DLG, (WPARAM)this );  
	CDialog::OnClose();
}

void CDNPIndexDlg::OnDestroy() 
{ 
	m_pParent->PostMessage( WM_DEL_DNPINDEX_DLG, (WPARAM)this );  
	CDialog::OnDestroy(); 
}

void CDNPIndexDlg::OnOK() 
{

// TODO: Add extra validation here
// m_pParent->PostMessage( WM_DEL_DNPINDEX_DLG, (WPARAM)this );  	
// CDialog::OnOK();

   m_pParent->SendMessage(WM_GETDNPINDEX, (WPARAM)this );    

}

void CDNPIndexDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_pParent->PostMessage( WM_DEL_DNPINDEX_DLG, (WPARAM)this );    
	CDialog::OnCancel();
}

BOOL CDNPIndexDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_CameraCnt  = 0;
	
	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN);
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN);
	int iCAPTION     = GetSystemMetrics(SM_CYCAPTION);
	int x0 =  648+10;
	int y0 = 15 + ((iFullScrn_cy - 610) >> 1);

 	this->SetWindowPos( NULL, x0, y0, 660, 600, SWP_SHOWWINDOW );

 	m_List.SetExtendedStyle(  LVS_EX_GRIDLINES);
	 
	m_List.InsertColumn( 0, "ID", LVCFMT_CENTER, 80 );

//	m_List.InsertColumn( 1, "RG_OTP", LVCFMT_CENTER, 80 );
//	m_List.InsertColumn( 2, "BG_OTP", LVCFMT_CENTER, 80 ); 
//	m_List.InsertColumn( 3, "GbGr_OTP", LVCFMT_CENTER, 100 ); 

	m_List.InsertColumn( 1, "RG_Cur", LVCFMT_CENTER, 80 ); 
	m_List.InsertColumn( 2, "BG_Cur", LVCFMT_CENTER, 80 ); 
 	m_List.InsertColumn( 3, "GbGr_Cur", LVCFMT_CENTER, 100 );

	m_List.InsertColumn( 4, "RG_Index", LVCFMT_CENTER, 80 ); 
	m_List.InsertColumn( 5, "BG_Index", LVCFMT_CENTER, 80 ); 
	m_List.InsertColumn( 6, "GbGr_Index", LVCFMT_CENTER, 100 ); 

	m_RGIndexSpec=GetPrivateProfileInt("OTP","DNPRGIndexSpec",2000,m_Setup);
	m_BGIndexSpec=GetPrivateProfileInt("OTP","DNPBGIndexSpec",2000,m_Setup);
	m_GGIndexSpec=GetPrivateProfileInt("OTP","DNPGGIndexSpec",2000,m_Setup);

	return TRUE;   
}

void CDNPIndexDlg::InsertTestDataIntoForm(int nIndex)
{ 
	CString str;  

	if( nIndex == -1 )
	{ 
		nIndex = m_CameraCnt;

		CString tempStr = m_List.GetItemText( nIndex, 0 );

		if (tempStr.GetLength() == 0 )
		{
	 		m_List.InsertItem( nIndex, "Average" );    

			str.Format("%d", m_RGIndex);
			m_List.SetItemText( nIndex, 4, str); 

			str.Format("%d", m_BGIndex );
			m_List.SetItemText( nIndex, 5, str); 

			str.Format("%d", m_GGIndex );
			m_List.SetItemText( nIndex, 6, str);  
		}
		else
		{
			str.Format("%d", m_RGIndex);
			m_List.SetItemText( nIndex, 4, str); 

			str.Format("%d", m_BGIndex );
			m_List.SetItemText( nIndex, 5, str); 

			str.Format("%d", m_GGIndex );
			m_List.SetItemText( nIndex, 6, str);   
		} 
	}
	else
	{ 
		str.Format("%d", nIndex+1);
		m_List.InsertItem( nIndex, str );   
		
		str.Format("%.2f", m_RGCurr);
		m_List.SetItemText( nIndex, 1, str); 

		str.Format("%.2f", m_BGCurr );
		m_List.SetItemText( nIndex, 2, str); 

		str.Format("%.2f", m_GGCurr );
		m_List.SetItemText( nIndex, 3, str); 

		str.Format("%d", m_RGIndex);
		m_List.SetItemText( nIndex, 4, str); 

		str.Format("%d", m_BGIndex );
		m_List.SetItemText( nIndex, 5, str); 

		str.Format("%d", m_GGIndex );
		m_List.SetItemText( nIndex, 6, str);  
	}
}

void CDNPIndexDlg::OnAverage() 
{
	// TODO: Add your control notification handler code here  
	if( m_CameraCnt < 3 )
	{
		AfxMessageBox("测试的模组少于3颗！");
		return;
	}

	int SumRG, SumBG, SumGG;
 

	SumRG = 0; 
	SumBG = 0;
	SumGG = 0; 
	 
	CString strRG;
	CString strBG;
	CString strGG;

	//GetItemText( int nItem, int nSubItem, LPTSTR lpszText, int nLen ) 

	for( int i = 0; i < m_CameraCnt; i++ )
	{
		 strRG = m_List.GetItemText( i, 4 );
		 strBG = m_List.GetItemText( i, 5 );
		 strGG = m_List.GetItemText( i, 6 );

		 SumRG += strtol(strRG.GetBuffer(255), NULL, 10 );
		 SumBG += strtol(strBG.GetBuffer(255), NULL, 10 );
		 SumGG += strtol(strGG.GetBuffer(255), NULL, 10 );		 
	} 


	m_RGIndex = int( SumRG*1.0/m_CameraCnt+0.5 );
	m_BGIndex = int( SumBG*1.0/m_CameraCnt+0.5 );
	m_GGIndex = int( SumGG*1.0/m_CameraCnt+0.5 ); 

	InsertTestDataIntoForm(-1);  

	GetDlgItem(IDD_WRITEINTOINI)->EnableWindow();

//	 m_pParent->PostMessage( WM_GETDNPINDEX, (WPARAM)this );  

}

void CDNPIndexDlg::OnWriteintoSpec() 
{
	// TODO: Add your control notification handler code here
//	m_szSetup

	char  section[128];
	if(m_RGIndex<(10000-m_RGIndexSpec)||m_RGIndex>(10000+m_RGIndexSpec))
	{
		CString Temp;
		Temp.Format("m_RGIndex超出范围，正负%d",m_RGIndexSpec);
		MessageBox(Temp,"校验失败",MB_OK|MB_ICONERROR);
		return;
	}
	if(m_BGIndex<(10000-m_BGIndexSpec)||m_RGIndex>(10000+m_BGIndexSpec))
	{
		CString Temp;
		Temp.Format("m_BGIndex超出范围，正负%d",m_BGIndexSpec);
		MessageBox(Temp,"校验失败",MB_OK|MB_ICONERROR);
		return;
	}
	if(m_GGIndex<(10000-m_GGIndexSpec)||m_GGIndex>(10000+m_GGIndexSpec))
	{
		CString Temp;
		Temp.Format("m_GGIndex超出范围，正负%d",m_GGIndexSpec);
		MessageBox(Temp,"校验失败",MB_OK|MB_ICONERROR);	
		return;
	}

	WritePrivateProfileString("OTP", "DNPRGIndex", itoa(m_RGIndex, section, 10), m_Setup);
	WritePrivateProfileString("OTP", "DNPBGIndex", itoa(m_BGIndex, section, 10), m_Setup);
	WritePrivateProfileString("OTP", "DNPGGIndex", itoa(m_GGIndex, section, 10), m_Setup); 
	//int iTimeTag=GetTimeTag();
	//WritePrivateProfileString("OTP", "CalTimeTag", itoa(iTimeTag, section, 10), m_Setup);
	AfxMessageBox("导入完成！");

	
}
