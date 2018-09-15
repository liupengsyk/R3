// UnlockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OfilmLib.h"
#include "UnlockDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnlockDlg dialog

CUnlockDlg::CUnlockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnlockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnlockDlg)
	m_PWD = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnlockDlg)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Text(pDX, IDC_EDIT1, m_PWD);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnlockDlg, CDialog)
	//{{AFX_MSG_MAP(CUnlockDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnlockDlg message handlers

BOOL CUnlockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{	
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Font.CreateFont(40,0,0,0,100,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	l_Font.CreateFont(18,0,0,0,FW_HEAVY,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	n_Font.CreateFont(25,0,0,0,FW_HEAVY,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FF_SWISS,"宋体");
	GetDlgItem(IDC_EDIT1)->SetFont(&m_Font);
	GetDlgItem(IDC_SHOWNOTE)->SetFont(&l_Font);
	GetDlgItem(IDC_SHOWTEXT)->SetFont(&n_Font);
	SetTimer(3,100,NULL);

	SetWindowText(cWndText);
	SetDlgItemText(IDC_SHOWTEXT,cShowText);
	::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnlockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnlockDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnlockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUnlockDlg::OnOK() 
{
	// TODO: Add extra validation here
	Res=1;
	GetDlgItemText(IDC_EDIT1,ReturnText,50);
	CDialog::OnOK();
}

void CUnlockDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	GetDlgItem(IDC_EDIT1)->SetFocus();
}

//DEL void CUnlockDlg::OnButton1() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	short a=GetKeyState(VK_CAPITAL)&0x0001;
//DEL }

//DEL BOOL CUnlockDlg::PreTranslateMessage(MSG* pMsg) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	return CDialog::PreTranslateMessage(pMsg);
//DEL }

HBRUSH CUnlockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC)
	{
		//pDC->SetBkMode(RGB(255,255,0));//设置背景色
		pDC->SetTextColor(RGB(50, 10, 255));///文本颜色
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CUnlockDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BYTE  keyState[256];  
	//if(pMsg->hwnd  ==  GetDlgItem(IDC_EDIT1)->m_hWnd)  
	GetKeyboardState((LPBYTE)&keyState);  
	if  ((keyState[VK_CAPITAL]) == 1 )  
	{  
		GetDlgItem(IDC_SHOWNOTE)->ShowWindow(TRUE);
	} 
	else
	{
		GetDlgItem(IDC_SHOWNOTE)->ShowWindow(FALSE);
	}	
	CDialog::OnTimer(nIDEvent);
}

void CUnlockDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	Res=-1;
	CDialog::OnClose();
}
