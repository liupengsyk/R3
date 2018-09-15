// TestItemManage.cpp : 实现文件
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "OFccmControl.h"
#include "TestItemManage.h"
#include "afxdialogex.h"
#include <WinVer.h>

#include "Iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib") 
// TestItemManage 对话框

IMPLEMENT_DYNAMIC(TestItemManage, CDialogEx)

TestItemManage::TestItemManage(int CamID,CWnd* pParent /*=NULL*/)
	: CDialogEx(TestItemManage::IDD, pParent)
{
	this->CamID = CamID;
	dllDir = OFccmControl::globalset.GetCurrentPath() + _T("TestItem\\") ;	
	DestDir = OFccmControl::globalset.GetCurrentPath() + _T("CurrentTestItem\\") ;	
	bDoModal = FALSE;
}

TestItemManage::~TestItemManage()
{
}

void TestItemManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SrcItemList, m_ListSrc);
	DDX_Control(pDX, IDC_DestList, m_ListDest);
}

 

BEGIN_MESSAGE_MAP(TestItemManage, CDialogEx)
	ON_BN_CLICKED(IDC_SetTestItem, &TestItemManage::OnBnClickedSettestitem)
	ON_BN_CLICKED(IDC_RemoveTestItem, &TestItemManage::OnBnClickedRemovetestitem)
	ON_BN_CLICKED(IDC_SelectedUp, &TestItemManage::OnBnClickedSelectedup)
	ON_BN_CLICKED(IDC_SelectedDown, &TestItemManage::OnBnClickedSelecteddown)
	ON_BN_CLICKED(IDC_SaveOption, &TestItemManage::OnBnClickedSaveoption)
END_MESSAGE_MAP()


// TestItemManage 消息处理程序
BOOL TestItemManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitListCtrl();
	LoadFileToSrcList();
	LoadConfigToDestList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void TestItemManage::OnBnClickedSettestitem()
{
	// TODO: 在此添加控件通知处理程序代码
	//大于12个退出
	int size = m_ListDest.GetItemCount();
	if (size > 40)		return;  //不支持大于40个。

	//得到左边最后一次点击的项目位置索引
	int SrcIndex = m_ListSrc.GetSelectionMark();
	//得到右边的项目位置
	int DestIndex = m_ListDest.GetSelectionMark(); 
	//如果没有选择右边的任何一行，默认插入最后一个
	if (DestIndex < 0)		DestIndex = size; //如果没有选择，默认为最后一个。

	CString SrcName = m_ListSrc.GetItemText(SrcIndex,0);
	
	m_CreateExTestItem = new CreateExTestItem(SrcName);
	m_CreateExTestItem->DoModal();
	if(m_CreateExTestItem->bUsed)
	{
		DestIndex++;  //插入下一行
		CString strCamID ;
		strCamID.Format(_T("%d"),CamID);
		CString CurrentTestItemName(strCamID + m_CreateExTestItem->ExTestItemName);

		if (!CheckSameName(CurrentTestItemName))
		{
			AfxMessageBox(_T("DLL已选择"));
			delete m_CreateExTestItem;
			m_CreateExTestItem = NULL;

			return;
		}

		CopyToCurrentTestItem(SrcName,CurrentTestItemName);
		m_ListDest.InsertItem( DestIndex-1, CurrentTestItemName);
		m_ListDest.SetItemText( DestIndex-1,2,SrcName);
		
		m_ListDest.ListSort();
		//m_ListDest.SetItemText( DestIndex,2,SrcName);
		
		// copy to CurrentTestItem
		
	}
	
	delete m_CreateExTestItem;
	m_CreateExTestItem = NULL;
	return ;
	
}
int TestItemManage::CopyToCurrentTestItem(CString m_fileName,CString DestName)
{ 
	CString DllFullPath = dllDir + m_fileName;
	CString DestFullPath = DestDir + DestName;	 
	int nFlag = ::CopyFile(DllFullPath,DestFullPath,FALSE);
	return 0;
}

void TestItemManage::OnBnClickedRemovetestitem()
{
	// TODO: 在此添加控件通知处理程序代码
	//移除当前所选择的项目
	int index = m_ListDest.GetSelectionMark();

	CString FileName = m_ListDest.GetItemText(index,0);
	CString DestFullPath = DestDir + FileName; 
	m_ListDest.DeleteItem(index);
	m_ListDest.ListSort(); 
	
}


void TestItemManage::OnBnClickedSelectedup()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ListDest.GetSelectionMark();
	CString CurrentItemName = m_ListDest.GetItemText(index,0);//得到当前的项
	CString CurrentBaseItemName = m_ListDest.GetItemText(index,2);//得到当前的项
	CString TopItemName;
	CString TopItemBaseName;
	if(index-1>-1)
	{
		TopItemName = m_ListDest.GetItemText(index-1,0);//得到前一项的名字
		TopItemBaseName = m_ListDest.GetItemText(index-1,2);//得到前一项的名字
	}
	else
		return;
	m_ListDest.SetItemText(index-1,0,CurrentItemName);
	m_ListDest.SetItemText(index-1,2,CurrentBaseItemName);
	m_ListDest.SetItemText(index,0,TopItemName);
	m_ListDest.SetItemText(index,2,TopItemBaseName);
	m_ListDest.SetSelectionMark(index-1);

	m_ListDest.ListSort(); 
}


