
// OFccmTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"

#include "OFccmTestDlg.h"
#include "afxdialogex.h"
#include "MyPropertySheet.h"
#include "TestItemManage.h"
#include "LoginDlg.h"
#include "CamCodeDlg.h"
#include "BaseSetDlg.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// COFccmTestDlg 对话框 

COFccmTestDlg::COFccmTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COFccmTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	for (int i=0;i<2;i++)
	{
		p_DisplayWnd[i]=new MyPictureCtrl(i);
		m_ButtonPanel[i] = new MyTestPanel(i);
		p_MyTestItemOptionDlg[i] = NULL;
	}
	
	
	p_MachineDlg = NULL;
	isCreateCtrl = 0;
	CamID = 0; 
	nStart = 0;
}

 
COFccmTestDlg::~COFccmTestDlg()
{
	for (int i=0;i<2;i++)
	{
		SAFE_DELETE(p_DisplayWnd[i]);
		SAFE_DELETE(m_ButtonPanel[i]);

		if(p_MyTestItemOptionDlg[i] != NULL)
		{
			if(p_MyTestItemOptionDlg[i]->m_hWnd != NULL)
			{
				p_MyTestItemOptionDlg[i]->DestroyWindow();
			}
			SAFE_DELETE(p_MyTestItemOptionDlg[i]);
		}
	}

	

	if(p_MachineDlg!=NULL)
	{
		if(p_MachineDlg->m_hWnd != NULL)
		{
			p_MachineDlg->DestroyWindow();
		}
	}
	SAFE_DELETE(p_MachineDlg); 
	SAFE_DELETE(pCcmCtrl);
}
void COFccmTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DisplayWnd, *p_DisplayWnd[0]);
	DDX_Control(pDX, IDC_DisplayWnd2, *p_DisplayWnd[1]);
	 
	 
	DDX_Control(pDX, IDC_LoggerEdit, LoggerWnd[0]);
	DDX_Control(pDX, IDC_LoggerEdit2, LoggerWnd[1]);
 

	DDX_Control(pDX, IDC_Info, m_Info[0]);
	DDX_Control(pDX, IDC_Info2, m_Info[1]);
 
}

BEGIN_MESSAGE_MAP(COFccmTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SHOWTEXTINFO, &COFccmTestDlg::ShowTextInfo)	  
	ON_MESSAGE(WM_UPDATEUI, &COFccmTestDlg::UpdateUI) 
	ON_MESSAGE(WM_CREATESETTINGDLG, &COFccmTestDlg::CreateSettingDlg)	  
	ON_MESSAGE(WM_DESTROYSETTINGDLG,&COFccmTestDlg::DestroySettingDlg)
    ON_MESSAGE(WM_CREATEMACHINESETTINGDLG, &COFccmTestDlg::CreateMachineSettingDlg)	
	 
	ON_COMMAND(ID_32780, &COFccmTestDlg::OnEnterDebugMode)
	ON_COMMAND(ID_32781, &COFccmTestDlg::OnExitDebugMode)
	ON_COMMAND(ID_32782, &COFccmTestDlg::OnOpenCamera)
	ON_COMMAND(ID_32812, &COFccmTestDlg::OnOpenCameraOnly)
	ON_COMMAND(ID_32783, &COFccmTestDlg::OnCloseCamera)
	ON_COMMAND(ID_32771, &COFccmTestDlg::OnBaseSetting)
	ON_COMMAND(ID_32772, &COFccmTestDlg::OnCameraSetting)
	ON_COMMAND(ID_32773, &COFccmTestDlg::OnTestItemManager)
	ON_COMMAND(ID_32774, &COFccmTestDlg::OnTestItemSetting)
	ON_COMMAND(ID_32778, &COFccmTestDlg::OnMachineSetting)
	 
	ON_COMMAND(ID_32786, &COFccmTestDlg::OnDebugIIC)
	ON_COMMAND(ID_32790, &COFccmTestDlg::OnEnterImgTest)
	ON_COMMAND(ID_32791, &COFccmTestDlg::OnLeaveImgTest)
	 
	ON_COMMAND(ID_32794, &COFccmTestDlg::OnSaveSingleCamImg)
	ON_COMMAND(ID_32795, &COFccmTestDlg::OnSaveDualCamImg)
	ON_COMMAND(ID_32787, &COFccmTestDlg::OnSelectCam0)
	ON_COMMAND(ID_32788, &COFccmTestDlg::OnSelectCam1)
	ON_COMMAND(ID_32799, &COFccmTestDlg::OnGoldenSampleRatio)
//	ON_COMMAND(ID_32804, &COFccmTestDlg::OnGoldenSampleRatio2)
//	ON_COMMAND(ID_32805, &COFccmTestDlg::OnGoldenSampleRatio3)
//	ON_COMMAND(ID_32806, &COFccmTestDlg::OnGoldenSampleRatio4)
	ON_COMMAND(ID_32808, &COFccmTestDlg::OnGoldenSampleRatio5)
	ON_COMMAND(ID_32809, &COFccmTestDlg::OnReadFuseID)
	 
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_strBarCode, &COFccmTestDlg::OnChangeStrbarcode)
	ON_EN_CHANGE(IDC_strBarCode2, &COFccmTestDlg::OnChangeStrbarcode2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

int COFccmTestDlg::Cam0_UImode =0;
int COFccmTestDlg::Cam1_UImode =0;
 

// COFccmTestDlg 消息处理程序

BOOL COFccmTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	MenuInit();

	//ShowWindow(SW_MAXIMIZE);
	

	m_ButtonPanel[0]->Create(IDD_TestPanel,this);
	m_ButtonPanel[0]->ShowWindow(SW_SHOW);


	m_ButtonPanel[1]->Create(IDD_TestPanel1,this);
	m_ButtonPanel[1]->ShowWindow(SW_SHOW);
	 
	if (!OFccmControl::globalset.bIsLoad)
	{ 
		OFccmControl::globalset.LoadSetting(); 
	}
	DebugMode = OFccmControl::globalset.DebugMode;
	TotalNum = OFccmControl::globalset.CamTotalNum;

	//互斥体//
	wchar_t TestSite[32];
	wsprintf(TestSite,_T("TestSite%d"),OFccmControl::globalset.siteNo);
	HANDLE m_Mutex;
	m_Mutex=CreateMutex(NULL,FALSE,TestSite); 
	if(GetLastError()==ERROR_ALREADY_EXISTS) 
	{
		AfxMessageBox(_T("程序已打开!"));
		exit(1);
	}

	SetWindowText(OFccmControl::globalset.StationName);
	SetWindowPosition(TotalNum);

	bOpen = FALSE;
	bGetCamCode = FALSE;
	hTestDlgWnd = this->GetSafeHwnd();
 
	
	for (int i=0;i<2;i++)
	{
		hDisplayWnd[i] = p_DisplayWnd[i]->GetSafeHwnd();
		hPanel[i] = m_ButtonPanel[i]->GetSafeHwnd();
		hLogger[i] = LoggerWnd[i].GetSafeHwnd();		 
	}

	pCcmCtrl = new OFccmControl( hTestDlgWnd, hDisplayWnd,hPanel, hLogger,TotalNum);

	//TotalNum =2;
	
	for (int i=0;i<2;i++)
	{
		if(!pCcmCtrl->Unlock(i))
		{
			exit(TRUE);
		}
	}
	
	 
	isCreateCtrl = 1;
	// TODO: 在此添加额外的初始化代码

	bOpen = TRUE;

	for (int i=TotalNum;i>0;i--)
	{
		CamID = i-1;	
		UpdateData(FALSE);		
		 
		UpdateUI(UI_DLGOPEN,CamID);

		switch(CamID)
		{
		case 0:
			OnSelectCam0();
			break;
		case 1:
			OnSelectCam1();
			break; 
		 
		} 
	}
	CamID = 0;
	UpdateData(FALSE);
	ClickTime=0;
	startTime=0;
	
	if(OFccmControl::globalset.bMachine && (OFccmControl::globalset.MachineName.GetLength()>1))
	{
		pCcmCtrl->pMachineCtrl = new MachineControl(pCcmCtrl, hTestDlgWnd, OFccmControl::globalset.machinePath, OFccmControl::globalset.MachineName);
	    pCcmCtrl->pMachineCtrl->LoadMachine();
		::PostMessage( this->m_hWnd,WM_CREATEMACHINESETTINGDLG, 0, 0);
	} 

	if(!OFccmControl::globalset.bOnLButtonUp)
	{
		for (int i=0;i<2;i++)
		{
			 p_DisplayWnd[i]->ModifyStyle(SS_NOTIFY, 0);
		}
	}

	pCcmCtrl->LogMes();
	pCcmCtrl->LEDPortInitia(this);
	lockUsb();
	char key[512]={0};
	sprintf(key,"Command_%d",pCcmCtrl->GetSiteNo());
	RegSetKey("", key, "");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COFccmTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COFccmTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		if (bOpen)
		{
			for (int i=0;i<TotalNum;i++)
			{
				/*if (pCcmCtrl->isStandby(i))
				{
				pCcmCtrl->ResetBackGround(i);
				}*/
			}

		} 
		
	}
    SetTimer(0,500,NULL);
	CDialogEx::OnPaint();
//	return CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COFccmTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL COFccmTestDlg::SetCamCode()
{
	UpdateData(TRUE); 
	CString strCam(_T("0")) ;
	if(bGetCamCode) return TRUE;

	bGetCamCode = TRUE;
	/*GetDlgItem(IDC_strCam)->GetWindowText(strCam);*/
	if (OFccmControl::globalset.bUsedCamCode)
	{
		//AfxMessageBox(_T("请输入模组编码"));
		CamCodeDlg m_CamCodeDlg;
		m_CamCodeDlg.DoModal();
		strCam = m_CamCodeDlg.CamCode;
		if(strCam.GetLength()<1)
		{ 
			return FALSE;
		} 
	}
	
	CString str;
	str.Format(_T("Cam %d Code"),CamID);
	pCcmCtrl->Ctrl_AddToLog(str + strCam,CamID); 
	pCcmCtrl->SetCamCode(strCam);
	return TRUE;
} 

