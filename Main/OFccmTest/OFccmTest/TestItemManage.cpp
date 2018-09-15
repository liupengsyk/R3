// TestItemManage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OFccmTest.h"
#include "OFccmControl.h"
#include "TestItemManage.h"
#include "afxdialogex.h"
#include <WinVer.h>

#include "Iphlpapi.h"
#pragma comment(lib,"Iphlpapi.lib") 
// TestItemManage �Ի���

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


// TestItemManage ��Ϣ�������
BOOL TestItemManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	InitListCtrl();
	LoadFileToSrcList();
	LoadConfigToDestList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void TestItemManage::OnBnClickedSettestitem()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����12���˳�
	int size = m_ListDest.GetItemCount();
	if (size > 40)		return;  //��֧�ִ���40����

	//�õ�������һ�ε������Ŀλ������
	int SrcIndex = m_ListSrc.GetSelectionMark();
	//�õ��ұߵ���Ŀλ��
	int DestIndex = m_ListDest.GetSelectionMark(); 
	//���û��ѡ���ұߵ��κ�һ�У�Ĭ�ϲ������һ��
	if (DestIndex < 0)		DestIndex = size; //���û��ѡ��Ĭ��Ϊ���һ����

	CString SrcName = m_ListSrc.GetItemText(SrcIndex,0);
	
	m_CreateExTestItem = new CreateExTestItem(SrcName);
	m_CreateExTestItem->DoModal();
	if(m_CreateExTestItem->bUsed)
	{
		DestIndex++;  //������һ��
		CString strCamID ;
		strCamID.Format(_T("%d"),CamID);
		CString CurrentTestItemName(strCamID + m_CreateExTestItem->ExTestItemName);

		if (!CheckSameName(CurrentTestItemName))
		{
			AfxMessageBox(_T("DLL��ѡ��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�Ƴ���ǰ��ѡ�����Ŀ
	int index = m_ListDest.GetSelectionMark();

	CString FileName = m_ListDest.GetItemText(index,0);
	CString DestFullPath = DestDir + FileName; 
	m_ListDest.DeleteItem(index);
	m_ListDest.ListSort(); 
	
}


void TestItemManage::OnBnClickedSelectedup()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_ListDest.GetSelectionMark();
	CString CurrentItemName = m_ListDest.GetItemText(index,0);//�õ���ǰ����
	CString CurrentBaseItemName = m_ListDest.GetItemText(index,2);//�õ���ǰ����
	CString TopItemName;
	CString TopItemBaseName;
	if(index-1>-1)
	{
		TopItemName = m_ListDest.GetItemText(index-1,0);//�õ�ǰһ�������
		TopItemBaseName = m_ListDest.GetItemText(index-1,2);//�õ�ǰһ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int index = m_ListDest.GetSelectionMark();
	CString CurrentItemName = m_ListDest.GetItemText(index,0);//�õ���ǰ����
	CString CurrentBaseItemName = m_ListDest.GetItemText(index,2);//�õ���ǰ����
	CString BotItemName;
	CString BotBaseItemName;
	if(index+1<m_ListDest.GetItemCount())
	{
		BotItemName = m_ListDest.GetItemText(index+1,0);//�õ�hou һ�������
		BotBaseItemName = m_ListDest.GetItemText(index+1,2);//�õ�hou һ�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//��ʼ��������*.dll���͵��ļ�
	BOOL bW=Finder.FindFile(findpath);
	//���ļ���
	//������ʼ�����ļ� ���һ���ļ�����0
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
	m_ListSrc.InsertColumn(0,_T("����"),LVCFMT_CENTER,140);
	 


	m_ListDest.InsertColumn(0,_T("����"),LVCFMT_CENTER,140);
	m_ListDest.InsertColumn(1,_T("���"),LVCFMT_CENTER,80);
	m_ListDest.InsertColumn(2,_T("BaseType"),LVCFMT_CENTER,80); 
}


void TestItemManage::SaveDestList(void)
{
	//1 ��дdll�б�
	vector<CString>* nameList = pCcmCtrl->Cam[CamID].pConfig->GetDllNamelist(); 
	nameList->clear();//��յ�ǰ�б�
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		nameList->push_back(m_ListDest.GetItemText(i,0));//����д��
	} 
	//1 �����б���嵥���¼���DLL
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

	//2 ��������ѡ��Ի���
	 ::SendMessage(AfxGetMainWnd()->m_hWnd,WM_CREATESETTINGDLG,0,0);  //lifangsen 0710

	 nameList->clear();//��յ�ǰ�б�
	for (int i=0; i < m_ListDest.GetItemCount(); i++)
	{
		nameList->push_back(m_ListDest.GetItemText(i,0)+ _T("@")+ m_ListDest.GetItemText(i,2));//����д��  ����basetype��Ϣ
	} 
	// ����д��config�ļ�
	pCcmCtrl->Cam[CamID].pConfig->WriteDllName();

	
}

int TestItemManage::LoadConfigToDestList(void)
{
	//��ȡ����CONFIG�ļ������е�DLL�����б�
	vector<CString>* nameList = pCcmCtrl->Cam[CamID].pConfig->GetDllNamelist();

	UpdateTestItem(nameList);
	 
	return 0;
}

//ɾ��CurrentTestItem����Ĳ�������¿���
int TestItemManage::UpdateTestItem(vector<CString>* nameList)
{ 
	CString findpath = DestDir+_T("*.dll");
	 
	CFileFind Finder;
	//��ʼ��������*.dll���͵��ļ�
	BOOL bW=Finder.FindFile(findpath);
	//���ļ���
	//������ʼ�����ļ� ���һ���ļ�����0
	 
	while(bW)
	{
		bW = Finder.FindNextFile(); 
		{
			if(Finder.IsDirectory()) continue;  //�����Ŀ¼������ȥ
			if(Finder.IsDots()) continue;       //����ǵ��ļ�������ȥ
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

  