// BPTDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BPTDemo.h"
#include "BPTDemoDlg.h"
#include "BPTTest.h"

#pragma comment(lib,"BPTCalibration.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

BOOL bSaveRawFile( CString sfilename, BYTE *pBuffer, UINT width, UINT height)
{
	{   
		
		HFILE rawFile;
		
		DWORD dwSizeImage = width * height * 2;
		
		rawFile = _lcreat( sfilename, 0 );
		
		if( rawFile < 0 )
			
			return FALSE;
		
		UINT len;
		
		len = _lwrite( rawFile, (LPSTR)pBuffer, dwSizeImage );
		
		_lclose( rawFile );
		
		return TRUE; 
	}
}

//读取raw8数据到buffer

BOOL bReadRawFile( BYTE *pBuffer,CString sfilename, UINT width, UINT height)
{
	{   
		
		HFILE rawFile;
		
		DWORD dwSizeImage = width * height * 2;
		
		rawFile = _lopen( sfilename, OF_READ );
		
		if( rawFile < 0 )
			
			return FALSE;
		
		UINT len;
		
		len=_lread(rawFile,pBuffer,dwSizeImage);
		
		_lclose( rawFile );
		
		return TRUE; 
	}
}


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
// CBPTDemoDlg dialog

CBPTDemoDlg::CBPTDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBPTDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBPTDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBPTDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBPTDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBPTDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CBPTDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBPTDemoDlg message handlers

BOOL CBPTDemoDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBPTDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBPTDemoDlg::OnPaint() 
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
HCURSOR CBPTDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBPTDemoDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	int nWidth = 3968;
	int nHeight = 2976;
	BYTE* image = new BYTE[nWidth*nHeight*2];
	for (int i = 0; i < nWidth*nHeight; i++)
	{
		image[2* i] = 20;
		image[2*i + 1] = 0;
	}
	bSaveRawFile("D:\\org.raw",image,nWidth,nHeight);
	//RGGB
	//R
	int x = 2;
	int y = 0;
	int index = (y*nWidth + x)*2;
	image[index] = 60;
	image[index + 4] = 60;		
	x = 0;
	y = 2;
	index = (y*nWidth + x)*2;
	image[index] = 60;	
	
	
	x = 20;
	y = 20;
	index = (y*nWidth + x)*2;
	image[index] = 60;
	image[index + 4] = 60;
	image[index - 4] = 60;
	index = ((y + 2)*nWidth + x)*2;
	image[index] = 60;
	

	x = 30;
	y = 30;
	index = (y*nWidth + x)*2;
	image[index] = 60;
	image[index + 4] = 60;
	image[index - 4] = 60;

	x = 0;
	y=100;
	index = (y*nWidth + x)*2;
	image[index] = 60;

	//Gr
	x = 1;
	y=100;
	index = (y*nWidth + x)*2;
	image[index] = 60;

	x = 9;
	y=100;
	index = (y*nWidth + x)*2;
	image[index] = 60;
	image[index + 4] = 60;
	image[index - 4] = 60;

	//B
	x = 1;
	y=301;
	index = (y*nWidth + x)*2;
	image[index] = 60;
	
	x = 9;
	y=301;
	index = (y*nWidth + x)*2;
	image[index] = 60;
	image[index + 4] = 60;
	image[index - 4] = 60;
	index = ((y + 2)*nWidth + x)*2;
	image[index] = 60;
	
	bSaveRawFile("D:\\bpt.raw",image,nWidth,nHeight);
	delete image;

}

void CBPTDemoDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	int nWidth = 3968;
	int nHeight = 2976;
	BYTE* image = new BYTE[nWidth*nHeight*2];
	memset(image,0,nWidth*nHeight*2);
	bReadRawFile(image,"D:\\bpt.raw",nWidth,nHeight);
	BadPixelTable badPixels;
	badPixels.BadPixelStack = new PixelInfo[2000];
	memset(badPixels.BadPixelStack,0,sizeof(PixelInfo)*2000);
	badPixels.DefectStackCnt = 2000;
	OutputDebugString("start...");
	BOOL ret = GetBPTTest(image,nWidth,nHeight,0,&badPixels,1,1);
	OutputDebugString("end....");
	delete badPixels.BadPixelStack;
	delete image;
}
