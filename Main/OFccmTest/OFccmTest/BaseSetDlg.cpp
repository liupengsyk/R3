// BaseSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "BaseSetDlg.h"
#include "afxdialogex.h"
#include "GlobalSetting.h"
#include "RawOptionDlg.h"


// BaseSetDlg 对话框

IMPLEMENT_DYNAMIC(BaseSetDlg, CDialogEx)

BaseSetDlg::BaseSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(BaseSetDlg::IDD, pParent)
	, showLog(FALSE)
{

}

BaseSetDlg::~BaseSetDlg()
{
}

void BaseSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_DeviceSel,OFccmControl::globalset.DeviceTypeSel);
	DDX_Text(pDX,IDC_CamTypeSel,OFccmControl::globalset.CamType);
	DDX_Text(pDX,IDC_CamNumSel,OFccmControl::globalset.CamTotalNum);

	DDX_Check(pDX,IDC_bGetCamCode,OFccmControl::globalset.bUsedCamCode);
	DDX_Check(pDX,IDC_bDebugOn,OFccmControl::globalset.DebugMode);

	DDX_Text(pDX,IDC_Cam0Sensor,OFccmControl::globalset.CAM0_SensorInitFile);
	DDX_Text(pDX,IDC_Cam1Sensor,OFccmControl::globalset.CAM1_SensorInitFile);

	DDX_Text(pDX,IDC_StationName,OFccmControl::globalset.StationName);	

	DDX_Check(pDX,IDC_bOffLineTest,OFccmControl::globalset.OffLineTest);	
	DDX_Text(pDX,IDC_Cam0ImgPath,OFccmControl::globalset.Cam0ImgPath);	
	DDX_Text(pDX,IDC_Cam1ImgPath,OFccmControl::globalset.Cam1ImgPath);	
	DDX_Check(pDX,IDC_bSite,OFccmControl::globalset.bSite);

	DDX_Check(pDX,IDC_bMachine,OFccmControl::globalset.bMachine);
	DDX_Text(pDX,IDC_MachineName,OFccmControl::globalset.MachineName);

	DDX_Check(pDX,IDC_bFocus,OFccmControl::globalset.bFocus);
	DDX_Check(pDX,IDC_bAutoMachine,OFccmControl::globalset.bAutoMachine);

	DDX_Check(pDX,IDC_bOnLButtonUp,OFccmControl::globalset.bOnLButtonUp);
	DDX_Text(pDX,IDC_ButtonUpTestItem,OFccmControl::globalset.ButtonUpTestItem);

	DDX_Text(pDX,IDC_EDIT_DisPlayInterval,OFccmControl::globalset.DisplayInterval);

	DDX_Check(pDX,IDC_bLogSave,OFccmControl::globalset.bLogSave);

	DDX_Text(pDX,IDC_COMB_SITE,OFccmControl::globalset.siteNo);

	DDX_Check(pDX, IDC_CHECK_SHOW_LOG, OFccmControl::globalset.showLog);
	DDX_Text(pDX, IDC_MySite, OFccmControl::globalset.m_MySite);

	DDX_Check(pDX, IDC_bLEDEnable, OFccmControl::globalset.bLEDEnable);
	DDX_Text(pDX, IDC_COM, OFccmControl::globalset.m_com);
}


BEGIN_MESSAGE_MAP(BaseSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &BaseSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_bOffLineTest, &BaseSetDlg::OnBnClickedbofflinetest)
	ON_CBN_KILLFOCUS(IDC_StationName, &BaseSetDlg::OnCbnKillfocusStationname)
	ON_BN_CLICKED(ID_SaveTo, &BaseSetDlg::OnBnClickedSaveto)
	ON_BN_CLICKED(ID_DELETETo, &BaseSetDlg::OnBnClickedDeleteto)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL BaseSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitImgDevSelect();

    InitCamTypeSelect();
	 
	InitCamNumSelect();

	InitSensorSelect();

	InitMachineSelect();

	InitCamSelect();
	return   TRUE;
}

