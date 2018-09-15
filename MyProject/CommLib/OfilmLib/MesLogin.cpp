// MesLogin.cpp : implementation file
//

#include "stdafx.h"
#include "OfilmLib.h"
#include "MesLogin.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString GetModulePath()
{
	int  i    = 0;
	int  len  = 0;
	bool flag = false;
	CString myStr;
	char ModulePath[128] = {0};
	char path[128]={0};
	GetModuleFileName(NULL, ModulePath, 128);//return  real  lenghth
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

BOOL CMesLogin::FtpConnect()
{
	hSession =InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	
	CString ftpFile = GetModulePath() + "FtpSetting.dll";
	char strHostName[512] = {0};
	GetPrivateProfileString("FtpSetting","ServerIP","192.168.210.5",strHostName,512,ftpFile);
	OutputDebugString(ftpFile.GetBuffer(ftpFile.GetLength()));
	
	CString strUser="CCM-SW";
	CString strPwd="liangwan100zl";
	OutputDebugString(strHostName);
	hConnection = InternetConnect(hSession,strHostName,INTERNET_DEFAULT_FTP_PORT,strUser,strPwd,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
	if (hConnection==0)
	{
		return FALSE;
	}
	return TRUE;	
	
}
/////////////////////////////////////////////////////////////////////////////
// CMesLogin dialog


CMesLogin::CMesLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CMesLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMesLogin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMesLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMesLogin)
	DDX_Control(pDX, IDC_BTNCLOSE, m_BtnClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMesLogin, CDialog)
	//{{AFX_MSG_MAP(CMesLogin)
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	ON_CBN_DROPDOWN(IDC_LINENO, OnDropdownLineno)
	ON_CBN_DROPDOWN(IDC_STATIONNO, OnDropdownStationno)
	ON_CBN_SELCHANGE(IDC_LINENO, OnSelchangeLineno)
	ON_CBN_SELCHANGE(IDC_STATIONNO, OnSelchangeStationno)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTNCLOSE, OnBtnclose)
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_DROPDOWN(IDC_SERADDR, OnDropdownSeraddr)
	ON_CBN_SELCHANGE(IDC_SERADDR, OnSelchangeSeraddr)
	ON_CBN_DROPDOWN(IDC_MODULEID, OnDropdownModuleid)
	ON_CBN_SELCHANGE(IDC_MODULEID, OnSelchangeModuleid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMesLogin message handlers

BOOL CMesLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 	m_ConfigFile=GetModulePath()+"MesSetting.dll";
 	m_bmpBackground.LoadBitmap(IDB_BITMAPUI);						
 	m_BtnClose.LoadBitmaps(IDB_CLOSEUP,IDB_CLOSEDOWN);
 
 	FontTxt.CreateFont (15, // FONT SIZE
 					   8, 0, 0, FW_DEMIBOLD, 0, 0, 0, ANSI_CHARSET,  //FW_DEMIBOLD -> be bold
 					   OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY, 
 					   VARIABLE_PITCH|FF_SWISS, "Cambria");

	RegReadKey("MESSetting","Operator",Operator);	
	GetDlgItem(IDC_OPERATOR)->SetWindowText(Operator);
	
	RegReadKey("MESSetting","Machine_NO",Machine_NO);
	GetDlgItem(IDC_MACHINE)->SetWindowText(Machine_NO);
	GetDlgItem(IDC_LINENO)->SetFocus();

	int WindowLeft=GetPrivateProfileInt("MESSetting","WindowLeft",0,m_ConfigFile);
	int WindowTop=GetPrivateProfileInt("MESSetting","WindowTop",0,m_ConfigFile);

	if ((WindowLeft!=0)&&(WindowTop!=0))
	{
		SetWindowPos(NULL,WindowLeft,WindowTop,0,0,SWP_NOSIZE|SW_SHOW);
	}

	SetTimer(1,300,NULL);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMesLogin::OnLogin() 
{
	// TODO: Add your control notification handler code here

	char Temp[512]={0};

	GetDlgItemText(IDC_SERADDR,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请选择服务器!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_MODULEID,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请选择机种名称!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_LINENO,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请选择线别名称!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_STATIONNO,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请选择站别名称!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_OPERATOR,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请填写作业员工号!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}


	GetDlgItemText(IDC_MACHINE,Temp,512);
	if (strlen(Temp)==0)
	{
		MessageBox("请填写机台编号!","MES系统",MB_OK|MB_ICONINFORMATION);
		return;
	}

	GetDlgItemText(IDC_MACHINE,Machine_NO,20);
	RegSetKey("MESSetting","Machine_NO",Machine_NO);

	GetDlgItemText(IDC_OPERATOR,Operator,20);
	RegSetKey("MESSetting","Operator",Operator);
	
	int iSersel=((CComboBox*)GetDlgItem(IDC_SERADDR))->GetCurSel();
	sprintf(Temp,"%d",iSersel);
	WritePrivateProfileString("MESSetting","ServerNo",Temp,m_ConfigFile);

	((CComboBox*)GetDlgItem(IDC_LINENO))->GetWindowText(Temp,512);
	WritePrivateProfileString("MESSetting","LineNoPos",Temp,m_ConfigFile);
	
	((CComboBox*)GetDlgItem(IDC_STATIONNO))->GetWindowText(Temp,512);
	WritePrivateProfileString("MESSetting","StaionNoPos",Temp,m_ConfigFile);

	if (strlen(Temp)!=0)
	{
		char tempStr1[512] = {0};
		char tempStr2[512] = {0};
	    int stationTotalNum = GetPrivateProfileInt("StationInfo","RouteNum",10,m_ConfigFile);
		for (int i=0;i<stationTotalNum;i++)
		{
			sprintf(tempStr1,"Station[%d].Name",i);
            GetPrivateProfileString("StationInfo",tempStr1,"",tempStr2,512,m_ConfigFile);
			if (strcmp(Temp,tempStr2)==0)
			{
				memset(tempStr1,0,512);
				WritePrivateProfileString("MESSetting","StationRouteID",itoa((i+1),tempStr1,10),m_ConfigFile);
				break;
			}
		}
	}	

	WritePrivateProfileString("MESSetting","LineNo",LineNO,m_ConfigFile);
	WritePrivateProfileString("MESSetting","StaionNo",StationNo,m_ConfigFile);

	((CComboBox*)GetDlgItem(IDC_MODULEID))->GetWindowText(Temp,512);
	strupr(Temp);
	WritePrivateProfileString("MESSetting","ModuleNoPos",Temp,m_ConfigFile);
	
	CRect rect;
	GetWindowRect(&rect);
	sprintf(Temp,"%d",rect.left);	
	WritePrivateProfileString("MESSetting","WindowLeft",Temp,m_ConfigFile);

	sprintf(Temp,"%d",rect.top);	
	WritePrivateProfileString("MESSetting","WindowTop",Temp,m_ConfigFile);

	OnOK();
}

bool CMesLogin::GetSCPRNO(char *ResturnData)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ResturnData,"创建连接失败");
		OutputDebugString(ResturnData);
		CoUninitialize();
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ResturnData,"创建选择集实例失败");
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	char sCommand[1024]={0};
	
	Fields *fields=NULL;
	strcpy(sCommand,"select lineid, line_no,line_name  FROM sys_line");
	try
	{
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		OutputDebugString(sCommand);	
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	_variant_t vValue;
	CString TempData;
	int index = 0;
	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{			
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(StationLineInfo[index].SN,TempData.GetBuffer(TempData.GetLength()));
			
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)2));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(StationLineInfo[index].Name,TempData.GetBuffer(TempData.GetLength()));
			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ResturnData,e.Description());
			OutputDebugString(ResturnData);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return false;
		}
	}
	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	strcpy(ResturnData,"OK");
	CoUninitialize();
	return true;
}