void COFccmTestDlg::Binding(int CamID)
{
	CString temp = _T("");
	CString temp1 = _T("");
	CString temp2 =_T("");		

	CString Mess(pCcmCtrl->ReciveData);
	temp = Mess.Mid(Mess.Find(_T(":"))+1);
	temp1 = temp.Left(temp.Find(_T(":")));
	temp2 = temp.Mid(temp.Find(_T(":"))+1);
	if(temp1==_T("")||temp2==_T(""))
	{
		pCcmCtrl->Ctrl_Addlog( CamID,_T("Handler绑定信息错误!"),COLOR_RED,220);
		pCcmCtrl->Ctrl_DrawingToMenDC(CamID,RGB(200,0,0));
		pCcmCtrl->Ctrl_DrawingToMenDC(CamID ,RGB(0,0,0),_T("Handler绑定信息错误!"),150,4,2);
		pCcmCtrl->Ctrl_ShowMemDC(CamID);
		pCcmCtrl->Cam[CamID].camSocket->SendInfo(_T("H04010001"));
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_strRuncard);
		pWnd->SetWindowText(temp1);
		pWnd = GetDlgItem(IDC_strBarCode);
		pWnd->SetWindowText(temp2);
		pCcmCtrl->Ctrl_Addlog( CamID,_T("获取绑定信息成功!"),COLOR_BLUE,220);
		pCcmCtrl->Ctrl_DrawingToMenDC(CamID,RGB(0,200,0));
		pCcmCtrl->Ctrl_DrawingToMenDC(CamID ,RGB(0,0,0),_T("获取绑定信息成功!"),150,4,2);
		pCcmCtrl->Ctrl_ShowMemDC(CamID);
		pCcmCtrl->Cam[CamID].camSocket->SendInfo(_T("H04010000"));						
	}					
}