void TestItemManage::OnBnClickedSelecteddown()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_ListDest.GetSelectionMark();
	CString CurrentItemName = m_ListDest.GetItemText(index,0);//得到当前的项
	CString CurrentBaseItemName = m_ListDest.GetItemText(index,2);//得到当前的项
	CString BotItemName;
	CString BotBaseItemName;
	if(index+1<m_ListDest.GetItemCount())
	{
		BotItemName = m_ListDest.GetItemText(index+1,0);//得到hou 一项的名字
		BotBaseItemName = m_ListDest.GetItemText(index+1,2);//得到hou 一项的名字
	}
	else
		return;
	m_ListDest.SetItemText(index+1,0,CurrentItemName);
	m_ListDest.SetItemText(index+1,2,CurrentBaseItemName);
	m_ListDest.SetItemText(index,0,BotItemName);
	m_ListDest.SetItemText(index,2,BotBaseItemName);
	m_ListDest.SetSelectionMark(index+1);
	 

	m_ListDest.ListSort();
}


void TestItemManage::OnBnClickedSaveoption()
{
	// TODO: 在此添加控件通知处理程序代码
	SaveDestList();
	CDialogEx::OnOK();
}




BOOL TestItemManage::CheckSameName(CString name)
{
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		if (name == m_ListDest.GetItemText(i,0))
			return FALSE;
	}	
	return TRUE;
}
 

int TestItemManage::LoadFileToSrcList(void)
{
	/*CString dllDir = OFccmControl::globalset.GetCurrentPath() + _T("TestItem\\") ;*/
	CString findpath = dllDir+_T("*.dll");
	 
	CFileFind Finder;
	//开始查找这样*.dll类型的文件
	BOOL bW=Finder.FindFile(findpath);
	//有文件吗
	//真正开始查找文件 最后一个文件返回0
	int i = 0;
	while(bW)
	{
		bW = Finder.FindNextFile(); 
		m_ListSrc.InsertItem(i,Finder.GetFileName()); 
		i++; 
	}

	return 0;
}




void TestItemManage::InitListCtrl(void)
{
	bDoModal = TRUE;
	m_ListSrc.InsertColumn(0,_T("名字"),LVCFMT_CENTER,140);
	 


	m_ListDest.InsertColumn(0,_T("名字"),LVCFMT_CENTER,140);
	m_ListDest.InsertColumn(1,_T("序号"),LVCFMT_CENTER,80);
	m_ListDest.InsertColumn(2,_T("BaseType"),LVCFMT_CENTER,80); 
}


void TestItemManage::SaveDestList(void)
{
	//1 重写dll列表
	vector<CString>* nameList = pCcmCtrl->Cam[CamID].pConfig->GetDllNamelist(); 
	nameList->clear();//清空当前列表
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		nameList->push_back(m_ListDest.GetItemText(i,0));//重新写入
	} 
	//1 根据列表的清单重新加载DLL
	pCcmCtrl->Cam[CamID].pTestCtrl->LoadInspectionItem(nameList); 
	 
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		BOOL bLoadPass = FALSE;
		for(UINT j=0;j<nameList->size();j++)
		{
			if(m_ListDest.GetItemText(i,0)==nameList->at(j))
			{
				bLoadPass = TRUE;
				break;
			}
		}
		if(!bLoadPass)
		{
			m_ListDest.DeleteItem(i);
		}
	} 

	 ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_DESTROYSETTINGDLG,0,0);  //kong xiang yuaun

	//2 重新生成选项对话框
	 ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CREATESETTINGDLG,0,0);  //lifangsen 0710

	 nameList->clear();//清空当前列表
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		nameList->push_back(m_ListDest.GetItemText(i,0)+ _T("@")+ m_ListDest.GetItemText(i,2));//重新写入  包含basetype信息
	} 
	// 重新写入config文件
	pCcmCtrl->Cam[CamID].pConfig->WriteDllName();

	
}

int TestItemManage::LoadConfigToDestList(void)
{
	//读取现在CONFIG文件上现有的DLL名字列表；
	vector<CString>* nameList = pCcmCtrl->Cam[CamID].pConfig->GetDllNamelist();

	UpdateTestItem(nameList);
	 
	return 0;
}

//删除CurrentTestItem下面的测试项，重新拷贝
int TestItemManage::UpdateTestItem(vector<CString>* nameList)
{ 
	CString findpath = DestDir+_T("*.dll");
	 
	CFileFind Finder;
	//开始查找这样*.dll类型的文件
	BOOL bW=Finder.FindFile(findpath);
	//有文件吗
	//真正开始查找文件 最后一个文件返回0
	 
	while(bW)
	{
		bW = Finder.FindNextFile(); 
		{
			if(Finder.IsDirectory()) continue;  //如果是目录，跳过去
			if(Finder.IsDots()) continue;       //如果是点文件，跳过去
			CString pathFine = Finder.GetFilePath(); 
			BOOL bFlag = ::DeleteFile(pathFine);
			 
		}
	} 
	 vector<CString>  ConfigItemName;
	for(UINT i=0;i<nameList->size();i++)
	{
		ConfigItemName.clear();
		OFccmControl::globalset.SeparateString(nameList->at(i),'@',&ConfigItemName);
		if(ConfigItemName.size()<2) continue;
		if(ConfigItemName[0].GetLength()>0&&ConfigItemName[1].GetLength()>0)
		{
			CopyToCurrentTestItem(  ConfigItemName[1],  ConfigItemName[0]);
			if(bDoModal)
			{
				m_ListDest.InsertItem(i,ConfigItemName[0]);
				CString szNum;
				szNum.Format(_T("%d"),i+1);
				m_ListDest.SetItemText(i,1,szNum);
				m_ListDest.SetItemText(i,2,ConfigItemName[1]);
			}
		}
	}
	
	return 0;
}

  