void CMesLogin::OnDropdownLineno() 
{
	// TODO: Add your control notification handler code here
	char TempMsg[512]={0};
	((CComboBox*)GetDlgItem(IDC_LINENO))->ResetContent();
	strcpy(LineNO,"");
	memset(StationLineInfo,0,sizeof(StationLineInfo));

	GetSCPRNO(TempMsg);
	for (int i=0;i<512;i++)
	{
		if (strlen(StationLineInfo[i].Name)==0)
		{
			break;
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_LINENO))->AddString(StationLineInfo[i].Name);
		}
	}
}

bool CMesLogin::GetStationNO(char *ResturnData)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ResturnData,"创建连接失败");
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	
	_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ResturnData,"创建选择集实例失败");
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	char sCommand[2048]={0};
	
	Fields *fields=NULL;
	sprintf(sCommand,"select b.stationid, b.station_name,a.musttest,a.linkBarcode from sys_route_detail a,sys_station b, sys_route_model c \
where a.routeid = c.routeid and a.station_id = b.stationid and c.model_id = '%s' order by a.route_seq ASC",Module_NO);

	try
	{
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		OutputDebugString(sCommand);	
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	_variant_t vValue;
	CString TempData;
	int index = 0;
	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{			
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(StationInfo[index].SN,TempData.GetBuffer(TempData.GetLength()));
			
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)1));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(StationInfo[index].Name,TempData.GetBuffer(TempData.GetLength()));

			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)2));
			if (vValue.vt!= VT_NULL)
			{
				TempData = ((LPCSTR)(_bstr_t)vValue);
				TempData.TrimLeft();
				TempData.TrimRight();
				TempData.MakeUpper();
				if (strcmp(TempData.GetBuffer(TempData.GetLength()),"Y")==0)
				{
					isMustTest[index]=1;
				}
				else
				{
					isMustTest[index]=0;
				}
			}
		    else
			{
                 isMustTest[index]=1;
			}

 //			if(index == 0)
 //			{
 //				vValue = m_pLiuRecordset->GetCollect(_variant_t((long)3));
 
 //				if (vValue.vt!= VT_NULL) 
 //				{
 //					TempData = ((LPCSTR)(_bstr_t)vValue);
 //					TempData.TrimLeft();
 //					TempData.TrimRight();
 //					TempData.MakeUpper();
 //					if (strcmp(TempData.GetBuffer(TempData.GetLength()),"Y")==0)
 //					{
 //						WritePrivateProfileString("MESSetting","UseBarcode","1",m_ConfigFile);
 //					}
 //					else
 //					{
 //						WritePrivateProfileString("MESSetting","UseBarcode","0",m_ConfigFile);
 //					}
 //				}
 //		        else
 //				{