void COFccmTestDlg::OnPlay(int CamID)
{ 
	if (pCcmCtrl->isStandby(CamID))
	{ 
		pCcmCtrl->Cam[CamID].bState.bCameraStandby = FALSE;
		startTime = GetTickCount();
		if(!SetCamCode()) return;		

		pCcmCtrl->Ctrl_ClearLog(CamID);
		pCcmCtrl->Cam[CamID].errorInfo = L"TEST FAIL!";      //初始化error info,用于做最终双cam结果的判断，注意"!"号

		pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_INIT_STATE;
				
		pCcmCtrl->pDevice->SetDevName(pCcmCtrl->Cam[CamID].pConfig->GetGeneralTab()->DevName); 
		if(pCcmCtrl->pDevice->GetKitTypeDeviceInfo(CamID)<1)
		{ 	
			pCcmCtrl->Cam[CamID].NGTestItemID = -1;
			pCcmCtrl->Cam[CamID].pTestCtrl->bTestResult = FALSE;
			pCcmCtrl->Ctrl_Addlog(CamID,_T("度信没有连接"),COLOR_RED,200);
			pCcmCtrl->OpenFailProc(CamID);
			pCcmCtrl->Ctrl_SetCamErrorInfo(CamID,_T("度信没有连接"));
			return; 
		}
		PostMessage(WM_UPDATEUI, UI_START_STOP);
		Sleep(300);
		pCcmCtrl->Ctrl_Addlog(CamID,_T("UPDateUI"),COLOR_RED,200);
		UpdateData(FALSE);
        CString str;
	    if(CamID==0)
		{
			OnSelectCam0();
			str.Format(_T("Cam[0].TestItemNum = %d"),pCcmCtrl->Cam[0].TestItemNum);
			pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

			str.Format(_T("Cam[0].TestItemPass = %d"),pCcmCtrl->Cam[0].TestItemPass);
			pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
		}
		else  if(CamID==1)
		{
			OnSelectCam1();
			str.Format(_T("Cam[0].TestItemNum = %d"),pCcmCtrl->Cam[1].TestItemNum);
			pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

			str.Format(_T("Cam[0].TestItemPass = %d"),pCcmCtrl->Cam[1].TestItemPass);
			pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
		} 
		
		str.Format(_T("Cam %d DlgOpen"),CamID);
		pCcmCtrl->Ctrl_AddToLog(str ,CamID); 
		pCcmCtrl->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
		pCcmCtrl->OpenCameraMsg(CamID);		 
	} 
	else
	{
		//OnCloseCamera();
	}
	 
}


 
void COFccmTestDlg::SetWindowPosition(int Num)
{
	switch (Num)
	{
	case 1:
		 
		if (OFccmControl::globalset.siteNo!=0)
		{
			if(OFccmControl::globalset.bSite==0)//独立窗口
			  SetWindowPosition1();
			else
              SetWindowPosition1_1();
		}
		else
		{
			SetWindowPosition1_0();
		}		
		break;
	case 2:
		if (OFccmControl::globalset.siteNo!=0)
		{
			SetWindowPosition2_2();
		}
		else
		{
			SetWindowPosition2();
		}		
		break;
	  
	}
}
void COFccmTestDlg::SetWindowPosition1()
{
	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN)/2;
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN)-20;

	int StartX=0;
	int StartY=0;
	int iFullScrn_width=0;
	int iFullScrn_Height=0; 

	CRect MainWindowRect;  

	int Xpos=0;
	int Ypos=0;

	if(OFccmControl::globalset.siteNo == 2)
		StartX = iFullScrn_cx;

	//CFont FontTxt;

	//FontTxt.CreateFont (20, // FONT SIZE
	//	10, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
	//	OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
	//	VARIABLE_PITCH|FF_SWISS, _T("Cambria"));

	iFullScrn_width=iFullScrn_cx;
	iFullScrn_Height= (int)(iFullScrn_cy*0.95);

	this->SetWindowPos(NULL, StartX, StartY, iFullScrn_width, iFullScrn_Height,  SWP_NOZORDER| SWP_SHOWWINDOW);//

	this->GetClientRect(&MainWindowRect);



	//int videoH = MainWindowRect.Height()-60;
	//int videoW = videoH*4/3 /*.Width()*/;

	
	int videoW = iFullScrn_width/3*2; /*.Width()*/;
	int videoH = videoW/4*3;


	p_DisplayWnd[0]->SetWindowPos(NULL,
		0,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);	
	p_DisplayWnd[1]->ShowWindow(HIDE_WINDOW);
	 

	int Width = videoW;
	int Height = videoH;

	m_Info[0].SetWindowPos(NULL,
		5,
		Height,
		Width,
		15,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Info[1].ShowWindow(HIDE_WINDOW);
	 

	int PanelWidth = iFullScrn_width-videoW;
	int PanelHeight = iFullScrn_Height;

	m_ButtonPanel[0]->MoveWindow(videoW,0,PanelWidth,PanelHeight);

	m_ButtonPanel[1]->ShowWindow(HIDE_WINDOW); 
	 


	LoggerWnd[0].SetWindowPos(NULL,
		0,
		videoH+15,
		videoW,
		(iFullScrn_Height-videoH-65)*2/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	LoggerWnd[1].ShowWindow(HIDE_WINDOW);

	GetDlgItem(IDC_TESTInfo1)->SetWindowPos(NULL,
		0,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3,
		videoW*2/3,
		(iFullScrn_Height-videoH-65)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_TESTInfo2)->ShowWindow(HIDE_WINDOW);

	GetDlgItem(IDC_Runcard)->SetWindowPos(NULL,
		videoW*2/3,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3+20,
		50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_BarCode)->SetWindowPos(NULL,
		videoW*2/3,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3+50,
		50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strRuncard)->SetWindowPos(NULL,
		videoW*2/3+50,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3+20,
		videoW/3-50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strBarCode)->SetWindowPos(NULL,
		videoW*2/3+50,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3+50,
		videoW/3-50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	GetDlgItem(IDC_Runcard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode2)->ShowWindow(HIDE_WINDOW);
	  
}
void COFccmTestDlg::SetWindowPosition1_1()
{
	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN)/4;
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN)-20;

	int StartX=0;
	int StartY=0;
	int iFullScrn_width=0;
	int iFullScrn_Height=0; 

	CRect MainWindowRect;  

	int Xpos=0;
	int Ypos=0;

	if(OFccmControl::globalset.siteNo == 2)
		StartX = iFullScrn_cx*1;
	if(OFccmControl::globalset.siteNo == 3)
		StartX = iFullScrn_cx*2;
	if(OFccmControl::globalset.siteNo == 4)
		StartX = iFullScrn_cx*3;

	//CFont FontTxt;

	//FontTxt.CreateFont (20, // FONT SIZE
	//	10, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
	//	OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
	//	VARIABLE_PITCH|FF_SWISS, _T("Cambria"));

	iFullScrn_width=iFullScrn_cx;
	iFullScrn_Height= (int)(iFullScrn_cy*0.95);

	this->SetWindowPos(NULL, StartX, StartY, iFullScrn_width, iFullScrn_Height,  SWP_NOZORDER| SWP_SHOWWINDOW);//

	this->GetClientRect(&MainWindowRect);



	//int videoH = MainWindowRect.Height()-60;
	//int videoW = videoH*4/3 /*.Width()*/;


	int videoW = iFullScrn_width/3*2; /*.Width()*/;
	int videoH = videoW/4*3;


	p_DisplayWnd[0]->SetWindowPos(NULL,
		0,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);	
	p_DisplayWnd[1]->ShowWindow(HIDE_WINDOW);


	int Width = videoW;
	int Height = videoH;

	m_Info[0].SetWindowPos(NULL,
		5,
		Height,
		Width,
		15,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Info[1].ShowWindow(HIDE_WINDOW);


	int PanelWidth = iFullScrn_width-videoW;
	int PanelHeight = iFullScrn_Height;

	m_ButtonPanel[0]->MoveWindow(videoW,0,PanelWidth,PanelHeight);

	m_ButtonPanel[1]->ShowWindow(HIDE_WINDOW); 



	LoggerWnd[0].SetWindowPos(NULL,
		0,
		videoH+15,
		videoW,
		(iFullScrn_Height-videoH-65)*2/3-50,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	LoggerWnd[1].ShowWindow(HIDE_WINDOW);

	////
	GetDlgItem(IDC_Runcard)->SetWindowPos(NULL,
		0,videoH+15+(iFullScrn_Height-videoH-65)*2/3-45,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_BarCode)->SetWindowPos(NULL,
		0,videoH+15+(iFullScrn_Height-videoH-65)*2/3-25,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strRuncard)->SetWindowPos(NULL,
		50,videoH+15+(iFullScrn_Height-videoH-65)*2/3-45,
		videoW-20,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strBarCode)->SetWindowPos(NULL,
		50,videoH+15+(iFullScrn_Height-videoH-65)*2/3-25,
		videoW-20,20,SWP_NOZORDER | SWP_SHOWWINDOW);

	GetDlgItem(IDC_Runcard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode2)->ShowWindow(HIDE_WINDOW);
	////
	
	GetDlgItem(IDC_TESTInfo1)->SetWindowPos(NULL,
		0,
		videoH+15+(iFullScrn_Height-videoH-65)*2/3,
		videoW,
		(iFullScrn_Height-videoH-65)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_TESTInfo2)->ShowWindow(HIDE_WINDOW);

	

}

void COFccmTestDlg::SetWindowPosition1_0()
{
	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN);
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN)-20;

	int StartX=0;
	int StartY=0;
	int iFullScrn_width=0;
	int iFullScrn_Height=0; 

	CRect MainWindowRect;  

	int Xpos=0;
	int Ypos=0;

 

	//CFont FontTxt;

	//FontTxt.CreateFont (20, // FONT SIZE
	//	10, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
	//	OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
	//	VARIABLE_PITCH|FF_SWISS, _T("Cambria"));

	iFullScrn_width=iFullScrn_cx;
	iFullScrn_Height= (int)(iFullScrn_cy*0.95);

	this->SetWindowPos(NULL, StartX, StartY, iFullScrn_width, iFullScrn_Height,  SWP_NOZORDER| SWP_SHOWWINDOW);//

	this->GetClientRect(&MainWindowRect);
	 
	int videoH = MainWindowRect.Height()-30;
	int videoW = videoH*4/3 /*.Width()*/;
	if (videoW>(iFullScrn_width*3/4))
	{
		videoW = iFullScrn_width*3/4;
		videoH = videoW*3/4;
	}


	//int videoW = iFullScrn_width/3*2; /*.Width()*/;
	//int videoH = videoW/4*3;


	p_DisplayWnd[0]->SetWindowPos(NULL,
		0,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);	
	p_DisplayWnd[1]->ShowWindow(HIDE_WINDOW);


	int Width = videoW;
	int Height = videoH;

	m_Info[0].SetWindowPos(NULL,
		0,
		Height,
		Width,
		15,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Info[1].ShowWindow(HIDE_WINDOW);


	int PanelWidth = iFullScrn_width-videoW;
	int PanelHeight = iFullScrn_Height/5*3;

	m_ButtonPanel[0]->MoveWindow(videoW,0,PanelWidth,PanelHeight);

	m_ButtonPanel[1]->ShowWindow(HIDE_WINDOW); 



	LoggerWnd[0].SetWindowPos(NULL,
		videoW,
		PanelHeight,
		PanelWidth-10,
		(iFullScrn_Height-PanelHeight-55)*2/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	LoggerWnd[1].ShowWindow(HIDE_WINDOW);

	GetDlgItem(IDC_TESTInfo1)->SetWindowPos(NULL,
		videoW,
		PanelHeight+(iFullScrn_Height-PanelHeight-55)*2/3,
		PanelWidth-10,
		(iFullScrn_Height-PanelHeight-55)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_TESTInfo2)->ShowWindow(HIDE_WINDOW);


	GetDlgItem(IDC_Runcard)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_Runcard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode2)->ShowWindow(HIDE_WINDOW);


}

void COFccmTestDlg::SetWindowPosition2()
{

	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN);
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN);

	int StartX=0;
	int StartY=0;
	int iFullScrn_width=0;
	int iFullScrn_Height=0; 

	CRect MainWindowRect;  

	int Xpos=0;
	int Ypos=0;

	CFont FontTxt;

	FontTxt.CreateFont (20, // FONT SIZE
		10, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
		VARIABLE_PITCH|FF_SWISS, _T("Cambria"));

	SetFont(&FontTxt);
	 

	iFullScrn_width=iFullScrn_cx;
	iFullScrn_Height= (int)(iFullScrn_cy*0.9);

	this->SetWindowPos(NULL, StartX, StartY, iFullScrn_width, iFullScrn_Height, SWP_NOZORDER | SWP_SHOWWINDOW);

	this->GetClientRect(&MainWindowRect);


	int videoW = MainWindowRect.Width()/3-5;
	int videoH = videoW*3/4;


	p_DisplayWnd[0]->SetWindowPos(NULL,
		videoW/2,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);	

	p_DisplayWnd[1]->SetWindowPos(NULL,
		videoW*2+5,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);
	 
	m_Info[0].SetWindowPos(NULL,
		videoW/2,
		videoH,
		videoW,
		40,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Info[1].SetWindowPos(NULL,
		videoW*2+5,
		videoH,
		videoW,
		40,
		SWP_NOZORDER | SWP_SHOWWINDOW);	 
	 

	int PanelWidth = videoW/2-5;	
	int PanelHeight = iFullScrn_Height ;
	m_ButtonPanel[0]->MoveWindow(0,0,PanelWidth,PanelHeight);
	m_ButtonPanel[1]->MoveWindow(videoW*3/2+5,0,PanelWidth,PanelHeight);	 
 

	int LoggerWndHeight = iFullScrn_Height -(videoH+40) ; 
	int LoggerWndWidth = videoW;
	LoggerWnd[0].SetWindowPos(NULL,
		videoW/2,
		videoH+50,
		LoggerWndWidth,
		(LoggerWndHeight-55)*2/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);


	LoggerWnd[1].SetWindowPos(NULL,
		videoW*2+5,
		videoH+50,
		LoggerWndWidth,
		(LoggerWndHeight-55)*2/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	GetDlgItem(IDC_TESTInfo1)->SetWindowPos(NULL,
		videoW/2,
		videoH+50+(LoggerWndHeight-55)*2/3,
		LoggerWndWidth,
		(LoggerWndHeight-55)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_TESTInfo2)->SetWindowPos(NULL,
		videoW*2+5,
		videoH+50+(LoggerWndHeight-55)*2/3,
		LoggerWndWidth,
		(LoggerWndHeight-55)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	 
	GetDlgItem(IDC_Runcard)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_Runcard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_BarCode2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strRuncard2)->ShowWindow(HIDE_WINDOW);
	GetDlgItem(IDC_strBarCode2)->ShowWindow(HIDE_WINDOW);
	 
}

void COFccmTestDlg::SetWindowPosition2_2()
{

	int iFullScrn_cx = GetSystemMetrics(SM_CXSCREEN)/2;
	int iFullScrn_cy = GetSystemMetrics(SM_CYSCREEN);

	int StartX=0;
	int StartY=0;
	int iFullScrn_width=0;
	int iFullScrn_Height=0; 

	CRect MainWindowRect;  

	int Xpos=0;
	int Ypos=0;

	CFont FontTxt;

	FontTxt.CreateFont (20, // FONT SIZE
		10, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
		VARIABLE_PITCH|FF_SWISS, _T("Cambria"));

	SetFont(&FontTxt);

	if(OFccmControl::globalset.siteNo==2)
	{
		StartX = iFullScrn_cx;
	}


	iFullScrn_width=iFullScrn_cx;
	iFullScrn_Height= (int)(iFullScrn_cy*0.9);
	
	this->SetWindowPos(NULL, StartX, StartY, iFullScrn_width, iFullScrn_Height, SWP_NOZORDER | SWP_SHOWWINDOW);

	this->GetClientRect(&MainWindowRect);


	int videoW = MainWindowRect.Width()/2;
	int videoH = videoW*3/4;

	if(videoW%2!=0)
	{
		videoW -=1;
	}
	if(videoH%2!=0)
	{
		videoH -=1;
	}



	p_DisplayWnd[0]->SetWindowPos(NULL,
		0,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);	

	p_DisplayWnd[1]->SetWindowPos(NULL,
		videoW,
		0,
		videoW,//MainWindowRect.right/8*6
		videoH,//
		SWP_NOZORDER | SWP_SHOWWINDOW);
	 
	m_Info[0].SetWindowPos(NULL,
		0,
		videoH,
		videoW,
		40,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	m_Info[1].SetWindowPos(NULL,
		videoW,
		videoH,
		videoW,
		40,
		SWP_NOZORDER | SWP_SHOWWINDOW);	 
	 

	int PanelWidth = videoW/2;	
	int PanelHeight = iFullScrn_Height-videoH-40 ;
	int PanelWidth_x = 0;	
	int PanelHeight_y = videoH+40 ;
	m_ButtonPanel[0]->MoveWindow(0,PanelHeight_y,PanelWidth,PanelHeight);
	m_ButtonPanel[1]->MoveWindow(videoW,PanelHeight_y,PanelWidth,PanelHeight);	 
	 

	int LoggerWndHeight = PanelHeight; 
	int LoggerWndWidth = videoW/2;
	int LoggerWnd_x = videoW/2;	
	int LoggerWnd_y = videoH+40 ;
	LoggerWnd[0].SetWindowPos(NULL,
		LoggerWnd_x,
		LoggerWnd_y,
		LoggerWndWidth,
		(LoggerWndHeight-55)*2/3-50,
		SWP_NOZORDER | SWP_SHOWWINDOW);


	LoggerWnd[1].SetWindowPos(NULL,
		LoggerWnd_x*3,
		LoggerWnd_y,
		LoggerWndWidth,
		(LoggerWndHeight-55)*2/3-50,
		SWP_NOZORDER | SWP_SHOWWINDOW);

	////
	GetDlgItem(IDC_Runcard)->SetWindowPos(NULL,
		LoggerWnd_x,LoggerWnd_y+(LoggerWndHeight-55)*2/3-45,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_BarCode)->SetWindowPos(NULL,
		LoggerWnd_x,LoggerWnd_y+(LoggerWndHeight-55)*2/3-25,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strRuncard)->SetWindowPos(NULL,
		LoggerWnd_x+50,LoggerWnd_y+(LoggerWndHeight-55)*2/3-45,
		LoggerWndWidth-50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strBarCode)->SetWindowPos(NULL,
		LoggerWnd_x+50,LoggerWnd_y+(LoggerWndHeight-55)*2/3-25,
		LoggerWndWidth-50,20,SWP_NOZORDER | SWP_SHOWWINDOW);

	GetDlgItem(IDC_Runcard2)->SetWindowPos(NULL,
		LoggerWnd_x*3,LoggerWnd_y+(LoggerWndHeight-55)*2/3-45,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_BarCode2)->SetWindowPos(NULL,
		LoggerWnd_x*3,LoggerWnd_y+(LoggerWndHeight-55)*2/3-25,
		50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strRuncard2)->SetWindowPos(NULL,
		LoggerWnd_x*3+50,LoggerWnd_y+(LoggerWndHeight-55)*2/3-45,
		LoggerWndWidth-50,20,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	GetDlgItem(IDC_strBarCode2)->SetWindowPos(NULL,
		LoggerWnd_x*3+50,LoggerWnd_y+(LoggerWndHeight-55)*2/3-25,
		LoggerWndWidth-50,20,SWP_NOZORDER | SWP_SHOWWINDOW);
	////
	GetDlgItem(IDC_TESTInfo1)->SetWindowPos(NULL,
		LoggerWnd_x,
		LoggerWnd_y+(LoggerWndHeight-55)*2/3,
		LoggerWndWidth,
		(LoggerWndHeight-55)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);


	GetDlgItem(IDC_TESTInfo2)->SetWindowPos(NULL,
		LoggerWnd_x*3,
		LoggerWnd_y+(LoggerWndHeight-55)*2/3,
		LoggerWndWidth,
		(LoggerWndHeight-55)/3,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	 
}

 
 
LRESULT COFccmTestDlg::ShowTextInfo(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR str = reinterpret_cast<LPCTSTR> (wParam);
	int Camid = (int)(lParam);
	m_Info[Camid].SetWindowText(str);
	return 0;
}

LRESULT COFccmTestDlg::UpdateUI(WPARAM wParam, LPARAM lParam)
{

	int type = static_cast<int> (wParam);
	int Camid = static_cast<int> (lParam);

	if (type == -1)
	{
		switch(Camid)
		{
		case 0:
			type =  Cam0_UImode ;
			break;
		case 1:
			type =  Cam1_UImode ;
			break;	 			 
		}	
	}
	else
	{
		switch(Camid)
		{
		case 0:
			Cam0_UImode =  type ;
			break;
		case 1:
			Cam1_UImode =  type ;
			break;		 
		}

	}


	switch(type)
	{
	case  UI_DLGOPEN: //程序初始设置
		{  
		    m_ButtonPanel[Camid]->ResetAllButtonImage();
            m_Info[Camid].SetWindowTextW(_T(""));
			m_ButtonPanel[Camid]->EnableButton(FALSE); 
			 
			break;
		}	 
	case UI_START_STOP:
		{
			//立即禁用的按钮等消息通知。
		  
			m_ButtonPanel[Camid]->EnableButton(FALSE); 
			m_Info[Camid].SetWindowText(_T(""));
			break;
		}
	case UI_PLAY_SUCESS: //启动成功
		{
			m_ButtonPanel[Camid]->ResetAllButtonImage();
			if(DebugMode==1)
			{
				m_ButtonPanel[Camid]->EnableButton(TRUE);

			}
			
			m_Info[Camid].SetWindowText(_T(""));
  
			break;
		}
	case UI_PLAY_FAILED: //启动失败
		{
			 bGetCamCode = FALSE;
			m_Info[Camid].SetWindowText(_T(""));
			if (OFccmControl::globalset.DeviceTypeSel==_T("UH920")&&OFccmControl::globalset.CamType==_T("Dual"))
			{
				CamID = 0;
				UpdateData(FALSE);
				 
			}
			m_ButtonPanel[Camid]->EnableButton(FALSE);
			 
			break;
		}
	case UI_STOP_SUCESS: //停止成功
		{ 
			p_DisplayWnd[Camid]->Invalidate(FALSE); //刷新控件画面			 
			m_ButtonPanel[Camid]->EnableButton(FALSE);
			pCcmCtrl->ResetBackGround(Camid);
		//	m_ButtonPanel[Camid]->ResetAllButtonImage();
        //  m_ButtonPanel[Camid]->EnableButton(FALSE);
		//  m_Info[Camid].SetWindowTextW(_T(""));
			 bGetCamCode = FALSE;
			break;
		}
	case UI_ENTER_OFFLINE: //进入OFFLINE模式
		{ 
			p_DisplayWnd[CamID]->Invalidate(FALSE); //刷新控件画面 
			m_ButtonPanel[Camid]->EnableButton(TRUE);
			m_Info[Camid].SetWindowText(_T(""));
			break;
		}
	case UI_EXIT_OFFLINE: //停止成功
		{ 
			m_ButtonPanel[Camid]->EnableButton(FALSE);
			m_ButtonPanel[Camid]->ResetAllButtonImage(); 
			p_DisplayWnd[CamID]->Invalidate(FALSE); //刷新控件画面
		 
			m_Info[Camid].SetWindowText(_T(""));
			break;

		}
	default:
		break;

	}
	if(type==-1)
	{
		m_ButtonPanel[Camid]->EnableButton(FALSE);
	}	
	return 0;
}

void COFccmTestDlg::SaveUIStatus(void)
{
	map<HWND,BOOL>::iterator it;
	for (it = m_WndStatus_Map.begin(); it != m_WndStatus_Map.end(); it++)
	{
		it->second = ::IsWindowEnabled(it->first);
	}
}

void COFccmTestDlg::InitBtnStatusMap(void)
{

	/*m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnTestItemOption.m_hWnd, FALSE));	 
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnI2CDebug.m_hWnd, FALSE));	  
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnPlay.m_hWnd, FALSE));
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnPause.m_hWnd, FALSE));
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnSaveImage.m_hWnd, FALSE));
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnTestItems.m_hWnd, FALSE));
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnOffLine.m_hWnd, FALSE));
	m_WndStatus_Map.insert(pair<HWND,BOOL>(m_btnCameraSetting.m_hWnd, FALSE));*/

}



BOOL COFccmTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类 
	if( pMsg->hwnd != this->m_hWnd)  return CDialog::PreTranslateMessage(pMsg);
	UINT boMessage=pMsg->message;
	switch(boMessage)
	{		
		//右键
	case WM_RBUTTONDOWN:
		if(OFccmControl::globalset.bFocus && pCcmCtrl->Cam[CamID].bFocusEnd == FALSE)
		{
			pCcmCtrl->Cam[CamID].bFocusEnd = TRUE;
		}
		else
			OnCloseCamera();

		break;
		//双击
	case WM_LBUTTONDBLCLK:
		OnOpenCamera();
		break;

	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{ 
		case  VK_ESCAPE:
			OnOpenCamera();
			break;

		/*case  'P':	
			TRACE("p dwon\n");
			break;*/
		//case  'S':

		//case 's':  //存储图片
		//	{
		//		//if(m_btnSaveImage.IsWindowEnabled())
		//		{
		//			OnBnClickedSaveimage();
		//		}
		//		return TRUE;
		//	}
		//	break;
		case VK_SPACE:
			//同右键
			OnCloseCamera();
			TRACE("space down\n");
			break;
		default:
			break;

		}
		break;

	default: 
		break;

	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL COFccmTestDlg::OnKey_CtrlNum(DWORD wParam)
{
	int ID = wParam - 0x30;
	MyTestItem* item = pCcmCtrl->Cam[CamID].pTestCtrl->GetInspectionItem_FromID(ID);
	if (item == NULL)	return FALSE;

	HWND hWnd = item->pItem->GetOptionDialogHandle();
	//模拟按钮效果
	::SendMessage(hWnd,WM_INSPCETIONDLL_KEY_CTRLNUM,0,0);
	return TRUE;
}

int COFccmTestDlg::OnArrowKey(int nDirection)
{
	int ID = pCcmCtrl->Cam[CamID].pTestCtrl->GetLastID();
	switch(nDirection)
	{
	case VK_UP:
		ID = ID - 1;
		break;
	case VK_DOWN:
		ID = ID + 1;
		break;
	}
	return OnKey_Num(ID + 0x30);
}

void COFccmTestDlg::RecoverOldUIStatus(void)
{
	map<HWND,BOOL>::iterator it;
	for (it = m_WndStatus_Map.begin(); it != m_WndStatus_Map.end(); it++)
	{
		::EnableWindow(it->first,it->second);
	}
}

BOOL COFccmTestDlg::OnKey_Num(DWORD wParam)
{
	int ID = wParam - 0x30;
	MyTestItem* item = pCcmCtrl->Cam[CamID].pTestCtrl->GetInspectionItem_FromID(ID);
	if (item == NULL)	return FALSE;

	HWND hBtn = item->hButton;
	if (hBtn == NULL) return FALSE;

	//模拟按钮效果
	::SendMessage(hBtn,WM_LBUTTONDOWN,0,0);
	Sleep(100);
	::SendMessage(hBtn,WM_LBUTTONUP,0,0);  

	return TRUE;
}

 

LRESULT COFccmTestDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	///////////////wanjiang2//////LightSource_En10ge//////
	char RevTemp[512]={0};
	CString RevInfo;
	int  len=0;
	int CamId = 0;
	switch(message)
	{

	case WM_COMMAND:
		{
			if(OFccmControl::globalset.bAutoMachine == FALSE)
			{
				UINT32 u32Msg = HIWORD(wParam);
				UINT32 u32CtrlId = LOWORD(wParam);
				if ((IDC_strBarCode == u32CtrlId) && (EN_KILLFOCUS == u32Msg))
				{
					CString strText = _T("");
					CWnd* pWnd = GetDlgItem(IDC_strBarCode);
					pWnd->GetWindowText(strText);
					if (strText.IsEmpty())
					{
						pWnd->SetFocus();
					}
				}
			}
			
		}
		break;

	case WM_UPDATEMSG:
		{ 
			CamId=(int)lParam&0x01;
			
			pCcmCtrl->Ctrl_Addlog(CamId,wParam,COLOR_BLUE,200);
			switch(wParam)
			{
			case WM_SOCKET_BINDING:	
				Binding(CamId);
				break;
			case WM_SOCKET_CLOSE:
				pCcmCtrl->Ctrl_Addlog( CamId,_T("Socket被关闭"),COLOR_BLUE,220);
				break;
			case WM_SOCKET_UPDATE:
				if (OFccmControl::globalset.CamType == L"Dual")
				{
					for (CamId = 0; CamId<2; CamId++)
					{
						if ((pCcmCtrl->Cam[0].pTestCtrl->bTestResult == FALSE)
							|| (pCcmCtrl->Cam[1].pTestCtrl->bTestResult == FALSE))   //主副摄有一个NG即为NG，避免出现一个NG一个OK的情况
						{
							pCcmCtrl->Ctrl_DrawingToMenDC(CamId,RGB(200,0,0));
						}
						else
						{
							pCcmCtrl->Ctrl_DrawingToMenDC(CamId,RGB(0,100,0));
						}
						pCcmCtrl->Ctrl_DrawingToMenDC(CamId ,RGB(0,0,0),_T("请换下一颗产品进行测试!"),150,4,2);
						pCcmCtrl->Ctrl_ShowMemDC(CamId);
					}
				}
				else
				{
					if (pCcmCtrl->Cam[CamID].TestItemNum != pCcmCtrl->Cam[CamID].TestItemPass)//pCcmCtrl->Cam[CamId].pTestCtrl->bTestResult == FALSE)
					{
						pCcmCtrl->Ctrl_DrawingToMenDC(CamId,RGB(200,0,0));
					}
					else
					{
						pCcmCtrl->Ctrl_DrawingToMenDC(CamId,RGB(0,100,0));
					}
					pCcmCtrl->Ctrl_DrawingToMenDC(CamId ,RGB(0,0,0),_T("请换下一颗产品进行测试!"),150,4,2);
					pCcmCtrl->Ctrl_ShowMemDC(CamId);
				}
				//Invalidate(TRUE); 

				if (OFccmControl::globalset.bAutoMachine == FALSE)
				{
					Sleep(100);
					if (pCcmCtrl->Cam[CamId].MesSelect == 1)
					{
						AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),TRUE); 	
						::SetForegroundWindow(m_hWnd);

						if (CamId==0)
						{
							GetDlgItem(IDC_strBarCode)->SetWindowText(NULL);
							GetDlgItem(IDC_strBarCode)->SetFocus();
						}
						else
						{
							GetDlgItem(IDC_strBarCode)->SetWindowText(NULL);
							GetDlgItem(IDC_strBarCode)->SetFocus();
						}

						AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),FALSE);	
						fSleep(1000);
					}
				}
				
				break;
			case  WM_SOCKET_CLOSECAMERAFINAL:
				{
					pCcmCtrl->CloseCamera_Final(CamId,1);

					if (OFccmControl::globalset.bAutoMachine == FALSE)
					{
						if (pCcmCtrl->Cam[CamId].MesSelect == 1)
						{
							AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),TRUE); 	
							::SetForegroundWindow(m_hWnd);

							if (CamId==0)
							{
								GetDlgItem(IDC_strBarCode)->SetWindowText(L"");
								GetDlgItem(IDC_strBarCode)->SetFocus();
							}
							else
							{
								GetDlgItem(IDC_strBarCode2)->SetWindowText(L"");
								GetDlgItem(IDC_strBarCode2)->SetFocus();
							}

							AttachThreadInput(GetWindowThreadProcessId(::GetForegroundWindow(),NULL), GetCurrentThreadId(),FALSE);	

						}
					}
					
				}
				break;
			case WM_SOCKET_OPENCAMERA:
				
				//pCcmCtrl->Ctrl_AddToLog(_T("Socket OpenCamera"),CamId); 
				{

					pCcmCtrl->Ctrl_Addlog( CamId,_T("Socket OpenCamera..."),COLOR_BLUE,220);

					if(OFccmControl::globalset.CamType==L"Dual" && pCcmCtrl->Cam[1].pTestCtrl->m_TestMode==TESTMODE_AATEST)
					{
						if(!pCcmCtrl->isStandby(0))
						{
							pCcmCtrl->CloseCameraMsg(0);
							Sleep(500);
						}
					
						if(!pCcmCtrl->isStandby(1))
						{
							pCcmCtrl->CloseCameraMsg(1);
							Sleep(500);
						}
						OnPlay(0);
						Sleep(300);
						OnPlay(1);
					}//机器模式，分别由Machine命令控制cam0，和cam1点亮，避免之前重复执行点亮的问题
					else if(OFccmControl::globalset.CamType==L"Dual" && pCcmCtrl->Cam[CamId].pTestCtrl->m_TestMode==TESTMODE_MACHINETEST)  
					{
						if(pCcmCtrl->isStandby(CamId))
						{
							OnPlay(CamId);
						}
					}
					else
					{ 
						if(pCcmCtrl->Ctrl_GetCamType()==1)//Dual
						{
							if(pCcmCtrl->isStandby(0))
							{
							  OnPlay(0);
							}
							Sleep(100);
							if(pCcmCtrl->isStandby(1))
							{
								OnPlay(1);
							}
						}
						else
						{
							OnPlay(CamId);
							Sleep(100);
						}
					}
					 
				} 
				break;
			case WM_SOCKET_TESTPASS:
				//关闭模组显示PASS
				{
					//pCcmCtrl->Ctrl_Addlog(CamID,_T("Mark2"),COLOR_BLUE);
					pCcmCtrl->Ctrl_Addlog(CamId,_T("TEST PASS"),COLOR_GREEN,200);
					BOOL bReFreshUi = (int)lParam&0x02;

					if(OFccmControl::globalset.CamType==L"Dual" && pCcmCtrl->Cam[1].pTestCtrl->m_TestMode==TESTMODE_AATEST)
					{
						pCcmCtrl->CloseCamera(0,1);
						pCcmCtrl->CloseCamera(1,1);
						pCcmCtrl->DrawingAndShowResult(0,_T("TEST PASS"), TRUE);
						pCcmCtrl->DrawingAndShowResult(1,_T("TEST PASS"), TRUE);
					}
					else
					{
						pCcmCtrl->CloseCamera(CamId,1);
						pCcmCtrl->DrawingAndShowResult(CamId,_T("TEST PASS"), TRUE,TRUE,bReFreshUi);
					}
				}

				break;
			case WM_SOCKET_TESTFAIL:
				{ 
					BOOL bReFreshUi = (int)lParam&0x02;
					//关闭模组 显示NG
					if(OFccmControl::globalset.CamType==L"Dual" && pCcmCtrl->Cam[1].pTestCtrl->m_TestMode==TESTMODE_AATEST)
					{
						pCcmCtrl->CloseCamera(0,0);
						Sleep(300);
						pCcmCtrl->CloseCamera(1,0);
			
						pCcmCtrl->DrawingAndShowResult(0,_T("TEST FAIL"), FALSE,TRUE);						
						pCcmCtrl->DrawingAndShowResult(1,_T("TEST FAIL"), FALSE,TRUE);

					}
					else
					{
						pCcmCtrl->CloseCamera(CamId,1);
						pCcmCtrl->DrawingAndShowResult(CamId,_T("TEST FAIL"), FALSE,TRUE,bReFreshUi);
					}
				}
				break;
			case WM_SOCKET_STARTTEST:
				if (pCcmCtrl->Cam[CamId].pTestCtrl->m_TestMode==TESTMODE_AUTO)
				{
					{

						pCcmCtrl->Ctrl_Addlog( CamId,_T("Socket OpenCamera..."),COLOR_BLUE,220);

						if(OFccmControl::globalset.CamType==L"Dual")
						{
							if(!pCcmCtrl->isStandby(0))
							{
								pCcmCtrl->CloseCameraMsg(0);
								Sleep(500);
							}

							if(!pCcmCtrl->isStandby(1))
							{
								pCcmCtrl->CloseCameraMsg(1);
								Sleep(500);
							}

							for(int i=0;i<2;i++)
							{
								OnPlay(i);
								Sleep(200);
							}
						}
						else
						{
							OnPlay(CamId);
						}

					} 
				}
				else
				{
					Sleep(100);
					pCcmCtrl->Cam[CamId].pTestCtrl->TestInit(1); 
				}
				
				break;
			case WM_SOCKET_STARTCHECK:
				pCcmCtrl->Cam[CamId].pTestCtrl->SetTestItem(0);
				Sleep(100);
				pCcmCtrl->Cam[CamId].pTestCtrl->TestInit(4); 
				break;
			case WM_SOCKET_SHOW_RESULT:
				{ 
					BOOL bReFreshUi_pass = ((int)lParam&0x02)>>1;
					pCcmCtrl->DrawingAndShowResult(CamId,_T("TEST PASS"), bReFreshUi_pass,TRUE,FALSE);
					break;
				}
			case  WM_SOCKET_CCT_A_OK:
				{
					pCcmCtrl->ReturnSetLEDG4C(CamId,WM_SOCKET_CCT_A_OK);
					break;
				}
			case WM_SOCKET_CCT_TL84_OK:
				{
					pCcmCtrl->ReturnSetLEDG4C(CamId,WM_SOCKET_CCT_TL84_OK);
					break;
				}
			case WM_SOCKET_CCT_D65_OK:
				{
					pCcmCtrl->ReturnSetLEDG4C(CamId,WM_SOCKET_CCT_D65_OK);
					break;
				}
			case WM_SOCKET_CCT_D50_OK:
				{
					pCcmCtrl->ReturnSetLEDG4C(CamId,WM_SOCKET_CCT_D50_OK);
					break;
				}

			case WM_AUTOCALIBRATION: //自动校验
				{
					// ADDED BY WZX	
					pCcmCtrl->bCheckLight = TRUE;
					pCcmCtrl->Ctrl_Addlog( CamId,_T("收到校验命令"),COLOR_BLUE,220);
					if (pCcmCtrl->bCheckLight == TRUE&& nStart == 0)
					{	
						//pCcmCtrl->Ctrl_Addlog( CamId,_T("初始化校验"),COLOR_BLUE,220);
						if (OFccmControl::globalset.CamTotalNum == 2)
						{
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,0);
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,1);
						}
						else
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,0);
					}
					

					nStart++;


					if (nStart>12)
					{
						nStart = 0;
						//清0
						if (OFccmControl::globalset.CamTotalNum == 2)
						{
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,0);
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,1);

							pCcmCtrl->UnlocktheFile(0);
							pCcmCtrl->UnlocktheFile(1);
						}
						else
						{
							pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,0);
							pCcmCtrl->UnlocktheFile(0);
						}
						//		AfxMessageBox(_T("关闭光源校准功能！"));	
						nStart = 0;
						pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("AUTOLIGHTSTATUS"),AUTOLIGHT_INIT,0,0);
						pCcmCtrl->bCheckLight = FALSE;
						break;
					}

					// 打开测试
					StartTestOpen(CamId);		
					break;
				}
			case WM_AUTOGETAVE: //自动导入
				{
					if (pCcmCtrl->bCheckLight == TRUE)
					{
						pCcmCtrl->Ctrl_Addlog( CamId,_T("收到导入命令"),COLOR_BLUE,220);
						int nsel = pCcmCtrl->Config_ReadConfigInt(_T("WBSetting"),_T("AUTOLIGHTSTATUS"),0,0);
						CString strTemp;
						CString strLog;

						if (nsel == AUTOLIGHT_SAVECOEFFEE_SUCESS)  
						{
							strTemp = L"T0200";
							pCcmCtrl->Cam[CamID].camSocket->SendInfo(strTemp);
							ShowWndInfo(GetDlgItem(IDC_TESTInfo1),0); //20180122 added by wzx
						}
						else if (nsel == AUTOLIGHT_SAVECOEFFEE_FAIL_AVAGEGODEN)
						{
							strTemp = L"T0201";
							pCcmCtrl->Cam[CamID].camSocket->SendInfo(strTemp);
						}
						strLog.Format(L"AutoLightSel:%d", nsel);
						pCcmCtrl->Ctrl_Addlog( CamId,strLog,COLOR_BLUE,220);

						
						strLog = L"Send:";
						strLog += strTemp;
						pCcmCtrl->Ctrl_Addlog( CamId,strLog,COLOR_BLUE,220);
					}

					//清0, AfxMessageBox(_T("关闭光源校准功能！"));	
					if (OFccmControl::globalset.CamTotalNum == 2)
					{
						pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,0);
						pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,1);

						pCcmCtrl->UnlocktheFile(0);
						pCcmCtrl->UnlocktheFile(1);
					}
					else
					{
						pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,0);
						pCcmCtrl->UnlocktheFile(0);
					}
			
					nStart = 0;
					pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("AUTOLIGHTSTATUS"),AUTOLIGHT_INIT,0,0);
					pCcmCtrl->bCheckLight = FALSE;
					
				}
				break;

			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


