// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SettingDlg.h"
#include "afxdialogex.h"
#include "Machine_NCO_SPV3.h"


// SettingDlg 对话框

IMPLEMENT_DYNAMIC(SettingDlg, CDialog)

int	SettingDlg::lastPos = -1;

SettingDlg::SettingDlg(CWnd* pParent /*=NULL*/,Machine_NCO_SPV3 *pMachine)
	: CDialog(SettingDlg::IDD, pParent)
	, ipAddr(_T(""))
	, error_en(FALSE)
 
{
	this->pMachine = pMachine; 
}

SettingDlg::~SettingDlg()
{

}

void SettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_Mode,pMachine->nMode ); 	 
	DDX_Text(pDX, IDC_EDIT_SVR_PORT0,pMachine->ServerPort ); 
	DDX_Control(pDX, IDC_STATION_LIST, StationList);
	DDX_Text(pDX, IDC_COMBO_SPV3_MACHINE_SETTINGS,pMachine->SettingFileName ); 

	DDX_Text(pDX, IDC_EDIT_HANDSHAKE_RECEIVE,pMachine->HandleRevcive ); 
	DDX_Text(pDX, IDC_EDIT_HANDSHAKE_SEND,pMachine->HandleSend ); 

	DDX_Text(pDX, IDC_EDIT_StationName,pMachine->DlgStationWorkSetting.name ); 
	DDX_Text(pDX, IDC_COMBO_STATION_TYPE,pMachine->DlgStationWorkSetting.Type ); 
	//CAM0
	DDX_Text(pDX, IDC_EDIT_CAM0_COUNT,pMachine->DlgStationWorkSetting.m_TestSet[0].count ); 
	DDX_Text(pDX, IDC_CAM0_TESTID,pMachine->DlgStationWorkSetting.m_TestSet[0].strTestItems );

	DDX_Check(pDX, IDC_CHECK_CAM0_SEND_EN,pMachine->DlgStationWorkSetting.msgProtocal[0].SendEn );  
	DDX_Text(pDX, IDC_EDIT_CAM0_RECEIVE,pMachine->DlgStationWorkSetting.msgProtocal[0].Receive ); 
	DDX_Text(pDX, IDC_EDIT_CAM0_PASS,pMachine->DlgStationWorkSetting.msgProtocal[0].SendPass );
	DDX_Text(pDX, IDC_EDIT_CAM0_FAIL,pMachine->DlgStationWorkSetting.msgProtocal[0].SendFail );

	//CAM1
	DDX_Text(pDX, IDC_EDIT_CAM1_COUNT,pMachine->DlgStationWorkSetting.m_TestSet[1].count ); 
	DDX_Text(pDX, IDC_CAM1_TESTID,pMachine->DlgStationWorkSetting.m_TestSet[1].strTestItems );

	DDX_Check(pDX, IDC_CHECK_CAM1_SEND_EN,pMachine->DlgStationWorkSetting.msgProtocal[1].SendEn );  
	DDX_Text(pDX, IDC_EDIT_CAM1_RECEIVE,pMachine->DlgStationWorkSetting.msgProtocal[1].Receive ); 
	DDX_Text(pDX, IDC_EDIT_CAM1_PASS,pMachine->DlgStationWorkSetting.msgProtocal[1].SendPass );
	DDX_Text(pDX, IDC_EDIT_CAM1_FAIL,pMachine->DlgStationWorkSetting.msgProtocal[1].SendFail );

	DDX_CBString(pDX, IDC_COMBO_IP, pMachine->ipAddr);	
	DDX_Check(pDX, IDC_bUseErrorCode,pMachine->bUseErrorCode ); 
	DDX_Check(pDX, IDC_bReFreshUi,pMachine->bRefreshUi ); 

	DDX_Check(pDX, IDC_CHECK_ERROR_EN, error_en);
 
	DDX_Check(pDX, IDC_CHECK_FALSE_CON, pMachine->false_continue);
}


BEGIN_MESSAGE_MAP(SettingDlg, CDialog)
	ON_MESSAGE(WM_UPDATE_SETTING, &SettingDlg::OnUpdateData)
	ON_MESSAGE(WM_KEY_CTRLNUM, &SettingDlg::OnCtrlKeyNum)
	ON_BN_CLICKED(IDC_BUTTON_ADD_STATION, &SettingDlg::OnBnClickedButtonAddStation) 
	ON_BN_CLICKED(IDC_BUTTON_DELETE_STATION, &SettingDlg::OnBnClickedButtonDeleteStation)
	  
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_STATION_LIST, &SettingDlg::OnLvnItemchangedStationList)
	 
END_MESSAGE_MAP()


// SettingDlg 消息处理程序

LRESULT SettingDlg::OnUpdateData(WPARAM wParam, LPARAM lParam)
{
	BOOL bSaveAndValidate = (BOOL) wParam;

	BOOL ret = UpdateData(bSaveAndValidate);

	// TODO: Add your specialized code here
	if(bSaveAndValidate)
	{
	}
	else
	{

	}

	return ret;
}

void SettingDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this;

	CDialog::PostNcDestroy();
}

BOOL SettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化 
		for(int i=0;i<=20;i++)StationList.DeleteColumn(0);
	StationList.DeleteAllItems();
	StationList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT); 
	StationList.InsertColumn( 1,_T("stationList"), LVCFMT_LEFT,200 ); 
	 
	int StationListSize = pMachine->stationNameVec.size();

	for (int i=0;i<StationListSize;i++)    //行
	{
		StationList.InsertItem(i,(pMachine->stationNameVec[i]));
	}
	 
	CComboBox * files = (CComboBox*)GetDlgItem(IDC_COMBO_SPV3_MACHINE_SETTINGS);
	
	for(UINT i=0;i<pMachine->machineParas.size();i++)
	{
		files->AddString(pMachine->machineParas[i]);
	}

	CComboBox * types = (CComboBox*)GetDlgItem(IDC_COMBO_STATION_TYPE);
	types->AddString(_T("0_OpenCamera"));
	types->AddString(_T("1_CloseCamera"));
	types->AddString(_T("2_ShowResult"));
	types->AddString(_T("3_TestAction"));
	types->AddString(_T("4_ChangeCameraTips"));

	CComboBox * ipList = (CComboBox*)GetDlgItem(IDC_COMBO_IP);
	ipvec.clear();
	ipvec = pMachine->pTcpServer->GetIpList();
	for (UINT i=0;i<ipvec.size();i++)
	{
		ipList->AddString(ipvec[i]);
	}

	
	 
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT SettingDlg::OnCtrlKeyNum(WPARAM wParam, LPARAM lParam)
{
	pMachine->OnCtrlKeyNum();
	return 1;
}
  
int SettingDlg::AddStationCheck()
{
	UpdateData(TRUE);
	CString name = pMachine->DlgStationWorkSetting.name;
	if(name.GetLength()<2)
		return -1;
	for(UINT i=0;i<pMachine->stationNameVec.size();i++)
	{
		if(pMachine->stationNameVec[i].Find(name) != -1)
		{
			AfxMessageBox(_T("已经有这个站位信息"));
			return -1;
		}
	}
	if(pMachine->DlgStationWorkSetting.msgProtocal[0].Receive.GetLength()<1)
		return -1;

	return 1;

}
void SettingDlg::OnBnClickedButtonAddStation()
{
	// TODO: 在此添加控件通知处理程序代码
	if(AddStationCheck()!=1)
		return ;

	//添加到list控件 
	int size = pMachine->stationNameVec.size();
	StationList.InsertItem(size+1,(pMachine->DlgStationWorkSetting.name));

	//添加到  vecStationWork
	//vecStationWork
	pMachine->vecStationWork.push_back(pMachine->DlgStationWorkSetting);
	pMachine->UpdateStationNameVec(); 
}
 


void SettingDlg::OnBnClickedButtonDeleteStation()
{
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos=StationList.GetFirstSelectedItemPosition();
	int nListSrcSelected=(int)StationList.GetNextSelectedItem(pos);
	CString selectStation;
	selectStation = StationList.GetItemText(nListSrcSelected,0);
	int Index = 0;
	vector<StationWork>::iterator it;
	for(it = pMachine->vecStationWork.begin(); it != pMachine->vecStationWork.end();it++)
	{
		Index++;
		if(it->name == selectStation)
		{
			pMachine->vecStationWork.erase(it);
			break;
		} 
	} 
	pMachine->UpdateStationNameVec(); 
	StationList.DeleteItem(nListSrcSelected);
}

void SettingDlg::OnLvnItemchangedStationList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UI_Update();
	*pResult = 0;
}

void SettingDlg::UI_Update()
{
	POSITION pos=StationList.GetFirstSelectedItemPosition();
	
	int nListSrcSelected=(int)StationList.GetNextSelectedItem(pos);

	if(lastPos != nListSrcSelected && lastPos != -1)
	{
		//保存上一个
		CString lastStation;
		lastStation = StationList.GetItemText(lastPos,0);
		for(UINT i=0;i<pMachine->vecStationWork.size();i++)
		{
			if(pMachine->vecStationWork[i].name == lastStation)
			{
				UpdateData(TRUE);
				pMachine->vecStationWork[i] = pMachine->DlgStationWorkSetting;			 
				break;
			}
		}
	}
	else if(lastPos != -1)
	{
		return;
	}
	lastPos = nListSrcSelected;
	CString selectStation;
	selectStation = StationList.GetItemText(nListSrcSelected,0);

	for(UINT i=0;i<pMachine->vecStationWork.size();i++)
	{
		if(pMachine->vecStationWork[i].name == selectStation)
		{
			pMachine->DlgStationWorkSetting = pMachine->vecStationWork[i];
			UpdateData(FALSE);
			break;
		}
	}

}


 