//                      WritePrivateProfileString("MESSetting","UseBarcode","1",m_ConfigFile);
 //				}
 //			}

			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ResturnData,e.Description());
			OutputDebugString(ResturnData);   
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return false;
		}
	}
	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	strcpy(ResturnData,"OK");
	CoUninitialize();
	return true;
}

void CMesLogin::OnDropdownStationno() 
{
	// TODO: Add your control notification handler code here
	char TempMsg[512]={0};
	char stationNum[512] = {0};
	((CComboBox*)GetDlgItem(IDC_STATIONNO))->ResetContent();
	strcpy(StationNo,"");
	memset(StationInfo,0,sizeof(StationInfo));
    GetStationNO(TempMsg);

	for (int i=0;i<512;i++)
	{
		if (strlen(StationInfo[i].Name)==0)
		{
			break;
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_STATIONNO))->AddString(StationInfo[i].Name);
			sprintf(stationNum,"Station[%d].Name",i);
			WritePrivateProfileString("StationInfo",stationNum,StationInfo[i].Name,m_ConfigFile);
			sprintf(stationNum,"Station[%d].RouteID",i);
			WritePrivateProfileString("StationInfo",stationNum,StationInfo[i].SN,m_ConfigFile);
			sprintf(stationNum,"Station[%d].IsMust",i);
			WritePrivateProfileString("StationInfo",stationNum,itoa(isMustTest[i],TempMsg,10),m_ConfigFile);
		}
	}

	WritePrivateProfileString("StationInfo","RouteNum",itoa(i,stationNum,10),m_ConfigFile);
}