void BaseSetDlg::InitImgDevSelect()
{
	CComboBox* DeviceSel =(CComboBox*)GetDlgItem(IDC_DeviceSel);
	CString str;
	DeviceSel->AddString(_T("UV910"));
	DeviceSel->AddString(_T("UH920"));
	DeviceSel->AddString(_T("MUD952"));
	DeviceSel->AddString(_T("R3"));
	DeviceSel->AddString(_T("R5"));
	DeviceSel->AddString(_T("CTS"));

	CComboBox* siteSel =(CComboBox*)GetDlgItem(IDC_COMB_SITE);
	siteSel->AddString(L"0");
	siteSel->AddString(L"1");
	siteSel->AddString(L"2");
	siteSel->AddString(L"3");
	siteSel->AddString(L"4");
	DeviceSel = NULL;
	siteSel = NULL;
}
 
void BaseSetDlg::InitCamTypeSelect()
{
	CComboBox* CamTypeSel =(CComboBox*)GetDlgItem(IDC_CamTypeSel);
	  
	CamTypeSel->AddString(_T("Normal"));
	CamTypeSel->AddString(_T("Dual"));

	CamTypeSel = NULL;
}

void BaseSetDlg::InitCamNumSelect()
{
	CString str;
	CComboBox* CamNumSel =(CComboBox*)GetDlgItem(IDC_CamNumSel);

	for (int i=1;i<3;i++)
	{
		str.Format(_T("%d"),i);
		CamNumSel->AddString(str);
	}

	CamNumSel  =NULL;
}

void BaseSetDlg::InitSensorSelect()
{
	GetvecSensorlist();

	CComboBox* Cam0SensorSel =(CComboBox*)GetDlgItem(IDC_Cam0Sensor);
	for (UINT i=0;i<vecSensorlist.size();i++)
	{		 
		Cam0SensorSel->AddString(vecSensorlist[i]);
	}

	Cam0SensorSel = NULL;

	CComboBox* Cam1SensorSel =(CComboBox*)GetDlgItem(IDC_Cam1Sensor);
	for (UINT i=0;i<vecSensorlist.size();i++)
	{		 
		Cam1SensorSel->AddString(vecSensorlist[i]);
	}
	
	Cam1SensorSel = NULL;
}

void BaseSetDlg::InitMachineSelect()
{
	GetvecMachinelist();
	CComboBox* machine =(CComboBox*)GetDlgItem(IDC_MachineName);
	for (UINT i=0;i<vecMachinelist.size();i++)
	{		 
		machine->AddString(vecMachinelist[i]);
	}

	machine = NULL;
}

void BaseSetDlg::InitCamSelect()
{
	GetvecCam();
	CComboBox* Cam =(CComboBox*)GetDlgItem(IDC_StationName);
	for (UINT i=0;i<vecCam.size();i++)
	{		 
		Cam->AddString(vecCam[i]);
	}

	Cam = NULL;
}
void BaseSetDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OFccmControl::globalset.SaveSetting();

	
	CDialogEx::OnOK();
}

 
void BaseSetDlg::GetvecSensorlist()
{
    SensorTabPath = OFccmControl::globalset.GetCurrentPath()+ _T("SensorTab\\");
	pCcmCtrl->GetDirAllFile(SensorTabPath,vecSensorlist); 
}

void BaseSetDlg::GetvecMachinelist()
{	
	CString MachineTabPath = OFccmControl::globalset.machinePath;
	pCcmCtrl->GetDirAllFile(MachineTabPath,vecMachinelist); 
}
void BaseSetDlg::GetvecCam()
{
	CString SensorTabPath = OFccmControl::globalset.GetCurrentPath() + _T("SensorTab\\");
	pCcmCtrl->GetDirAllDirs(SensorTabPath,vecCam);  
}
 
	 


void BaseSetDlg::OnBnClickedbofflinetest()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(!OFccmControl::globalset.OffLineTest) return;
	if(m_RawOptionDlg.DoModal() == IDOK)
	{
		OFccmControl::globalset.m_Raw_Width    = m_RawOptionDlg.m_Raw_Width;
		OFccmControl::globalset.m_Raw_Height    = m_RawOptionDlg.m_Raw_Height;
		OFccmControl::globalset.RawBits    = m_RawOptionDlg.RawBits;
		OFccmControl::globalset.RawOutForamt = m_RawOptionDlg.RawOutForamt;
		OFccmControl::globalset.m_RawShowType = m_RawOptionDlg.m_ShowType; //显示各个点
		OFccmControl::globalset.bRawImg = TRUE;
	}
	else
	{ 
		OFccmControl::globalset.bRawImg = FALSE;
		return ;
	}
      
}


