
// Handle_Mdp01Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Handle_Mdp01.h"
#include "Handle_Mdp01Dlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#pragma comment(lib,"M_DP01\\lib\\Release\\vcm_plc_dll.lib")
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define SOCKET_DEBUG
//#define NO_RECV


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHandle_Mdp01Dlg 对话框

 


CHandle_Mdp01Dlg::CHandle_Mdp01Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HANDLE_MDP01_DIALOG, pParent),
	NearPos(0), FarPos(0), CommPort(_T("COM0"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	IP_PC = (_T("127.0.0.1"));
	Port_PC = (6000);
	bCommOn = FALSE;

}

CHandle_Mdp01Dlg::~CHandle_Mdp01Dlg()	// 标准构造函数
{
	pDlgS = NULL;
	bControlThreadOn = FALSE;
//	if (bWorking)
	{
		bWorking = FALSE;
		//AfxEndThread(0);
	}

}


void CHandle_Mdp01Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMBO_COM, CommPort);
	DDX_Text(pDX, IDC_IPADDRESS_PC1, IP_PC);
	DDX_Text(pDX, IDC_EDIT_PC1_PORT, Port_PC);

}

BEGIN_MESSAGE_MAP(CHandle_Mdp01Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_KILLFOCUS(IDC_COMBO_COM, &CHandle_Mdp01Dlg::OnCbnKillfocusComboCom)
	ON_BN_CLICKED(IDC_BUTTON_HOME, &CHandle_Mdp01Dlg::OnBnClickedButtonHome)
	ON_BN_CLICKED(IDC_BUTTON_NEARPOS, &CHandle_Mdp01Dlg::OnBnClickedButtonNearpos)
	ON_BN_CLICKED(IDC_BUTTON_FARPOS, &CHandle_Mdp01Dlg::OnBnClickedButtonFarpos)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHandle_Mdp01Dlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPENCOMM, &CHandle_Mdp01Dlg::OnBnClickedButtonOpencomm)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CHandle_Mdp01Dlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CHandle_Mdp01Dlg 消息处理程序

BOOL CHandle_Mdp01Dlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	bCommOn = FALSE;

	LogCreate();
	SetCurrentPath();
	m_ConfigFileRw.SetConfigFilePath(CurrentPath);
	LoadOption();
	CameraControlInit();
	 
	 
	
	pDlgS = this;
	bControlThreadOn = TRUE;

	if (!InitCommPort()) return FALSE;

	CWinThread* WndThread = AfxBeginThread(ControlerThread, this);
	ControlThreadID = WndThread->m_nThreadID;
	WndThread = NULL;

	bWorking = TRUE;
	UpdateData(FALSE);
	is90 = FALSE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHandle_Mdp01Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHandle_Mdp01Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHandle_Mdp01Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CHandle_Mdp01Dlg::OnCbnKillfocusComboCom()
{
	// TODO: 在此添加控件通知处理程序代码


}

BOOL CHandle_Mdp01Dlg::EnumPortList(CStringArray* str_array)
{
	HKEY hKey;
	CString str;
	int ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DeviceMap\\SerialComm"), 0,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);

	if (ret == ERROR_SUCCESS)
	{
		int i = 0;
		TCHAR portName[256], commName[256];
		DWORD dwLong, dwSize;
		pDlgS->LogShow(_T("EnumPortList_in"));
		while (1)
		{
			dwLong = dwSize = sizeof(portName);
			if (::RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize) == ERROR_NO_MORE_ITEMS)//   枚举串口 
			{
				pDlgS->LogShow(_T("EnumPortList_out"));
				break;
			}		
			str_array->Add(commName);   // commName就是串口名字  
			OutputDebugString(commName);
			i++;
		}
		if (str_array->GetCount() == 0)
		{
			//::AfxMessageBox(_T("在HKEY_LOCAL_MACHINE:Hardware//DeviceMap//SerialComm里找不到串口!"));
			AddLog(_T("在HKEY_LOCAL_MACHINE:Hardware//DeviceMap//SerialComm里找不到串口!"));
		}
		RegCloseKey(hKey);
		return TRUE;
	}
	else
	{
		str.Format(_T("错误代码:%d"), ret);
		//AfxMessageBox(str);
		OutputDebugString(str);
		//::AfxMessageBox(_T("打开注册表位置失败"));
		AddLog(_T("打开注册表位置失败"));
		str.Format(_T("%d"), GetLastError());
		OutputDebugString(str);
		return FALSE;
	}
}