void COFccmTestDlg::StartTestOpen(int CamId)
{
	if (pCcmCtrl->Cam[CamId].pTestCtrl->m_TestMode==TESTMODE_AUTO)
	{
		{
			pCcmCtrl->Ctrl_Addlog( CamId,_T("Socket OpenCamera..."),COLOR_BLUE,220);

			if(OFccmControl::globalset.CamType==L"Dual")
			{
				if(!pCcmCtrl->isStandby(0))
				{
					pCcmCtrl->CloseCameraMsg(0);
					Sleep(500);
				}

				if(!pCcmCtrl->isStandby(1))
				{
					pCcmCtrl->CloseCameraMsg(1);
					Sleep(500);
				}

				for(int i=0;i<2;i++)
				{
					OnPlay(i);
					Sleep(200);
				}
			}
			else
			{
				OnPlay(CamId);
			}

		} 
	}
	else
	{
		Sleep(100);
		pCcmCtrl->Cam[CamId].pTestCtrl->TestInit(1); 
	}
}

LRESULT COFccmTestDlg::CreateSettingDlg(WPARAM wParam, LPARAM lParam)
{
#if 1
	if (NULL == p_MyTestItemOptionDlg[CamID])
	{
		p_MyTestItemOptionDlg[CamID] = new MyTestItemOptionDlg(CamID);
		p_MyTestItemOptionDlg[CamID]->Create(IDD_MyTestItemOptionDlg,this);
	}

#endif

	return 0;
}