void CMesLogin::OnSelchangeLineno() 
{
	// TODO: Add your control notification handler code here
	char strLineName[512]={0};
	GetDlgItemText(IDC_LINENO,strLineName,512);
	if (strlen(strLineName)==0)
	{
		strcpy(LineNO,"");
	}
	else
	{
		for (int i=0;i<512;i++)
		{
			if (strcmp(StationLineInfo[i].Name,strLineName)==0)
			{
				strcpy(LineNO,StationLineInfo[i].SN);				
				break;
			}
		}
	}
}

void CMesLogin::OnSelchangeStationno() 
{
	// TODO: Add your control notification handler code here
	char strStaionName[512]={0};
	GetDlgItemText(IDC_STATIONNO,strStaionName,512);
	if (strlen(strStaionName)==0)
	{
		strcpy(StationNo,"");
	}
	else
	{
		for (int i=0;i<512;i++)
		{
			if (strcmp(StationInfo[i].Name,strStaionName)==0)
			{
				strcpy(StationNo,StationInfo[i].SN);
				WritePrivateProfileString("MESSetting","StationRouteID",itoa((i+1),strStaionName,10),m_ConfigFile);
				break;
			}
		}
	}	
}

void CMesLogin::OnPaint() 
{	
	CPaintDC dc(this);				//对话框的dc
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc);   //创建与对话框dc兼容的内存dc
		
	CRect rect;
	GetClientRect(&rect);
		
	BITMAP bitMap;
	m_bmpBackground.GetBitmap(&bitMap);
		
	CBitmap *pbmpOld=dcMem.SelectObject(&m_bmpBackground);		//将背景位图选入内存dc中
	//dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bitMap.bmWidth,bitMap.bmHeight,SRCCOPY);   //将内存dc中的位图拉伸显示在对话框的dc中
	dc.BitBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);
	GetDlgItem(IDC_STATIC1)->SetFont(&FontTxt);
}

void CMesLogin::OnBtnclose() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CMesLogin::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	if(pWnd->GetDlgCtrlID()==IDC_BTNCLOSE)
	{
		
		SetCursor(AfxGetApp()->LoadCursor(IDC_MYHAND));
		return true;
	}
	SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR1));
	return true;
}

void CMesLogin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y)); 	//向系统发送HTCAPTION消息，让系统以为鼠标点在标题栏上
	
	CDialog::OnLButtonDown(nFlags, point);
}

HBRUSH CMesLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(pWnd->GetDlgCtrlID()==IDC_STATIC1)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255)); 
		return   (HBRUSH)::GetStockObject(NULL_BRUSH);   
	}
	return hbr;
}