void BaseSetDlg::OnCbnKillfocusStationname()
{
	// TODO: 在此添加控件通知处理程序代码
	//copy 至外面，并且完成初始化
	UpdateData(TRUE); 
	 
	//刷新setting
	CString CamSetPath = SensorTabPath + OFccmControl::globalset.StationName+ _T("\\*.*");
	CString StationName = OFccmControl::globalset.StationName;
	vector<CString> sensorTabFiles;
	pCcmCtrl->GetDirAllFile(CamSetPath,sensorTabFiles); 

	for(UINT i=0;i<sensorTabFiles.size();i++)
	{
		int nFlag = ::CopyFile(CamSetPath + sensorTabFiles[i],SensorTabPath + sensorTabFiles[i],FALSE);
	} 
	 
	OFccmControl::globalset.LoadSetting();

	OFccmControl::globalset.StationName = StationName;
	OFccmControl::globalset.SaveSetting();
	UpdateData(FALSE);

}


void BaseSetDlg::OnBnClickedSaveto()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(FALSE);
	OFccmControl::globalset.SaveSetting();

	CString destFilePath = SensorTabPath + OFccmControl::globalset.StationName + _T("\\");
	CString Cam0path = SensorTabPath +  OFccmControl::globalset.CAM0_SensorInitFile;
	CString Cam1path = SensorTabPath +  OFccmControl::globalset.CAM1_SensorInitFile;
 
    CString destFilePath0 = destFilePath +  OFccmControl::globalset.CAM0_SensorInitFile;
	CString destFilePath1 = destFilePath +  OFccmControl::globalset.CAM1_SensorInitFile;
 
	//创建文件夹
	::CreateDirectory(destFilePath,NULL);

	if(Cam0path.GetLength()>2) ::CopyFile(Cam0path ,destFilePath0,FALSE);
	if(Cam1path.GetLength()>2) ::CopyFile(Cam1path ,destFilePath1,FALSE);
 
	::CopyFile(SensorTabPath + _T("init.ini") ,destFilePath + _T("init.ini"),FALSE);
	InitCamSelect();
}


void BaseSetDlg::OnBnClickedDeleteto()
{
	// TODO: 在此添加控件通知处理程序代码
	CString File = SensorTabPath + OFccmControl::globalset.StationName;

	CString CamSetPath = SensorTabPath + OFccmControl::globalset.StationName+ _T("\\*.*");

	vector<CString> sensorTabFiles;
	pCcmCtrl->GetDirAllFile(CamSetPath,sensorTabFiles); 

	for(UINT i=0;i<sensorTabFiles.size();i++)
	{
		int nFlag = ::DeleteFile(CamSetPath + sensorTabFiles[i]);
	} 
	  
	::RemoveDirectory(File);
	 InitCamSelect();
}


HBRUSH BaseSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	/*
	if (pWnd->GetDlgCtrlID() == IDC_TESTInfo1)
	{
		HBRUSH m_brush;
		if(pTestMainView->useMes) 
		{
			m_brush=CreateSolidBrush(RGB(0,255,0));
			//pDC->TextOut(50,5,"Mes已打开"); 
			SetDlgItemText(IDC_MES,"Mes已打开");
		}
		else
		{
			m_brush=CreateSolidBrush(RGB(255,0,0));
			//pDC->TextOut(50,5,"Mes已关闭");
			SetDlgItemText(IDC_MES,"Mes已关闭");
		}
		CFont m_font;
		m_font.CreateFont(25, 0,0,0,FW_NORMAL, 0,0,0,
			DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(RGB(0,0,0)); 
		pDC->SetBkColor(RGB(255,255,255)); 
		pDC->SetBkMode(TRANSPARENT); 
		return m_brush;
	}*/
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