LRESULT COFccmTestDlg::CreateMachineSettingDlg(WPARAM wParam, LPARAM lParam)
{
	p_MachineDlg = new MachineDlg();
	//重建设置对话框
	if(p_MachineDlg->m_hWnd != NULL)
	{
		p_MachineDlg->DestroyWindow();
	}

	p_MachineDlg = new MachineDlg();
	p_MachineDlg->Create(IDD_MachineSetting,this);

	return 0;
}
 

 
void COFccmTestDlg::RestartExe()
{
	HANDLE hself = GetCurrentProcess();
	TerminateProcess(hself, 0);
	//CDialogEx::OnCancel();
}


void COFccmTestDlg::OnBnClickedBaseset()
{
	// TODO: 在此添加控件通知处理程序代码
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	BaseSetDlg m_BaseSetDlg;
	m_BaseSetDlg.DoModal();
}


void COFccmTestDlg::OnBnClickedMachinesetting()
{
	// TODO: 在此添加控件通知处理程序代
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	if(!OFccmControl::globalset.bMachine)
	{
		MessageBox(_T("请设定机器型号"));
		return;
	}
	p_MachineDlg->ShowWindow(SW_SHOW);
}


void COFccmTestDlg::OnEnterDebugMode() 
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加控件通知处理程序代码 
 
		CString password = _T("OFILM2016");
		LoginDlg dlg;
		if (dlg.DoModal() != IDOK) return ;
		dlg.m_Password.MakeUpper();
		if (dlg.m_Password != password)	
		{
			AfxMessageBox(_T("密码错误"));
			DebugMode=0;
			UpdateUI(-1, 0);
			return ;
		}
		DebugMode=1;
		UpdateUI(-1,0);
	 
}