BOOL CHandle_Mdp01Dlg::InitCommPort()
{
	CStringArray comName;
	EnumPortList(&comName);
	int PortCount = comName.GetCount();
	if (PortCount < 1)
		return FALSE;
	CComboBox* ComPort = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
	for (int i = 0; i < PortCount; i++)
	{
		ComPort->AddString(comName[i]);
	}
	return TRUE;
}

void LogShowG(CString Msg)
{
	pDlgS->LogShow(Msg);
}
BOOL CHandle_Mdp01Dlg::OpenCommPort(CString strComm)
{
	int m_port_Z;
	strComm.MakeUpper();
	if (strComm.Left(3) == "COM")
	{
		strComm = strComm.Right(strComm.GetLength() - 3);
	}
	m_port_Z = _ttoi(strComm);
	LogShow(_T("端口打开"));
	if (!plc_IsOpen(m_port_Z))
	{
		if (!plc_InitComPort(m_port_Z))
		{
			OutputDebugString(_T("端口打开失败!"));
			LogShow(_T("端口打开失败"));
			plc_ClosePort();
			return FALSE;
		}
	}
	else
	{
		BOOL x = plc_ClosePort();
		ASSERT(x);
		return FALSE;
	}
	return TRUE;
}

UINT CHandle_Mdp01Dlg::WorkThread(void* lparam)
{
	CHandle_Mdp01Dlg* pDlg = static_cast<CHandle_Mdp01Dlg*>(lparam);
	
	pDlg->TestStation();
	pDlg = NULL;
	return 0;
}

  UINT CHandle_Mdp01Dlg::ControlerThread(void* lparam)
{
	CHandle_Mdp01Dlg* pDlg = static_cast<CHandle_Mdp01Dlg*>(lparam);
	pDlg->Controler();
	pDlg = NULL;
	return 0;
}

  int CHandle_Mdp01Dlg::Controler()
  {
	  MSG msg; 
	 LogShow(_T("Controler_in"));
	  while (bControlThreadOn)
	  { 
		  if (!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		  {
			  Sleep(100);
			  continue;
		  }
		  else
		  {
			  CString stStation;
			  switch (msg.message)
			  { 
			  case TEST_MSG:
				  
				  CurrentStationID = msg.wParam;
				  LogShow(_T("收到测试命令"));
				 
				  stStation.Format(L"station :%d", CurrentStationID);
				  LogShow(stStation);
				  Working();
				  break;
			  case DIS_CON_MSG:
				  OnBnClickedButtonHome();
				  break;
				  
			  default:
				  break;
			  }

		  }
	  }
	  LogShow(_T("Controler_out"));
	  return 0;
  }
   
  void CHandle_Mdp01Dlg::PostMsgToControl(int Msg,int stationID)
  {
	 // HOME_MSG
	  PostThreadMessage(ControlThreadID, Msg , (WPARAM)stationID,0);
  }


  int CHandle_Mdp01Dlg::SendCamMsg(int CamID, int StationID)
  {
	  m_SocketClient.SendStr(m_CameraControl.CamStationStartTest[CamID][StationID]);
	  LogShow(m_CameraControl.CamStationStartTest[CamID][StationID]);
	  m_SocketClient.iCamComplete[CamID] = 0;
	  return 0;
  }

int CHandle_Mdp01Dlg::Home()
{
	plc_WriteData(0, 1, 1); //D0
	return 1;
}
int CHandle_Mdp01Dlg::MoveToNearPos()
{
	plc_WriteData(1, 1, 1);
	return 1;
}
int CHandle_Mdp01Dlg::MoveToFarPos()
{
	plc_WriteData(1, 2, 1);
	return 1;
}
int CHandle_Mdp01Dlg::bMoveCompelete()
{
	LogShow(_T("bMoveCompelete_in"));
	while (bWorking)
	{
		int ret = plc_ReadData(1, 1);
		if (ret == 0)
		{
			LogShow(_T("bMoveCompelete_out"));
			break;
		}
		Sleep(300);
	}
	return 1;
}
int CHandle_Mdp01Dlg::SpinCarrier(BOOL bVertical)
{
	if (!bVertical)
	{
		plc_WriteData(3, 2, 1);//y轴0°
	}
	else
	{
		plc_WriteData(3, 1, 1);//y 轴90°
	}
	return 0;
}
int CHandle_Mdp01Dlg::GetStart()
{
	LogShow(_T("GetStart_in"));
	while (bWorking)
	{
		if (plc_ReadData(4, 1))
		{
			LogShow(_T("GetStart_out"));
			break;
		}			
		Sleep(100);
	}
	return 1;
}
int CHandle_Mdp01Dlg::StopMove()
{
	plc_WriteData(5, 1, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetCam0_Pass()
{
	plc_WriteData(7, 1, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetCam0_Fail()
{
	plc_WriteData(7, 2, 1);
	return 0;
}
 
int CHandle_Mdp01Dlg::SetLightRed()
{
	plc_WriteData(9, 1, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetLightGreen()
{
	plc_WriteData(9, 2, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetLightYellow()
{
	plc_WriteData(9, 3, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetLightRing()
{
	plc_WriteData(9, 4, 1);
	return 0;
}
int CHandle_Mdp01Dlg::SetAlarm()
{
	plc_WriteData(12, 1, 1);
	return 0;
}


BOOL CHandle_Mdp01Dlg::InitDp01()
{
	if (!bCommOn) return FALSE;
	if(is90) SpinCarrier(FALSE);
	is90 = FALSE;
	return TRUE;
}
void CHandle_Mdp01Dlg::OnBnClickedButtonHome()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!bCommOn) return ;
	if (!plc_IsOpen())
		return;
	MoveToFarPos();
	bMoveCompelete();

	Home();
	if(is90) SpinCarrier(FALSE);
	is90 = FALSE;
	SetLightGreen();
}
void CHandle_Mdp01Dlg::OnBnClickedButtonNearpos()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!bCommOn) return ;
	if(!is90) SpinCarrier(TRUE);
	is90 = TRUE;
	SetLightYellow();
	MoveToNearPos();
	
	//bWorking = FALSE;
}
void CHandle_Mdp01Dlg::OnBnClickedButtonFarpos()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!bCommOn) return ;
	if(!is90) SpinCarrier(TRUE);
	is90 = TRUE;
	SetLightYellow();
	MoveToFarPos();
	
	//bWorking = FALSE;
}
void CHandle_Mdp01Dlg::SetCurrentPath()
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
	// _tsplitpath(sFilename, sDrive, sDir, sFname, sExt);
	_wsplitpath_s(sFilename, sDrive, sDir, sFname, sExt);

	CurrentPath = (CString(sDrive) + CString(sDir));
	CurrentPath = CurrentPath + _T("DP01.ini");
}
void CHandle_Mdp01Dlg::LoadOption()
{
	CString Section = _T("HadleSetting");

	CommPort = m_ConfigFileRw.ReadConfigString(Section, _T("CommPort"), _T("CM0"));
	IP_PC = m_ConfigFileRw.ReadConfigString(Section, _T("IP_PC1"), _T("127.0.0.1"));
	Port_PC = m_ConfigFileRw.ReadConfigData(Section, _T("Port_PC1"), 8000);

}
void CHandle_Mdp01Dlg::SaveOption()
{
	UpdateData(TRUE);
	CString Section = _T("HadleSetting");

	m_ConfigFileRw.WriteConfigString(Section, _T("CommPort"), CommPort);
	m_ConfigFileRw.WriteConfigString(Section, _T("IP_PC1"), IP_PC);
	m_ConfigFileRw.WriteConfigData(Section, _T("Port_PC1"), Port_PC);

}
void CHandle_Mdp01Dlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveOption();
	CameraControlInit();
}
 

void CHandle_Mdp01Dlg::TestStation()
{
	LogShow(_T("准备测试"));
	
	switch (CurrentStationID)
	{
	case 0:
		GetStart();
		plc_WriteData(7, 0, 1);//熄灭指示灯
		m_CameraControl.StationTestResult = TRUE;
		SendCamMsg(0, CurrentStationID);
		Sleep(1000);
		SendCamMsg(1, CurrentStationID);
		if(!is90) SpinCarrier(TRUE);
		is90 = TRUE;
		MoveToFarPos();
		bMoveBack = FALSE;
		break;
	case 1://远焦
		bMoveCompelete();
		SendCamMsg(0, CurrentStationID);
		Sleep(1000);
		SendCamMsg(1, CurrentStationID);
		break;

	case 2://近焦
		m_CameraControl.StationTestResult = TRUE;
		if(!is90) SpinCarrier(TRUE);
		is90 = TRUE;
		MoveToNearPos();
		bMoveCompelete();
		SendCamMsg(0, CurrentStationID);
		Sleep(1000);
		SendCamMsg(1, CurrentStationID);
		
		break;
	case 3://计算 
		SendCamMsg(0, CurrentStationID);
		Sleep(1000);
		SendCamMsg(1, CurrentStationID);
		MoveToFarPos();
		bMoveBack = TRUE;
		if (is90) SpinCarrier(FALSE);
		is90 = FALSE;

		break;
	case 4://关闭

		if (!bMoveBack)
		{
			MoveToFarPos();
			bMoveBack = TRUE;
		}
		SendCamMsg(0, CurrentStationID);
		Sleep(1000);
		SendCamMsg(1, CurrentStationID);
		 
		if (m_CameraControl.StationTestResult)
		{
			SetCam0_Pass();
		}
		else
		{
			SetCam0_Fail();
		}
		
		if (is90) SpinCarrier(FALSE);
		is90 = FALSE;
		Sleep(2000);
		PostMsgToControl(TEST_MSG, 0);//进入循环
		break;
	default:
		OnBnClickedButtonHome();
		break;
	} 
}


BOOL CHandle_Mdp01Dlg::ConnectToServer()
{
	BOOL bConnect = TRUE;

	bConnect = m_SocketClient.Conn(IP_PC, Port_PC);
	if (!bConnect)
	{
		LogShow(_T("PC%d连接失败"));
		int erroe = 0;
		m_SocketClient.OnClose(erroe);
		int errorCode = 0;	
		return FALSE;
	}

	LogShow(_T("PC%d连接OK"));

	return bConnect;
}
 
void CHandle_Mdp01Dlg::CameraControlInit()
{
	//OPEN CAMERA
	 
		//Cam 0  1 TEST
		//station 0  open camera
	m_CameraControl.CamStationStartTest[0][0] = _T("T000100");
	m_CameraControl.CamStationTestResultPass[0][0] = _T("H00010000");
	m_CameraControl.CamStationTestResultFail[0][0] = _T("H00010001");
	

	//station 1
	m_CameraControl.CamStationStartTest[0][1] = _T("T010100");
	m_CameraControl.CamStationTestResultPass[0][1] = _T("H01010000");
	m_CameraControl.CamStationTestResultFail[0][1] = _T("H01010001");

	//station 2
	m_CameraControl.CamStationStartTest[0][2] = _T("T010200");
	m_CameraControl.CamStationTestResultPass[0][2] = _T("H01020000");
	m_CameraControl.CamStationTestResultFail[0][2] = _T("H01020001");

	//station 3
	m_CameraControl.CamStationStartTest[0][3] = _T("T010300");
	m_CameraControl.CamStationTestResultPass[0][3] = _T("H01030000");
	m_CameraControl.CamStationTestResultFail[0][3] = _T("H01030001");

	//station 4 close camera
	m_CameraControl.CamStationStartTest[0][4] = _T("T020100");
	m_CameraControl.CamStationTestResultPass[0][4] = _T("H02010000");
	m_CameraControl.CamStationTestResultFail[0][4] = _T("H02010001");


	//Cam 1

	//station 0  open camera
	m_CameraControl.CamStationStartTest[1][0] = _T("T000101");
	m_CameraControl.CamStationTestResultPass[1][0] = _T("H00010100");
	m_CameraControl.CamStationTestResultFail[1][0] = _T("H00010101");

	//station 1
	m_CameraControl.CamStationStartTest[1][1] = _T("T010101");
	m_CameraControl.CamStationTestResultPass[1][1] = _T("H01010100");
	m_CameraControl.CamStationTestResultFail[1][1] = _T("H01010101");

	//station 2
	m_CameraControl.CamStationStartTest[1][2] = _T("T010201");
	m_CameraControl.CamStationTestResultPass[1][2] = _T("H01020100");
	m_CameraControl.CamStationTestResultFail[1][2] = _T("H01020101");

	//station 3
	m_CameraControl.CamStationStartTest[1][3] = _T("T010301");
	m_CameraControl.CamStationTestResultPass[1][3] = _T("H01030100");
	m_CameraControl.CamStationTestResultFail[1][3] = _T("H01030101");

	//station 4
	m_CameraControl.CamStationStartTest[1][4] = _T("T020101");
	m_CameraControl.CamStationTestResultPass[1][4] = _T("H02010100");
	m_CameraControl.CamStationTestResultFail[1][4] = _T("H02010101");

	m_CameraControl.StationTestResult = TRUE;
	   
}
 
 

void CHandle_Mdp01Dlg::LogCreate()
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	GetModuleFileName(AfxGetInstanceHandle(), sFilename, _MAX_PATH);
	_wsplitpath_s(sFilename, sDrive, sDir, sFname, sExt);

	CString strCurrentPath = (CString(sDrive) + CString(sDir));
	if (strCurrentPath.Right(1) != _T('\\'))
		strCurrentPath += _T("\\");

	logPath = strCurrentPath + _T("log.txt");
	CStdioFile logfile;
	logfile.Open(logPath, CFile::modeCreate);
	logfile.Close();
}

void CHandle_Mdp01Dlg::AddLog(CString strLog)
{
	CStdioFile logfile;
	if (logfile.Open(logPath, CFile::modeWrite))
	{
		logfile.SeekToEnd();
		logfile.WriteString(strLog);
	}
	logfile.Close();
 
}



void CHandle_Mdp01Dlg::OnBnClickedButtonOpencomm()
{
	// TODO: 在此添加控件通知处理程序代码
	bWorking = TRUE;
	CString strComm(_T(""));
	  
	BOOL bFlag = TRUE;
	//发送socket连接
	bFlag = ConnectToServer();

	if (!bFlag)
	{
		AddLog(_T("连接失败"));
		return;
	}
	GetDlgItemText(IDC_COMBO_COM, strComm);
	if (strComm.GetLength() < 3)
	{
		//::AfxMessageBox(_T("请选择COM端口，请更换COM端口"));
		AddLog(_T("请选择COM端口，请更换COM端口"));
		bCommOn = FALSE;
		return;
	}

	BOOL bRet = OpenCommPort(strComm);
	if (!bRet)
	{
		//::AfxMessageBox(L"COM端口打开失败，请更换COM端口");
		AddLog(L"COM端口打开失败，请更换COM端口");
		bCommOn = FALSE;
		return;
	}
	else
	{
		//::AfxMessageBox(strComm + _T("打开成功"));
		AddLog(strComm + _T("打开成功"));
		bCommOn = TRUE;
		InitDp01();
		LogShow(strComm + _T("打开成功"));
		PostMsgToControl(TEST_MSG, 0);
	}
}
 
void CHandle_Mdp01Dlg::Working()
{ 
	AfxBeginThread(WorkThread, this);
}
void CHandle_Mdp01Dlg::LogShow(CString Info)
{
	LogBoard = (CRichEditCtrl*)GetDlgItem(IDC_RICHEDIT2_LOG);

	LogBoard->SetSel(0, 0); //防止格式出错，先选中，再设置字体

	DEFcf.yHeight = 200;
	DEFcf.crTextColor = (0, 0, 0);

	LogBoard->SetWordCharFormat(DEFcf);

	CString str = Info;
	str = str + _T("\n");
	LogBoard->ReplaceSel(str);
	//SendMessage(WM_VSCROLL, SB_BOTTOM, 0);	
	SendMessage(WM_VSCROLL, SB_TOP, 0);//	  SB_TOP
	LogBoard->SetSel(-1, -1);
}

void CHandle_Mdp01Dlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	//关闭sock
	int erroe = 0;
	m_SocketClient.OnClose(erroe);
	m_SocketClient.isCon = FALSE;
	bWorking = FALSE;
	//关闭comm
	if(bCommOn)
		plc_ClosePort();
	bCommOn = FALSE;
}