void CMesLogin::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==1)
	{
		KillTimer(1);
		char Temp[512]={0};
		//RegReadKey("MESSetting","LineNoPos",Temp);

		GetPrivateProfileString("MESSetting","ServerNo",NULL,Temp,512,m_ConfigFile);
		if (strlen(Temp)>0)
		{
			int PPos=atoi(Temp);
			OnDropdownSeraddr();
			int PCount=((CComboBox*)GetDlgItem(IDC_SERADDR))->GetCount();
			if (PPos<PCount)
			{
				((CComboBox*)GetDlgItem(IDC_SERADDR))->SetCurSel(PPos);
				OnSelchangeSeraddr();
			}
		}

		//RegReadKey("MESSetting","LineNoPos",Temp);
		GetPrivateProfileString("MESSetting","LineNoPos",NULL,Temp,512,m_ConfigFile);
		if (strlen(Temp)>0)
		{
			char LineName[512]={0};
			strcpy(LineName,Temp);
			OnDropdownLineno();
			int PCount=((CComboBox*)GetDlgItem(IDC_LINENO))->GetCount();
			for (int i=0;i<PCount;i++)
			{
				char TempLineName[512]={0};
				CString Temp;
				((CComboBox*)GetDlgItem(IDC_LINENO))->GetLBText(i,Temp);
				strcpy(TempLineName,Temp.GetBuffer(Temp.GetLength()));
				if (strcmp(LineName,TempLineName)==0)
				{
					((CComboBox*)GetDlgItem(IDC_LINENO))->SetCurSel(i);
					break;
				}
			}
		}
		
		GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,Temp,512,m_ConfigFile);
		if (strlen(Temp)>0)
		{
			char StationName[512]={0};
			strcpy(StationName,Temp);
			OnDropdownModuleid();			
			GetPrivateProfileString("MESSetting","ModuleNoPos",NULL,Module_NO,512,m_ConfigFile);
			int PCount=((CComboBox*)GetDlgItem(IDC_MODULEID))->GetCount();
			for (int i=0;i<PCount;i++)
			{
				char TempStationName[512]={0};
				CString Temp;
				((CComboBox*)GetDlgItem(IDC_MODULEID))->GetLBText(i,Temp);
				strcpy(TempStationName,Temp.GetBuffer(Temp.GetLength()));
				if (strcmp(StationName,TempStationName)==0)
				{
					((CComboBox*)GetDlgItem(IDC_MODULEID))->SetCurSel(i);
					break;
				}
			}

			GetPrivateProfileString("MESSetting","StaionNoPos",NULL,Temp,512,m_ConfigFile);
			if (strlen(Temp)>0)
			{
				char StationName[512]={0};
				strcpy(StationName,Temp);
				GetPrivateProfileString("MESSetting","LineNo",NULL,LineNO,512,m_ConfigFile);
				OnDropdownStationno();
				GetPrivateProfileString("MESSetting","StaionNo",NULL,StationNo,512,m_ConfigFile);
				int PCount=((CComboBox*)GetDlgItem(IDC_STATIONNO))->GetCount();
				for (int i=0;i<PCount;i++)
				{
					char TempStationName[512]={0};
					CString Temp;
					((CComboBox*)GetDlgItem(IDC_STATIONNO))->GetLBText(i,Temp);
					strcpy(TempStationName,Temp.GetBuffer(Temp.GetLength()));
					if (strcmp(StationName,TempStationName)==0)
					{
						((CComboBox*)GetDlgItem(IDC_STATIONNO))->SetCurSel(i);
						break;
					}
				}	
			}    
		}

	}
	CDialog::OnTimer(nIDEvent);
}

void CMesLogin::OnDropdownModuleid() 
{
	// TODO: Add your control notification handler code here
	char TempMsg[512]={0};
	((CComboBox*)GetDlgItem(IDC_MODULEID))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_STATIONNO))->ResetContent();
	strcpy(Module_NO,"");
	strcpy(StationNo,"");
	GetModuleName(TempMsg);


	for (int i=0;i<512;i++)
	{
		if (strlen(ModuleNOInfo[i].Name)==0)
		{
			break;
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_MODULEID))->AddString(ModuleNOInfo[i].Name);
		}
	}
}