void COFccmTestDlg::OnExitDebugMode()
{
	// TODO: 在此添加命令处理程序代码
	DebugMode=0;
	UpdateUI(-1, 0);
}


void COFccmTestDlg::OnOpenCamera()
{
	// TODO: 在此添加命令处理程序代码
	OnPaint();
	SendMessage(WM_UPDATEUI, UI_START_STOP);
	startTime = GetTickCount();
	
	if(OFccmControl::globalset.CamType == _T("Dual"))
	{   
		for(int i=0;i<2;i++)
		{
			pCcmCtrl->Cam[i].bOnlyOpenCamera = FALSE;
			OnPlay(i);
			Sleep(100);
		}
	}
	else
	{      
		pCcmCtrl->Cam[CamID].bOnlyOpenCamera = FALSE;
		OnPlay(CamID);
	}
	
}

void COFccmTestDlg::OnOpenCameraOnly()
{
	// TODO: 在此添加命令处理程序代码
	OnPaint();
	SendMessage(WM_UPDATEUI, UI_START_STOP);
	startTime = GetTickCount();

	if(OFccmControl::globalset.CamType == _T("Dual"))
	{   
		for(int i=0;i<2;i++)
		{
			pCcmCtrl->Cam[i].bOnlyOpenCamera = TRUE;
			OnPlay(i);
			Sleep(100);
		}
	}
	else
	{      
		pCcmCtrl->Cam[CamID].bOnlyOpenCamera = TRUE;
		OnPlay(CamID);
	}

}

void COFccmTestDlg::OnCloseCamera()
{
	// TODO: 在此添加命令处理程序代码

	SendMessage(WM_UPDATEUI, UI_START_STOP);

	if(OFccmControl::globalset.CamType == _T("Dual")&&OFccmControl::globalset.CamTotalNum==2)
	{
		pCcmCtrl->CloseCameraMsg(0);
		Sleep(300);
		pCcmCtrl->CloseCameraMsg(1);
	}
	else
		pCcmCtrl->CloseCameraMsg(CamID);

	Sleep(100);
	 
	CString str;
	str.Format(_T("Cam %d DlgClose"),CamID);
	pCcmCtrl->Ctrl_AddToLog(str ,CamID); 
	char key[512]={0};
	sprintf(key,"Command_%d",pCcmCtrl->GetSiteNo());
	RegSetKey("", key, "");
	UpdateData(FALSE);
	

}


