// DllDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OfilmLib.h"
#include "DllDialog.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDllDialog dialog


CDllDialog::CDllDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDllDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDllDialog)
		// NOTE: the ClassWizard will add member initialization here  
	//}}AFX_DATA_INIT
}
CDllDialog::CDllDialog(CWnd* pParent,int Res,DWORD xScale,DWORD yScale,DWORD WScale,DWORD HSale,char *sPass,char *sFail,char *Other)
: CDialog(CDllDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDllDialog)
	// NOTE: the ClassWizard will add member initialization here  
	//}}AFX_DATA_INIT
	if (Res==1)
	{
		m_brush.CreateSolidBrush(RGB(0,255,0));
	}
	else if (Res==-1)
	{
		m_brush.CreateSolidBrush(RGB(255,0,0));
	}
	else
	{
		m_brush.CreateSolidBrush(RGB(255,255,0));
	}
	this->XScale=xScale;
	this->YScale=yScale;
	this->WidthScale=WScale;
	this->HeightScale=HSale;
	strcpy(this->m_sPASS,sPass);
	strcpy(this->m_sFAIL,sFail);
	strcpy(this->m_Other,Other);	
}

void CDllDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDllDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDllDialog, CDialog)
	//{{AFX_MSG_MAP(CDllDialog)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDllDialog message handlers

//DEL void CDllDialog::OnHelloButton() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	MessageBox("Hello,pconline的网友","pconline");
//DEL }

HBRUSH CDllDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
 	if (pWnd->GetDlgCtrlID()==IDC_MSG)    
	{
// 		if( m_Res == 1 )
// 		{
// 			pDC->SetTextColor(RGB(200,0,68));  /*将色彩容器中的文字颜色设置为绿色*/
// 
// 		}
// 		else if( m_Res == -1 )
// 		{
// 			pDC->SetTextColor(RGB(0,255,0)); 	
// 		}
// 		else
// 		{
// 			pDC->SetTextColor(RGB(0,0,0)); 
// 		}
// 		
 		pDC->SetBkMode(TRANSPARENT);  /*设置文字的背景为透明，否则文字背景认为系统默认色*/
 	}
	// TODO: Return a different brush if the default is not desired
	return m_brush;
}

BOOL CDllDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN);
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN);
	int iCAPTION     = GetSystemMetrics(SM_CYCAPTION);
	int x0,y0,w0,h0;

	x0=iFullScrn_cx/10*XScale;

	y0 =iFullScrn_cy/10*YScale;

	w0=iFullScrn_cx/10*WidthScale;
	
	h0=iFullScrn_cy/10*HeightScale;




	this->SetWindowPos(NULL, x0, y0, w0, h0, SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_MSG)->SetWindowPos(NULL, 20, 20, w0-50, h0-10,SWP_NOZORDER|SWP_SHOWWINDOW);

	CRect rect;
	GetClientRect(&rect);
	
	GetDlgItem(IDOK)->SetWindowPos(NULL,rect.right/2-40,rect.bottom-50,80,40,SWP_NOZORDER|SWP_SHOWWINDOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDllDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(rect); 
// 	if (m_Res == 1)
// 	{
// 		dc.FillSolidRect(rect,RGB(0,255,0)); 	
// 	}
// 	else if (m_Res == -1)
// 	{
// 		dc.FillSolidRect(rect,RGB(255,0,0)); 
// 	}
	PassMessage(m_strTemp, m_Res);
	CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}
void CDllDialog::PassMessage(CString str,int Res)
{ 
	CFont font;
	
	font.CreateFont (50, // FONT SIZE
					   20, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
					   OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
					   VARIABLE_PITCH|FF_SWISS, "宋体");
	
	GetDlgItem(IDC_MSG)->SetFont(&font); 
	
	m_Res = Res; 
	
	if( m_Res == 1 )
	{ 
		SetWindowText(m_sPASS);
	}
	else if (m_Res == -1)
	{
		SetWindowText(m_sFAIL);
	}
	else
	{
		SetWindowText(m_Other);
	}
	
	GetDlgItem(IDC_MSG)->SetWindowText(str); 
}

void CDllDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