bool CMesLogin::GetModuleName(char *ResturnData)
{
	char ServerIP[512] = {0};
	RegReadKey("MESSetting","LocalServerAddr",ServerIP);
	OutputDebugString(ServerIP);
	
	CoInitialize(NULL);
	_ConnectionPtr m_pLiuConnection;//连接实体
	HRESULT hr = m_pLiuConnection.CreateInstance(__uuidof(Connection));
	if (!SUCCEEDED(hr))
	{
		strcpy(ResturnData,"创建连接失败");
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	
	char strConnection[512] = {0};
	
	sprintf(strConnection,"Provider=SQLOLEDB.1;Password=mes.123;Persist Security Info=True;User ID=mes;Initial Catalog=TESTDB;Data Source=%s\\SQLEXPRESS,1433",
		ServerIP);
	m_pLiuConnection->ConnectionTimeout = 4;
	m_pLiuConnection->CommandTimeout = 15;
	try
	{
		m_pLiuConnection->Open((_bstr_t)strConnection,"","",NULL);
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}
	
	_RecordsetPtr m_pLiuRecordset;
	hr = m_pLiuRecordset.CreateInstance(__uuidof(Recordset));
	if (FAILED(hr))
	{
		strcpy(ResturnData,"创建选择集实例失败");
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();
			m_pLiuConnection = NULL;
		}

		CoUninitialize();
		return false;
	}	
	
	char sCommand[1024]={0};
	
	Fields *fields=NULL;
	strcpy(sCommand,"select model_id  FROM sys_route_model order by model_id");
	try
	{
		m_pLiuRecordset->Open(sCommand,m_pLiuConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		OutputDebugString(sCommand);	
	}
	catch (_com_error &e)
	{
		strcpy(ResturnData,e.Description());
		OutputDebugString(ResturnData);
		if (m_pLiuConnection->State)
		{
			m_pLiuConnection->Close();
		}
		if (m_pLiuConnection)
		{
			m_pLiuConnection.Release();

			m_pLiuConnection = NULL;
		}
		if (m_pLiuRecordset->State)
		{
			m_pLiuRecordset->Close();
		}
		if (m_pLiuRecordset)
		{
			m_pLiuRecordset.Release();
			m_pLiuRecordset = NULL;
		}
		CoUninitialize();
		return false;
	}
	
	_variant_t vValue;
	CString TempData;
	int index = 0;
	while(!m_pLiuRecordset->LiuEOF)
	{
		try
		{			
			vValue = m_pLiuRecordset->GetCollect(_variant_t((long)0));
			TempData = ((LPCSTR)(_bstr_t)vValue);
			TempData.TrimLeft();
			TempData.TrimRight();
			strcpy(ModuleNOInfo[index].SN,TempData.GetBuffer(TempData.GetLength()));
			strcpy(ModuleNOInfo[index].Name,TempData.GetBuffer(TempData.GetLength()));
			index++;
			m_pLiuRecordset->MoveNext();
		}
		catch(_com_error &e)
		{
			strcpy(ResturnData,e.Description());
			OutputDebugString(ResturnData);
			if (m_pLiuConnection->State)
			{
				m_pLiuConnection->Close();
			}
			if (m_pLiuConnection)
			{
				m_pLiuConnection.Release();
				m_pLiuConnection = NULL;
			}
			if (m_pLiuRecordset->State)
			{
				m_pLiuRecordset->Close();
			}
			if (m_pLiuRecordset)
			{
				m_pLiuRecordset.Release();
				m_pLiuRecordset = NULL;
			}
			CoUninitialize();
			return false;
		}
	}
	if (m_pLiuConnection->State)
	{
		m_pLiuConnection->Close();
	}
	if (m_pLiuConnection)
	{
		m_pLiuConnection.Release();
		m_pLiuConnection = NULL;
	}
	if (m_pLiuRecordset->State)
	{
		m_pLiuRecordset->Close();
	}
	if (m_pLiuRecordset)
	{
		m_pLiuRecordset.Release();
		m_pLiuRecordset = NULL;
	}
	strcpy(ResturnData,"OK");
	CoUninitialize();
	return true;
}

void CMesLogin::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	char TmpUSBKey[512]={0};
	GetUSBKeyID(TmpUSBKey);
	if (strlen(TmpUSBKey)==0)
	{
		char TempPwd[512]={0};
		int Result=ShowInputPwbBox("登录到工程模式","请输入密码:",TempPwd);
		if (Result==1)
		{
			char Pwd[512]={0};
			char Des[512]={0};
			bool Result=GetPwd("MesPwd",Pwd,Des);
			if ((strcmp(TempPwd,Pwd)==0)&&(Result))
			{
				GetDlgItem(IDC_SERADDR)->EnableWindow(TRUE);
				GetDlgItem(IDC_LINENO)->EnableWindow(TRUE);
				GetDlgItem(IDC_STATIONNO)->EnableWindow(TRUE);
				GetDlgItem(IDC_OPERATOR)->EnableWindow(TRUE);
				GetDlgItem(IDC_MACHINE)->EnableWindow(TRUE);
                GetDlgItem(IDC_MODULEID)->EnableWindow(TRUE);
			}
			else
			{
				MessageBox("密码输入错误!","MES系统",MB_OK|MB_ICONINFORMATION);
			}
			
		}
	}
	else
	{
		char Pwd[512]={0};
		char Des[512]={0};
		bool Result=GetPwd(TmpUSBKey,Pwd,Des);
		if ((strcmp(Pwd,"TE")==0)&&(Result))
		{
			GetDlgItem(IDC_SERADDR)->EnableWindow(TRUE);
			GetDlgItem(IDC_LINENO)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIONNO)->EnableWindow(TRUE);
			GetDlgItem(IDC_OPERATOR)->EnableWindow(TRUE);
			GetDlgItem(IDC_MACHINE)->EnableWindow(TRUE);
		}
		else
		{
			MessageBox("无法识别的USBKey!","MES系统",MB_OK|MB_ICONINFORMATION);
		}
	}
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CMesLogin::OnDropdownSeraddr() 
{
	// TODO: Add your control notification handler code here
	((CComboBox*)GetDlgItem(IDC_SERADDR))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_LINENO))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_STATIONNO))->ResetContent();
	((CComboBox*)GetDlgItem(IDC_MODULEID))->ResetContent();

	memset(ServerAddr,0,sizeof(MesInfo)*512);

	char m_ConfigFile[512]={0};
	CString Temp=GetModulePath();
	strcpy(m_ConfigFile,Temp.GetBuffer(Temp.GetLength()));

	strcat(m_ConfigFile,"ServerIPList.ini");
	
	BOOL Result=FtpGetFile(hConnection,"\\System\\ServerIPList.ini",m_ConfigFile,FALSE,FILE_ATTRIBUTE_HIDDEN, FTP_TRANSFER_TYPE_BINARY + INTERNET_FLAG_RESYNCHRONIZE, 0);
	if (!Result)
	{
		if (hConnection != NULL)
			InternetCloseHandle(hConnection);
		if (hSession != NULL)
			InternetCloseHandle(hSession);
		Result=FtpConnect();
		if (Result)
		{
			Result=FtpGetFile(hConnection,"\\System\\ServerIPList.ini",m_ConfigFile,FALSE,FILE_ATTRIBUTE_HIDDEN, FTP_TRANSFER_TYPE_BINARY + INTERNET_FLAG_RESYNCHRONIZE, 0);
		}
		else
		{
			return ;
		}
	}

	int ServerNum=GetPrivateProfileInt("Main","ServerNum",0,m_ConfigFile);
	for (int i=0;i<ServerNum;i++)
	{
		char Temp[512]={0};
		sprintf(Temp,"Server[%d].Addr",i);
		GetPrivateProfileString("Main",Temp,NULL,ServerAddr[i].SN,512,m_ConfigFile);

		sprintf(Temp,"Server[%d].Name",i);
		GetPrivateProfileString("Main",Temp,NULL,ServerAddr[i].Name,512,m_ConfigFile);
	}

	for (i=0;i<512;i++)
	{
		if (strlen(ServerAddr[i].Name)==0)
		{
			break;
		}
		else
		{
			((CComboBox*)GetDlgItem(IDC_SERADDR))->AddString(ServerAddr[i].Name);
		}
	}


}

void CMesLogin::OnSelchangeModuleid() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_MODULEID,Module_NO,512);
    OnDropdownStationno();
}


void CMesLogin::OnSelchangeSeraddr() 
{
	// TODO: Add your control notification handler code here
	char ServerName[512]={0};
	GetDlgItemText(IDC_SERADDR,ServerName,512);
	if (strlen(ServerName)==0)
	{
		RegSetKey("MESSetting","LocalServerAddr","");
	}
	else
	{
		for (int i=0;i<512;i++)
		{
			if (strcmp(ServerAddr[i].Name,ServerName)==0)
			{
				RegSetKey("MESSetting","LocalServerAddr",ServerAddr[i].SN);
				break;
			}
		}
	}
}