void COFccmTestDlg::OnBaseSetting()
{
	// TODO: 在此添加命令处理程序代码
	/*
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	*/
	BaseSetDlg m_BaseSetDlg;
	m_BaseSetDlg.DoModal();
}

void COFccmTestDlg::OnCameraSetting()
{
	// TODO: 在此添加命令处理程序代码
	/*
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}*/
	MyPropertySheet* pPropertySheet;
	pPropertySheet = new MyPropertySheet(_T("参数设置"),NULL,1,CamID);
	pPropertySheet->DoModal();
	SAFE_DELETE(pPropertySheet);
}


void COFccmTestDlg::OnTestItemManager()
{
	// TODO: 在此添加命令处理程序代码
	if(!pCcmCtrl->IsUSBlock())
	{
		MessageBox(_T("请插入加密狗解锁"));
		return;
	}
	/*
	if(DebugMode==0)
	{
		MessageBox(_T("请插入加密狗解锁"));
		return;
	}*/
	TestItemManage* m_TestItemManage;
	m_TestItemManage = new TestItemManage(CamID);
	m_TestItemManage->DoModal();
	SAFE_DELETE(m_TestItemManage);
}


void COFccmTestDlg::OnTestItemSetting()
{
	// TODO: 在此添加命令处理程序代码
	if(!pCcmCtrl->IsUSBlock())
	{
		MessageBox(_T("请插入加密狗解锁"));
		return;
	}

	p_MyTestItemOptionDlg[CamID]->ShowWindow(SW_SHOW); 
}


void COFccmTestDlg::OnMachineSetting()
{
	// TODO: 在此添加命令处理程序代码
	/*
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}*/
	if(!OFccmControl::globalset.bMachine || p_MachineDlg==NULL)
	{
		MessageBox(_T("请设定机器型号"));
		return;
	}
	p_MachineDlg->ShowWindow(SW_SHOW);
}


int COFccmTestDlg::MenuInit()
{
	menu.LoadMenu(IDR_MENU); 
	menu.AppendMenu(MF_POPUP); //MF_POPUP  MF_SEPARATOR

	CString strMenu;
	strMenu.LoadString(ID_32771);
	menu.AppendMenu(MF_POPUP, ID_32771, strMenu);

	UpdateData(	TRUE);

	return 0;
}

void COFccmTestDlg::OnDebugIIC()
{
	// TODO: 在此添加命令处理程序代码
	/*
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}*/
	MyIICdebug* m_MyIICdebug = new MyIICdebug(CamID);	 
	m_MyIICdebug->DoModal();
}


void COFccmTestDlg::OnEnterImgTest()
{
	// TODO: 在此添加命令处理程序代码
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	  
	BOOL bEnterOffLine;
	//SetCamCode();
	if (!pCcmCtrl->isOffline(CamID))
	{
		bEnterOffLine = TRUE;
		pCcmCtrl->EnterOfflineMode(CamID,bEnterOffLine); 
	}	 
}


void COFccmTestDlg::OnLeaveImgTest()
{
	// TODO: 在此添加命令处理程序代码
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	BOOL bEnterOffLine;
	//SetCamCode();
	 
	bEnterOffLine = FALSE;
	Sleep(100);
	pCcmCtrl->ExitOfflineMode(CamID); 
}


void COFccmTestDlg::OnSaveSingleCamImg()
{
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	// TODO: 在此添加命令处理程序代码
	pCcmCtrl->Ctrl_SaveImage(CamID);

}


void COFccmTestDlg::OnSaveDualCamImg()
{
	// TODO: 在此添加命令处理程序代码
	if(DebugMode==0)
	{
		MessageBox(_T("请登入"));
		return;
	}
	 
	CString sfilename = pCcmCtrl->Ctrl_GetCurrentLocalTime(); 
	sfilename = sfilename + _T("Cam0"); 
	CString ImgPath = OFccmControl::globalset.GetCurrentPath()+_T("ImgSave\\");
	pCcmCtrl->Ctrl_SaveImage(0,1,ImgPath,sfilename);
	sfilename = pCcmCtrl->Ctrl_GetCurrentLocalTime(); 
	sfilename = sfilename + _T("Cam1"); 
	pCcmCtrl->Ctrl_SaveImage(1,1,ImgPath,sfilename);
	 
}


void COFccmTestDlg::OnSelectCam0()
{
	// TODO: 在此添加命令处理程序代码
	CamID = 0;

	UpdateCamSelect(); 
	pCcmCtrl->UnlocktheFile(CamID);
}


void COFccmTestDlg::OnSelectCam1()
{
	// TODO: 在此添加命令处理程序代码
	CamID  = 1;

	UpdateCamSelect();
	pCcmCtrl->UnlocktheFile(CamID);
}

void COFccmTestDlg::UpdateCamSelect()
{
	if((CamID+1) > OFccmControl::globalset.CamTotalNum) return;
	pCcmCtrl->SetCamID(CamID);
	
	
	if (!pCcmCtrl->Cam[CamID].pTestCtrl->isLoadingDll)
	{
		if (!pCcmCtrl->Cam[CamID].pConfig->bIsLoad)
		{			  
			pCcmCtrl->Cam[CamID].pConfig->LoadConfig(CamID);
		} 

		vector<CString>* nameList = pCcmCtrl->Cam[CamID].pConfig->GetDllNamelist();
		vector<CString>  ConfigItemName;
		vector<CString>  CurrentNameList ;
		 map<CString,CString>  itemMap ;
		CurrentNameList.clear();
		for(UINT i=0;i<nameList->size();i++)
		{
			ConfigItemName.clear();
			OFccmControl::globalset.SeparateString(nameList->at(i),'@',&ConfigItemName);
			CurrentNameList.push_back(ConfigItemName[0]);
			itemMap.insert(pair<CString,CString>(ConfigItemName[0], ConfigItemName[1]));
		}
		TestItemManage* m_TestItemManage;
		m_TestItemManage = new TestItemManage(CamID);
		m_TestItemManage->UpdateTestItem(nameList);
		SAFE_DELETE(m_TestItemManage);
		nameList->clear();
		pCcmCtrl->Cam[CamID].pTestCtrl->LoadInspectionItem(&CurrentNameList);  
		pCcmCtrl->Cam[CamID].TestItemNum = CurrentNameList.size();
		

		for (UINT i=0;i<CurrentNameList.size();i++)
		{ 
			nameList->push_back(CurrentNameList[i]+ _T("@")+ itemMap.find(CurrentNameList[i])->second);//重新写入  包含basetype信息
		} 
		pCcmCtrl->Cam[CamID].pConfig->WriteDllName();
	} 

	pCcmCtrl->Cam[CamID].TestItemPass = 0;
	pCcmCtrl->Cam[CamID].bFocusEnd = TRUE;
	pCcmCtrl->Cam[CamID].bFocusStatusChange = FALSE;
	pCcmCtrl->Cam[CamID].MesResult.Res = TRUE;
	pCcmCtrl->Cam[CamID].MesResult.MesErrorInfo = _T("");

	::PostMessage( this->m_hWnd,WM_CREATESETTINGDLG, 0, 0);
	UpdateUI(-1,CamID);
	 
}


void COFccmTestDlg::OnGoldenSampleRatio()
{
	CString TCCFSettingPath;
	if (OFccmControl::globalset.CamTotalNum == 2)
	{
		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(0);//CamID0
		pCcmCtrl->Ctrl_Addlog(CamID,TCCFSettingPath,COLOR_BLUE,200);
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"0",TCCFSettingPath);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,0);

		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(1);//CamID1
		pCcmCtrl->Ctrl_Addlog(CamID,TCCFSettingPath,COLOR_BLUE,200);
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"0",TCCFSettingPath);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,1);
	}
	else
	{
		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(0);//CamID0
		pCcmCtrl->Ctrl_Addlog(CamID,TCCFSettingPath,COLOR_BLUE,200);
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"0",TCCFSettingPath);
//		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),0,0,CamID);
	}
	
	AfxMessageBox(_T("光源校准开始！"));

}
// void COFccmTestDlg::OnGoldenSampleRatio2()
// {
// 	if (OFccmControl::globalset.CamTotalNum == 2)
// 	{
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),3,0,0);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),3,0,1);
// 	}
// 	else
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),3,0,CamID);
// 	AfxMessageBox(_T("第二种光源开始校准！"));
// }
// void COFccmTestDlg::OnGoldenSampleRatio3()
// {
// 	if (OFccmControl::globalset.CamTotalNum == 2)
// 	{
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),6,0,0);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),6,0,1);
// 	}
// 	else
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),6,0,CamID);
// 	AfxMessageBox(_T("第三种光源开始校准！"));
// }
// void COFccmTestDlg::OnGoldenSampleRatio4()
// {
// 	if (OFccmControl::globalset.CamTotalNum == 2)
// 	{
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),9,0,0);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),9,0,1);
// 	}
// 	else
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),9,0,CamID);
// 	AfxMessageBox(_T("第四种光源开始校准！"));
// }
void COFccmTestDlg::OnGoldenSampleRatio5()
{
	CString TCCFSettingPath;
	if (OFccmControl::globalset.CamTotalNum == 2)
	{
		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(0);//CamID0
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"12",TCCFSettingPath);

		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(1);//CamID1
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"12",TCCFSettingPath);

// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,0);
// 		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,1);

		pCcmCtrl->UnlocktheFile(0);
		pCcmCtrl->UnlocktheFile(1);
	}
	else
	{
		TCCFSettingPath = pCcmCtrl->Config_GetConfigPathName(0);//CamID0
		WritePrivateProfileString(_T("WBSetting"),_T("GlobalTime"),L"12",TCCFSettingPath);
//		pCcmCtrl->Config_WriteConfigInt(_T("WBSetting"),_T("GlobalTime"),12,0,CamID);
	    pCcmCtrl->UnlocktheFile(0);
	}
	AfxMessageBox(_T("关闭光源校准功能！"));
}
void COFccmTestDlg::OnReadFuseID()
{
	CString strFuseID;
	strFuseID=pCcmCtrl->Sensor_GetCamFuseID(CamID);
	pCcmCtrl->Ctrl_Addlog(CamID,strFuseID,COLOR_RED,200);
	CStringA filename;
	USES_CONVERSION;
	filename=W2A((pCcmCtrl->Ctrl_GetCurrentPath()+_T("FuseID.txt")));
	FILE *fp;
	fp=fopen(filename,"at");
	if (fp!=NULL)
	{
		CStringA temp=W2A(strFuseID);
		fwrite(temp.GetBuffer(temp.GetLength()),1,temp.GetLength(),fp);
		fwrite("\n",1,1,fp);
		fclose(fp);
	}
	else
	{
		return;
	}
}

void COFccmTestDlg::lockUsb()
{
	HKEY hKey;
	CString str;
	int ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DeviceMap\\SerialComm"), 0,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);

	if (ret == ERROR_SUCCESS)
	{

	}

}

void COFccmTestDlg::ShowWndInfo(CWnd *m_TestInfo,int Index)
{
	CDC *pDC=m_TestInfo->GetDC();
	CFont m_font1;
	m_font1.CreateFont(125, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
	CFont m_font2;
	m_font2.CreateFont(15, 0,0,0,FW_NORMAL, 0,0,0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		CRect m_Rect;
		m_TestInfo->GetClientRect(&m_Rect);
		CRect m_Rect1,m_Rect2,m_Rect3,m_Rect4;
		m_Rect1.top=m_Rect.top;
		m_Rect1.bottom=m_Rect.bottom;
		m_Rect1.left=m_Rect.left;
		m_Rect1.right=m_Rect.Width()/2;
		pDC->SelectObject(&m_font1);
		pDC->FillSolidRect(m_Rect1,RGB(0,255,0));
		m_Rect1.left=m_Rect1.left+m_Rect1.Width()/4;
		if (OFccmControl::globalset.m_MySite!="")
		{
			pDC->DrawText(OFccmControl::globalset.m_MySite,m_Rect1,0);
		}
		else
		{
			if(OFccmControl::globalset.CamTotalNum==2)
			{
				if (OFccmControl::globalset.siteNo==0||OFccmControl::globalset.siteNo==1)
				{
					if(Index==0)
						pDC->DrawText(_T("1"),m_Rect1,0);
					if(Index==1)
						pDC->DrawText(_T("2"),m_Rect1,0);
				}
				if (OFccmControl::globalset.siteNo==2)
				{
					if(Index==0)
						pDC->DrawText(_T("3"),m_Rect1,0);
					if(Index==1)
						pDC->DrawText(_T("4"),m_Rect1,0);
				}
			}
			else
			{
				if (OFccmControl::globalset.siteNo==0||OFccmControl::globalset.siteNo==1)
				{
					pDC->DrawText(_T("1"),m_Rect1,0);	
				}
				else if (OFccmControl::globalset.siteNo==2)
				{
					pDC->DrawText(_T("2"),m_Rect1,0);
				}
				else if (OFccmControl::globalset.siteNo==3)
				{
					pDC->DrawText(_T("3"),m_Rect1,0);
				}
				else if (OFccmControl::globalset.siteNo==4)
				{
					pDC->DrawText(_T("4"),m_Rect1,0);
				}
			}
		}
		

		m_Rect2.top=m_Rect.top;
		m_Rect2.bottom=m_Rect.top+m_Rect.Height()/4;
		m_Rect2.left=m_Rect.Width()/2;
		m_Rect2.right=m_Rect.Width();
		pDC->SelectObject(&m_font2);

		if (pCcmCtrl->Cam[Index].usedMes)
		{
			pDC->FillSolidRect(m_Rect2,RGB(0,255,0));
			m_Rect2.left=m_Rect2.left+m_Rect2.Width()/4;
			m_Rect2.top=m_Rect2.top+m_Rect2.Height()/4;
			pDC->DrawText(_T("Mes 已开启"),m_Rect2,0);

		}
		else
		{
			pDC->FillSolidRect(m_Rect2,RGB(255,0,0));
			m_Rect2.left=m_Rect2.left+m_Rect2.Width()/4;
			m_Rect2.top=m_Rect2.top+m_Rect2.Height()/4;
			pDC->DrawText(_T("Mes 已关闭"),m_Rect2,0);
		}


		m_Rect3.top=m_Rect.top+m_Rect.Height()/4;
		m_Rect3.bottom=m_Rect.top+m_Rect.Height()*5/8;
		m_Rect3.left=m_Rect.Width()/2;
		m_Rect3.right=m_Rect.Width();
		pDC->SelectObject(&m_font2);
		pDC->FillSolidRect(m_Rect3,RGB(0,255,0));
		m_Rect3.left=m_Rect3.left+m_Rect3.Width()/4-10;
		m_Rect3.top=m_Rect3.top+m_Rect3.Height()/4;
		char wctemp[32]={0};
		char wcstation[32]={0};
		wchar_t station[32]={0};
		sprintf(wctemp,"CamID%d_Time%d",Index,OFccmControl::globalset.siteNo);
		RegReadKey("",wctemp,wcstation);
		USES_CONVERSION;
		wcscpy(station,A2W(wcstation));
		CString strTemp;
		strTemp.Format(_T("校准时间：\n%s"),station);
		pDC->DrawText(strTemp,m_Rect3,0);

		m_Rect4.top=m_Rect.top+m_Rect.Height()*5/8;
		m_Rect4.bottom=m_Rect.top+m_Rect.Height();
		m_Rect4.left=m_Rect.Width()/2;
		m_Rect4.right=m_Rect.Width();
		pDC->SelectObject(&m_font2);
		pDC->FillSolidRect(m_Rect4,RGB(0,255,0));
		m_Rect4.left=m_Rect4.left+m_Rect4.Width()/4-10;
		m_Rect4.top=m_Rect4.top+m_Rect4.Height()/4;

		sprintf(wcstation,"CamID%d_Site%d",Index,OFccmControl::globalset.siteNo);
		char mystation[32];
		RegReadKey("",wcstation,mystation);

		wcscpy(station,A2W(mystation));
		pDC->DrawText(station,m_Rect4,0);
		ReleaseDC(pDC);
	
}

void COFccmTestDlg::ShowTestInfo()
{
	// TODO:  Change any attributes of the DC here
	ShowWndInfo(GetDlgItem(IDC_TESTInfo1),0);
	ShowWndInfo(GetDlgItem(IDC_TESTInfo2),1);
}


void COFccmTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==0)//画图
	{
		KillTimer(0);
		for (int i=0;i<3;i++)
		{
			ShowTestInfo();
			fSleep(1000);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void COFccmTestDlg::OnChangeStrbarcode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	char strtemp[128]={0};
	GetDlgItemTextA(this->m_hWnd,IDC_strBarCode,strtemp,128);
    CStringA strBarCode(strtemp);
	GetDlgItemTextA(this->m_hWnd,IDC_strRuncard,strtemp,128);
	CStringA strRuncard(strtemp);
	if (strRuncard.GetLength()<5)
	{
		pCcmCtrl->Ctrl_Addlog(CamID,_T("Runcard 异常"),COLOR_RED,200);
		return;
	}
	if (strBarCode.GetLength()<5)
	{
		pCcmCtrl->Ctrl_Addlog(CamID,_T("BarCode 异常"),COLOR_RED,200);
	}
	char key[512]={0};
	sprintf(key,"Command_%d",pCcmCtrl->GetSiteNo());
	char temp[512]={0};
	sprintf(temp,"T040100:%s:%s",strRuncard,strBarCode);
	RegSetKey("",key,temp);
	CString TempStr(temp);
	pCcmCtrl->Ctrl_Addlog(CamID, TempStr, COLOR_BLUE, 200);
	// TODO:  Add your control notification handler code here
}


void COFccmTestDlg::OnChangeStrbarcode2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	char strtemp[128]={0};
	GetDlgItemTextA(this->m_hWnd,IDC_strBarCode2,strtemp,128);
	CStringA strBarCode(strtemp);
	GetDlgItemTextA(this->m_hWnd,IDC_strRuncard2,strtemp,128);
	CStringA strRuncard(strtemp);
	if (strRuncard.GetLength()<5)
	{
		pCcmCtrl->Ctrl_Addlog(CamID,_T("Runcard 异常"),COLOR_RED,200);
		return;
	}
	if (strBarCode.GetLength()<5)
	{
		pCcmCtrl->Ctrl_Addlog(CamID,_T("BarCode 异常"),COLOR_RED,200);
	}
	char key[512]={0};
	sprintf(key,"Command_%d",pCcmCtrl->GetSiteNo());
	char temp[512]={0};
	sprintf(temp,"T040100:%s:%s",strRuncard,strBarCode);
	RegSetKey("",key,temp);
	CString TempStr(temp);
	pCcmCtrl->Ctrl_Addlog(CamID, TempStr, COLOR_BLUE, 200);
	// TODO:  Add your control notification handler code here
}



void COFccmTestDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	
	if(OFccmControl::globalset.CamType == _T("Dual")&&OFccmControl::globalset.CamTotalNum==2)
	{
		pCcmCtrl->CloseCamera(0,0);
		Sleep(300);
		pCcmCtrl->CloseCamera(1,0);
	}
	else
		pCcmCtrl->CloseCamera(0,0);
	Sleep(300);
	CDialogEx::OnClose();
}


void COFccmTestDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


LRESULT COFccmTestDlg::DestroySettingDlg(WPARAM wParam, LPARAM lParam)
{
	if(p_MyTestItemOptionDlg[CamID] != NULL)
	{
		if(p_MyTestItemOptionDlg[CamID]->m_hWnd != NULL)
		{
			p_MyTestItemOptionDlg[CamID]->DestroyWindow();
		}
		SAFE_DELETE(p_MyTestItemOptionDlg[CamID]);
	}

	return 0;
}
