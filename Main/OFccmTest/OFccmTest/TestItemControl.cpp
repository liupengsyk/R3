#include "StdAfx.h"
#include "TestItemControl.h"
#include "OFccmTest.h"


TestItemControl::TestItemControl(OFccmControl* pTccmControl, HWND hButtonPanel,int CamID)
{
	m_pTccm = pTccmControl;
	m_hButtonPanel = hButtonPanel;
	this->CamID =CamID;
	//���ó�ʼ�Ĳ���ģʽ
	//m_TestMode = TESTMODE_MANUAL; //��ʼĬ��Ϊ�ֶ�ģʽ
	//SetTestMode(m_TestMode);//������Ϣ����UI�����ѡ��
	m_TestMode = m_pTccm->Cam[CamID].pConfig->GetGeneralTab()->m_TestMode; //TESTMODE_MANUAL =0

	InitializeCriticalSection(&m_csLock);
 
	hSingleTestComplete = CreateEvent(NULL, TRUE, TRUE, NULL);
	CurrentTestItem_ID = -1;
	
	TestStep      = STEP_NULL;
	TerminateFlag = FALSE;
	LastID = 0;

	ITEM_ID_FIRST = 1;
	 

	CString ExeDir = m_pTccm->Config_GetExeDir();

	//��ʼ��DLL·��
	DllPath = ExeDir + _T("CurrentTestItem\\");
	//����DLL·��
	CreateDirectory(DllPath,NULL);
	bTestResult = FALSE;
	isLoadingDll = FALSE;

	MachineItemID.clear();
	MachineCurrentItemID = 0;

	Result_ok2 = FALSE;
}


TestItemControl::~TestItemControl(void)
{
	m_pTccm = NULL;
	CloseHandle(hSingleTestComplete);
	 
}

void TestItemControl::EnableButton(BOOL enable)
{
	for(UINT i = 0; i < VecTestItem.size(); i++)
	{
		::EnableWindow(VecTestItem[i].hButton,enable);
	}
}

void TestItemControl::LoadInspectionItem(vector<CString>* dllnameList)
{
	FreeAll();
	int ID = ITEM_ID_FIRST;
	TestItemCnt=dllnameList->size();//�������д���
	for (UINT i=0;i<dllnameList->size();i++)
	{
		CString dllname = dllnameList->at(i);
		HMODULE hdll = LoadDLL(DllPath,dllname);
		if (hdll == NULL) 
		{
			hdll = LoadDLL(DllPath,dllname);//�ڶ���Load

			if(hdll == NULL)
			{
				CString Notice;
				Notice.Format(_T(" NO DLL\n   %s"), dllname);
				AfxMessageBox(Notice);
				exit(TRUE);
// 				m_pTccm->Ctrl_Addlog(CamID,Notice,COLOR_RED,200);			 
// 				dllnameList->erase(dllnameList->begin() +i);  //dllnameList->begin() +i	
// 
// 				i--;
// 				continue;
			}
			
		}

		//���庯��ָ��
		typedef TestItemInterface* (*ADDPROC)(ccmBaseInterface*,LPCTSTR);
		//�õ�dll�ӿں�����ַ
		ADDPROC GetTestItemAddr = (ADDPROC)GetProcAddress(hdll,"CreateTestItem");//��dll�е�����һ�¡�
		//ִ��dll�ӿں���
		if (!GetTestItemAddr)
		{
			ADDPROC GetTestItemAddr = (ADDPROC)GetProcAddress(hdll,"CreateTestItem");//��dll�е�����һ�¡�//�ڶ��ε���

			if (!GetTestItemAddr)
			{
				AfxMessageBox(_T("GET DLL ADDR FAIL"));
				exit(TRUE);
				//continue;
			}
		}
		//�õ��ļ�Title
		CString DllTitle = dllname.Left(dllname.ReverseFind('.'));
		ccmBaseInterface* pInterface = dynamic_cast<ccmBaseInterface*> (pCcmCtrl);

		TestItemInfo m_TestItemInfo;
		m_TestItemInfo.bResult = FALSE;
		m_TestItemInfo.bCompelete = FALSE;
		m_TestItemInfo.vec_Data.clear();
		m_TestItemInfo.Test_ID = i+1;
		m_TestItemInfo.strName = DllTitle;
		m_TestItemInfo.baseDllName = DllTitle;
		vecTestItemInfo.push_back(m_TestItemInfo);
		//����DLL�ӿں���
		TestItemInterface* pItem = NULL;
		TRY
		{
			pItem = GetTestItemAddr(pInterface,DllTitle);
			pItem->SetCamID(CamID);
			pItem->InitItem();
		}
		CATCH(CException,e)
		{
			AfxMessageBox(_T("DLL FAIL ADD"));
			exit(TRUE);
			//continue;
		}
		END_CATCH

		MyTestItem item; //ע��VECTOR����������ݵ������ͱ��������
		item.pItem   = pItem;
		item.hdll    = hdll;

		if (pItem->GetType() == TestItemInterface::TYPE_INIT)
		{
			item.ID      = 0;
			item.hButton = NULL;
		}
		else 
		{			
			item.ID = ID;
			item.hButton = CreateTestButton(&item,ID);
			ID++;
		}
		
		VecTestItem.push_back(item);
		m_pTccm->Ctrl_Addlog(CamID,_T("ADD ") + dllname,COLOR_GREEN,200);
		pInterface = NULL;
		pItem = NULL;
		//m_pTccm->Addlog(_T("�Ѽ��� ") ,COLOR_GREEN,200);

	}
	//����ID��ŵ����ֵ
	TestItemCnt=VecTestItem.size();//
	ITEM_ID_LAST = ITEM_ID_FIRST + VecTestItem.size();
	LoadAllOption();
	SaveAllOption();	
	isLoadingDll = TRUE;
}
 
HMODULE TestItemControl::LoadDLL(CString dllPath,CString dllName)
{
	CString DllFullPath = DllPath + dllName;
	 
	BOOL bFind =  pCcmCtrl->isFileExist(DllFullPath); 
	if(!bFind) return NULL;
		
	HMODULE hdll = ::LoadLibrary(DllFullPath);
	if(hdll)
	{ 
		return hdll;
	} 
	return NULL;
}
HWND TestItemControl::CreateTestButton(MyTestItem* pMyTestItem,int ID)
{
	MyTestButton* btn = new MyTestButton(CamID);
	btn->MaxItemSize=23;
	LPCTSTR str = pMyTestItem->pItem->GetName();
	CString BtnName;
	BtnName.Format(_T("%d %s"), ID, str);

	btn->m_ID = ID;
	btn->Create(BtnName,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON|WS_GROUP,CRect(0,0,1,1),CWnd::FromHandle(m_hButtonPanel),NULL);
	btn->SendMessage(WM_INSPECTIONBUTTON_ALIGNWNDPOS); //����λ��
	HWND hWnd_ = btn->GetSafeHwnd();
	btn = NULL;
	return 	hWnd_;
}

void TestItemControl::LoadAllOption()
{
	for(UINT i = 0; i < VecTestItem.size(); i++)
	{
		VecTestItem[i].pItem->LoadOption();
	}
}

int TestItemControl::SaveAllOption()
{
	int error = 0;
	for(UINT i = 0; i < VecTestItem.size(); i++)
	{
		int ret = VecTestItem[i].pItem->SaveOption();
		if (ret!=0)
		{
			error++;
		}
	}
	return error;
}

 
//���Է���Ϊ������߲��Խ���ֹͣ����
int TestItemControl::Run()
{
	int nRet = STEP_NULL;
	//m_TestMode = 2;
	switch(m_TestMode)
	{
	case TESTMODE_AUTO:
		{
			if (OFccmControl::globalset.bFocus == TRUE)
			{
				nRet = FocusAutoTest();
			}
			else if (m_pTccm->Ctrl_GetCamType() == 1)
			{
				nRet = DualCameraAutoTest();
			}
			else
			{
				nRet = AutoTest();
			}
			
			break;
		}
	case  TESTMODE_MANUAL:
		{
			nRet = ManualTest();
			break;
		}
	case  TESTMODE_MACHINETEST:
		{
			nRet = MachineTest();
			break;
		}
	case  TESTMODE_AATEST:
		{
			nRet =AATest();
			break;
		}	 
	} 
	return nRet;
}

 

void TestItemControl::MachineTestInit()
{
	SetEvent(hSingleTestComplete);
	 
	CurrentTestItem_ID = -1;
	TestStep = STEP_START;

	MachineItemID.clear();
	MachineCurrentItemID = 0;

	
	Result_ok2 = FALSE;
}
int TestItemControl::MachineTest()
{
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (item == NULL)		return SINGLETEST_NULL;
		 
	int ret = SingleItemTest(item);
	 if (  ret == SINGLETEST_COMPLETE 
		 || ret == SINGLETEST_ERROR
		 || ret == SINGLETEST_NULL ) 
	 { 	    
		 if(MachineItemID.size()<1) 
		 {
			 MachineTestInit();
			 return 0;
		 }
		 MachineCurrentItemID++; 

		 int Item_TestResult = item->pItem->GetResult();
		 if (Item_TestResult != TestItemInterface::RESULT_FAIL)//���TestItem����Pass��������+1
		 {
			 pCcmCtrl->Cam[CamID].TestItemPass++;
		 }

		 CString str;
		 str.Format(_T("machine test ID :%d"),MachineCurrentItemID);
		 m_pTccm->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

		 if (Item_TestResult == TestItemInterface::RESULT_FAIL)
		 {
			 MachineTestInit();
			 SendMachineTestResult(FALSE);

			  item = NULL;
			 return -1;
		 }
		 else if( MachineCurrentItemID == MachineItemID.size() )
		 {
			 MachineTestInit();
			 SendMachineTestResult(TRUE);

			 item = NULL;
			 return 0;
		 }

		 if(MachineItemID[MachineCurrentItemID]<=VecTestItem.size())
		 {
			 TestStep = STEP_START;
			 CurrentTestItem_ID = MachineItemID[MachineCurrentItemID];
		 }
		 else
		 {
			 
			 MachineTestInit();
			 SendMachineTestResult(TRUE);

			 item = NULL;
			 return 0;

		 }
		

	 }
	 item = NULL;
	 return 0;
}

void TestItemControl::SetMachineItemID(vector<int> MachineItemID)
{
	if(MachineItemID.size()<1) 
	{
		SendMachineTestResult(TRUE);
	}
	else
	{		
		MachineTestInit();
		this->MachineItemID = MachineItemID;
		MachineCurrentItemID = 0;
		int ID = MachineItemID[MachineCurrentItemID];
		if(ID<(VecTestItem.size()+1))
		{
			 
			CurrentTestItem_ID = ID;
			 
		}
		else
		{
			SendMachineTestResult(TRUE);
		}
		  
	}
}

void TestItemControl::SendMachineTestResult(BOOL bResult)
{
   m_pTccm->SendMachineTestResult(CamID,bResult);
}


void TestItemControl::InitAutoTest()
{
	SetEvent(hSingleTestComplete);
		 
	CurrentTestItem_ID = 1;
	TestStep = STEP_START;
	 

	Result_ok2 = FALSE;
}
//�Զ����Է���Ϊ������߲��Խ���ֹͣ����
//�ֶ����Բⲻ��ͣ
int TestItemControl::AutoTest()
{ 
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	
	int ret = SingleItemTest(item);
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//ִ�в��Բ����ؽ��
	 if (  ret == SINGLETEST_COMPLETE 
		 || ret == SINGLETEST_ERROR
		 || ret == SINGLETEST_NULL )
	{

		 
		if (ret == SINGLETEST_NULL)
		{
			item = NULL;
			CurrentTestItem_ID =-1;
		}		
		TestStep = STEP_START; 
		CurrentTestItem_ID ++;

		int Item_TestResult = item->pItem->GetResult();
		if (Item_TestResult != TestItemInterface::RESULT_FAIL)
		{
			pCcmCtrl->Cam[CamID].TestItemPass++;
		}
		 

		//�����һ��������Ŀ��
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//���ٽ��������
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{				
				if(m_pTccm->Cam[CamID].usedMes &&(m_pTccm->Cam[CamID].NGTestItemID>0) /*m_pTccm->Cam[CamID].bSuccessMes*/)
				{
					DrawingAndShowResult(/*item*/GetInspectionItem_FromID(m_pTccm->Cam[CamID].NGTestItemID),TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=ITEM_ID_LAST-1;
					DrawingAndShowResult(item,TRUE,FALSE);
				} 
			}
			else if(Result_ok2)
			{
				DrawingAndShowResult(item,TRUE,TRUE);
			}
			else
			{				
				if (m_pTccm->Cam[CamID].usedMes&&(m_pTccm->Cam[CamID].NGTestItemID>0))
				{
					DrawingAndShowResult(GetInspectionItem_FromID(m_pTccm->Cam[CamID].NGTestItemID),TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=0;
					DrawingAndShowResult(item,TRUE,TRUE);
				}
			}	

			if(OFccmControl::globalset.OffLineTest)
			{
				m_pTccm->Cam[CamID].bState.bCameraStandby = TRUE;
			}	
		}
		else
		{
			//SetEvent(hSingleTestComplete);
			//������NG,���أ�ֹͣ����
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{
				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //����MesUpdata������Ŀ��һ�߻�ִ��MesUpdata
				{
					//Add for MesCheck NG return directly
					CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);
					str.Format(L"NG Info = %s", strErrorInfo);
					pCcmCtrl->Ctrl_Addlog(CamID, str, COLOR_RED, 200);
					if (-1 != strErrorInfo.Find(L"MesCheck Fail"))
					{
						CurrentTestItem_ID = -1;
						DrawingAndShowResult(item,TRUE,FALSE);
						item = NULL;
						return -1;
					}
					CurrentTestItem_ID = ITEM_ID_LAST-1;//MesUpdata
				}
				else		   
				{
					CurrentTestItem_ID = -1;
			
				    DrawingAndShowResult(item,TRUE,FALSE);
			        item = NULL;
				    return -1;
				}
			}
			else if(Item_TestResult == TestItemInterface::RESULT_NORMAL)
			{
				CurrentTestItem_ID = -1;

				DrawingAndShowResult(item,TRUE,2);
				item = NULL;
				return -1;
			}
		}
		item = NULL;
		return 1;
	}
	item = NULL;
	return -1;
}

void TestItemControl::DualCameraAutoTestTryToWaitPeerCameraTestFinished(int CamID)
{
	if (CamID > 1)
	{
		return;
	}
	//˫��ʱ����Ҫ�ȴ��Զ��Ƿ������
	for (int iWait=0;iWait<OFccmControl::globalset.m_iDualAutoTestWaitPeerMaxTime/50;iWait++)
	{
		int iPeerCamStat = pCcmCtrl->Cam[1-CamID].iDualAutoTestStatus; 
		if (iPeerCamStat != DUAL_AUTO_TEST_TESTING)//��һ��ģ���Ƿ��ڲ��ԣ����ڲ���״̬��һ��˵��������
		{
			break;
		}
		Sleep(50);
	}
}

//˫��ģ���Զ�����
int TestItemControl::DualCameraAutoTest()
{ 
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		
	{
		pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
		return SINGLETEST_NULL;
	}

	pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_TESTING;
	int ret = SingleItemTest(item);
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//ִ�в��Բ����ؽ��
	if (  ret == SINGLETEST_COMPLETE 
		|| ret == SINGLETEST_ERROR
		|| ret == SINGLETEST_NULL )
	{
		if (ret == SINGLETEST_NULL)
		{
			item = NULL;
			CurrentTestItem_ID =-1;
		}		
		TestStep = STEP_START; 
		CurrentTestItem_ID ++;

		int Item_TestResult = item->pItem->GetResult();
		if (Item_TestResult != TestItemInterface::RESULT_FAIL)
		{
			pCcmCtrl->Cam[CamID].TestItemPass++;
		}


		//�����һ��������Ŀ��
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//���ٽ��������

			pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;

			DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);


			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{				
				if(m_pTccm->Cam[CamID].usedMes&&m_pTccm->Cam[CamID].bSuccessMes)
				{
					DrawingAndShowResult(item,TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=ITEM_ID_LAST-1;
					DrawingAndShowResult(item,TRUE,FALSE);
				} 
			}
			else if(Result_ok2) 
			{
				DrawingAndShowResult(item,TRUE,TRUE);
			}
			else
			{				
				if (m_pTccm->Cam[CamID].usedMes&&(m_pTccm->Cam[CamID].NGTestItemID>0))
				{
					DrawingAndShowResult(GetInspectionItem_FromID(m_pTccm->Cam[CamID].NGTestItemID),TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=0;
					DrawingAndShowResult(item,TRUE,TRUE);
				}
			}	

			if(OFccmControl::globalset.OffLineTest)
			{
				m_pTccm->Cam[CamID].bState.bCameraStandby = TRUE;
			}	
		}
		
		else
		{
			//SetEvent(hSingleTestComplete);
			//������NG,���أ�ֹͣ����
			//xcz add����Զ�NG�ˣ�ҲӦ�ý���if

			if (Item_TestResult == TestItemInterface::RESULT_FAIL || (m_pTccm->Cam[1-CamID].NGTestItemID >= 1))
			{
				if (Item_TestResult == TestItemInterface::RESULT_FAIL)
				{
					//xcz modify��if�жϣ���ֹ�Զ�ʧ�ܣ����Ǳ����Լ�û��ʧ�ܣ����ó�NG
					m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				}	

//				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //����MesUpdata������Ŀ��һ�߻�ִ��MesUpdata
				{
					m_pTccm->Ctrl_Addlog(CamID,_T("��ʼ�ȴ��Զ˲�������mesupdate"),COLOR_BLUE,200);
					//˫��ʱ����Ҫ�ȴ��Զ��Ƿ������
					//xcz modify
					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

					CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);
					str.Format(L"NG Info = %s", strErrorInfo);
					pCcmCtrl->Ctrl_Addlog(CamID, str, COLOR_RED, 200);
					if (-1 != strErrorInfo.Find(L"MesCheck Fail"))
					{
						//xcz add ��MesCheck����ʱ���Զ˲��õ�
						pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
	//					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

						CurrentTestItem_ID = -1;
						DrawingAndShowResult(item,TRUE,FALSE);
						item = NULL;
						return -1;
					}
	//				m_pTccm->Ctrl_Addlog(CamID,_T("��ʼ�ȴ��Զ˲�������mesupdate"),COLOR_BLUE,200);
	//				DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

					CurrentTestItem_ID = ITEM_ID_LAST-1;//MesUpdata
				}
				else		   
				{
					CurrentTestItem_ID = -1;

					pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

					DrawingAndShowResult(item,TRUE,FALSE);
					item = NULL;
					return -1;
				}
			}
			else if (Item_TestResult == TestItemInterface::RESULT_PASS && CurrentTestItem_ID == ITEM_ID_LAST-1 &&
				(m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID))
			{
					m_pTccm->Ctrl_Addlog(CamID,_T("��ʼ�ȴ��Զ˲�������mesupdate"),COLOR_BLUE,200);
					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);
			}
			else if(Item_TestResult == TestItemInterface::RESULT_NORMAL)
			{
				CurrentTestItem_ID = -1;

				pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
			
				//˫��ʱ����Ҫ�ȴ��Զ��Ƿ������				
				m_pTccm->Ctrl_Addlog(CamID,_T("��ʼ�ȴ��Զ˲������"),COLOR_BLUE,200);
				DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);							

				DrawingAndShowResult(item,TRUE,2);
				item = NULL;
				return -1;
			}
		}
		item = NULL;
		return 1;
	}
	item = NULL;
	return -1;
}

int TestItemControl::FocusAutoTest()
{ 
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;


	int ret = SingleItemTest(item);
	if (pCcmCtrl->Cam[CamID].bFocusStatusChange == TRUE)
	{
//		pCcmCtrl->Cam[CamID].TestItemPass++;
//		CurrentTestItem_ID++;
		pCcmCtrl->Cam[CamID].bFocusStatusChange = FALSE;
	}
	else if (pCcmCtrl->Cam[CamID].bFocusEnd == FALSE && pCcmCtrl->Cam[CamID].bFocusStatusChange == FALSE )
	{
		SetButtonStatus( item, BUTTON_RUNNING);    //����BUTTONΪ����״̬

		ret = 1;
		TestStep = STEP_TEST;
	}
	else
	{
		;
	}
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//ִ�в��Բ����ؽ��
	if (  ret == SINGLETEST_COMPLETE 
		|| ret == SINGLETEST_ERROR
		|| ret == SINGLETEST_NULL )
	{


		if (ret == SINGLETEST_NULL)
		{
			item = NULL;
			CurrentTestItem_ID =-1;
		}		
		TestStep = STEP_START; 
		CurrentTestItem_ID ++;

		int Item_TestResult = item->pItem->GetResult();
		if (Item_TestResult != TestItemInterface::RESULT_FAIL)
		{
			pCcmCtrl->Cam[CamID].TestItemPass++;
		}


		//�����һ��������Ŀ��
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//���ٽ��������
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{				
				if(m_pTccm->Cam[CamID].usedMes &&(m_pTccm->Cam[CamID].NGTestItemID>0) /*m_pTccm->Cam[CamID].bSuccessMes*/)
				{
					DrawingAndShowResult(/*item*/GetInspectionItem_FromID(m_pTccm->Cam[CamID].NGTestItemID),TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=ITEM_ID_LAST-1;
					DrawingAndShowResult(item,TRUE,FALSE);
				} 
			}
			else if(Result_ok2)
			{
				DrawingAndShowResult(item,TRUE,TRUE);
			}
			else
			{				
				if (m_pTccm->Cam[CamID].usedMes&&(m_pTccm->Cam[CamID].NGTestItemID>0))
				{
					DrawingAndShowResult(GetInspectionItem_FromID(m_pTccm->Cam[CamID].NGTestItemID),TRUE,FALSE);
				}
				else
				{
					m_pTccm->Cam[CamID].NGTestItemID=0;
					DrawingAndShowResult(item,TRUE,TRUE);
				}
			}	

			if(OFccmControl::globalset.OffLineTest)
			{
				m_pTccm->Cam[CamID].bState.bCameraStandby = TRUE;
			}	
		}
		else
		{
			//SetEvent(hSingleTestComplete);
			//������NG,���أ�ֹͣ����
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{
				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //����MesUpdata������Ŀ��һ�߻�ִ��MesUpdata
				{
					//Add for MesCheck NG return directly
					CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);
					str.Format(L"NG Info = %s", strErrorInfo);
					pCcmCtrl->Ctrl_Addlog(CamID, str, COLOR_RED, 200);
					if (-1 != strErrorInfo.Find(L"MesCheck Fail"))
					{
						CurrentTestItem_ID = -1;
						DrawingAndShowResult(item,TRUE,FALSE);
						item = NULL;
						return -1;
					}
					CurrentTestItem_ID = ITEM_ID_LAST-1;//MesUpdata
				}
				else		   
				{
					CurrentTestItem_ID = -1;

					DrawingAndShowResult(item,TRUE,FALSE);
					item = NULL;
					return -1;
				}
			}
			else if(Item_TestResult == TestItemInterface::RESULT_NORMAL)
			{
				CurrentTestItem_ID = -1;

				DrawingAndShowResult(item,TRUE,2);
				item = NULL;
				return -1;
			}
		}
		item = NULL;
		return 1;
	}
	item = NULL;
	return -1;
}


void TestItemControl::InitAATest(int TestItemID)
{
	SetEvent(hSingleTestComplete);

	CurrentTestItem_ID = TestItemID;
	TestStep = STEP_START;
	 
	Result_ok2 = FALSE;
}
int TestItemControl::AATest()
{
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	int ret = SingleItemTest(item);
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//ִ�в��Բ����ؽ��
	 if (  ret == SINGLETEST_COMPLETE 
		 || ret == SINGLETEST_ERROR
		 || ret == SINGLETEST_NULL )
	{
		if (ret == SINGLETEST_NULL)
		{
			item = NULL;
			CurrentTestItem_ID =-1;
		}		
		 
		 
		TestStep = STEP_START; 
		CurrentTestItem_ID ++;

		int Item_TestResult = item->pItem->GetResult();
		if (Item_TestResult != TestItemInterface::RESULT_FAIL)
		{
			pCcmCtrl->Cam[CamID].TestItemPass++;
		}
		 

		//�����һ��������Ŀ��
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{
				if(CamID==1&&m_pTccm->Cam[CamID].pServerSocket!=NULL)
				{
					if(m_pTccm->Cam[CamID].pServerSocket->m_client!=NULL)
					m_pTccm->Cam[CamID].pServerSocket->m_client->Send("FAIL\n",5);
					m_pTccm->Ctrl_Addlog(CamID,_T("SEND FAIL"),COLOR_RED,200);
				}
				
				DrawingAndShowResult(item,TRUE,FALSE);
			} 
			else
			{
				if(CamID==1)
				{
					if(m_pTccm->Cam[CamID].pServerSocket==NULL)
					{
						item = NULL;
						return 1;
					}
					if(m_pTccm->Cam[CamID].pServerSocket->m_client!=NULL)
					m_pTccm->Cam[CamID].pServerSocket->m_client->Send("PASS\n",5);
					m_pTccm->Ctrl_Addlog(CamID,_T("SEND PASS"),COLOR_BLUE,200);
				}
				DrawingAndShowResult(item,TRUE,TRUE);
			} 
			CurrentTestItem_ID = -1;//���Խ�������Ȼ�����������  2016.01.05
		}	
		item = NULL;
		return 1;
	}
	 item = NULL;
	return -1;

}
 
int TestItemControl::ManualTest()
{
	//����ID������Ŀ
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	//ִ�в��Բ����ؽ��
	int ret = SingleItemTest(item);

	if(ret == SINGLETEST_COMPLETE 
		|| ret == SINGLETEST_ERROR
		|| ret == SINGLETEST_NULL)
	{
		int ID = CurrentTestItem_ID;
		
		InitManualTestStatus();
		if (!pCcmCtrl->isOffline(CamID))
		{
			if( ID == VecTestItem.size()) 
			{
				if (item->pItem->GetResult() == TestItemInterface::RESULT_FAIL)
				{
					DrawingAndShowResult( item, FALSE, FALSE); 
				}
				else
				{
					DrawingAndShowResult( item, FALSE, TRUE); 
				}				
			}
		}
		item = NULL;
		return 1;
	} 	 
	item = NULL;
	return ret;
}

void TestItemControl::InitManualTestStatus()
{
	TestStep = STEP_START;
	SetEvent(hSingleTestComplete);
	CurrentTestItem_ID = -1;	 
}

void TestItemControl::DrawingAndShowResult(MyTestItem* item,BOOL bCloseCamera,BOOL bResult)
 {
	CString str;
		if (Result_ok2)
		{ 
			str.Format(_T("[%s] Pass!!"), item->pItem->GetName());
			m_pTccm->Ctrl_Addlog(CamID,str, COLOR_GREEN, 220);
			bTestResult = TRUE;
			Sleep(100); 
		}
		else if(bResult)
		{ 
// 			if (bResult==2)
// 			{
// 				str.Format(_T("[%s] Pass!!"), item->pItem->GetName());
// 				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_GREEN, 220);
// 			}
// 			else
// 			{
				str.Format(_T("[%s] Pass!!"), item->pItem->GetName());
				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_GREEN, 220);
//			}
			bTestResult = TRUE; 
		}
		else
		{  			
			CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);  //�Զ�ģʽNGʱ�ж�ErroInfo�Ƿ�δ���ã����û����������ʾ��ӦItem TEST FAIL
			str.Format(L"NG Info = %s", strErrorInfo);
			pCcmCtrl->Ctrl_Addlog(CamID, str, COLOR_RED, 200);

			if (strErrorInfo.Find(_T("PASS")) != -1 ||
				strErrorInfo.Find(_T("OK")) != -1 ||
				strErrorInfo == _T(""))
			{
				strErrorInfo = _T("TEST FAIL!");
			}
			if (strErrorInfo == "TEST FAIL!")
			{
				strErrorInfo.Format(L"[%s]\nTEST_FAIL!",item->pItem->GetName());
				pCcmCtrl->Ctrl_Addlog(CamID,strErrorInfo,COLOR_RED, 200);
				pCcmCtrl->Ctrl_SetCamErrorInfo(CamID, strErrorInfo);
			}
			else
			{
				str.Format(_T("[%s] Fail!!"), item->pItem->GetName());
				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_RED, 220);
				if (m_pTccm->Cam[CamID].pConfig->GetGeneralTab()->m_TestMode == TESTMODE_AUTO)
				{
					pCcmCtrl->Ctrl_SetCamErrorInfo(CamID, str);  //�Զ�ģʽ�²���������Ҫ
				}
			}
			bTestResult = FALSE; 
		}
		Sleep(100);	
		if((OFccmControl::globalset.CamType==L"Dual")&&(m_TestMode==TESTMODE_AUTO))
		{
			
			if(bTestResult==FALSE)//������һ��cam����ʧ��
			{
				pCcmCtrl->Ctrl_Addlog(CamID,L"����һ��NG�ر�DualCam", COLOR_RED,200);

               for (int i=0;i<2;i++)
               {
				   if (!pCcmCtrl->isStandby(i))
				   {
					    if(bCloseCamera) m_pTccm->CloseCamera(i,bTestResult);
				   }
				  
               }
			}
			else
			{
				if((m_pTccm->Cam[0].pTestCtrl->GetCurrentID()==-1)&&(m_pTccm->Cam[1].pTestCtrl->GetCurrentID()==-1))//������������
				{
					pCcmCtrl->Ctrl_Addlog(CamID, L"��������ر�ģ��", COLOR_RED, 200);
					for (int i=0;i<2;i++)
					{
						if(m_pTccm->Cam[0].pTestCtrl->bTestResult==FALSE||m_pTccm->Cam[1].pTestCtrl->bTestResult==FALSE)//��ֹCam 0��1ͬʱ���������һ��NG,һ��OK���������
						{
							if(bCloseCamera) m_pTccm->CloseCamera(i,FALSE);
							//m_pTccm->DrawingAndShowResult(i,_T("TEST FAIL"),FALSE,Result_ok2);
						}
						else
						{
							if(bCloseCamera) m_pTccm->CloseCamera(i,bTestResult);
							//m_pTccm->DrawingAndShowResult(i,str,bResult,Result_ok2);
							//m_pTccm->DrawingAndShowResult(i,_T("TEST PASS"),bResult,Result_ok2);
						}
					}
				}
				
			}
		}
		else if (m_TestMode==TESTMODE_AATEST)
		{
			m_pTccm->Ctrl_Addlog(CamID,_T("Mark"),COLOR_BLUE);
		}
		else
		{
			if(bCloseCamera) m_pTccm->CloseCamera(CamID,bTestResult);
		//	m_pTccm->DrawingAndShowResult(CamID,str,bResult,Result_ok2);
		}
}

int TestItemControl::SingleItemTest(MyTestItem* item)
{
	CString str;
	 
	TRY 
	{
		if (TerminateFlag)
		{
			 
			TestStep = STEP_TERMINATE;
			 
		}

		switch(TestStep)
		{
		case STEP_START: //��Ŀ���� ǰ �ĳ�ʼ����
			{
				ResetEvent(hSingleTestComplete); //���źţ�������
				m_TimeStart = GetTickCount();  //��¼��ʼʱ��
				 
				TestStep = STEP_INITLIZE;	
				LastID = item->ID;  //��¼��һ�β���ID��
				//��ʾ
				
				str.Format(_T("[%s]"), item->pItem->GetName());
				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_BLUE);				 
 			}

		case STEP_INITLIZE: //��Ŀ���Եĳ�ʼ��
			{ 
				SetButtonStatus( item, BUTTON_RUNNING);    //����BUTTONΪ����״̬
				item->pItem->SetResult(TestItemInterface::RESULT_NULL); //Ĭ�Ͻ������ΪNULL			 
				
				int nRet = item->pItem->Initialize();   //ִ�г�ʼ��
				if(nRet != 0)
				{
					return nRet;     //�����Ϊ0����ʼ��δ��ϣ�����
				}				 
				TestStep = STEP_TEST;     //���Ϊ0��������һ�����ڡ�
				 
			}

		case STEP_TEST: //��Ŀ��ʽ����
			{
				//ִ�в���
				int nRet = item->pItem->Testing();    //ִ�в��� 
				 
				if(nRet != 0 )
				{
					return nRet;     //�����Ϊ0������δ��ϣ�����
				}
			 
				TestStep = STEP_FINALIZE;
				 

				//���Ϊ0��������һ�����ڡ�
			}

		case STEP_FINALIZE: //��Ŀ���Խ�����β
			{
				int nRet = item->pItem->Finalize();  //ִ����β����
				if(nRet != 0)	 
				{
					return nRet;  //�����Ϊ0������δ��ϣ�����
				}
				else if (nRet==0)
				{
					//��������
					Save(item);
					ShowTestTime(item);
						 
					TestStep = STEP_COMPLETE;

					ShowItemResult(item); //��ť��ʾ��� 
				} 
			}
		case STEP_COMPLETE: //���Խ�������
			{				 
				SetEvent(hSingleTestComplete); 

				return SINGLETEST_COMPLETE;
			}
		case STEP_TERMINATE://ǿ���жϣ�ִ�н�������һ��
			{				
				item->pItem->Finalize();
				Save(item);                 //��������
				ShowItemResult(item);      //��ť��ʾ���
				ShowTestTime(item);
				SetEvent(hSingleTestComplete); 
				 
				return SINGLETEST_TERMINATE;
			}
		default:
			{
				str.Format(_T("[%s] default"), item->pItem->GetName());
				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_BLUE);
				
				SetEvent(hSingleTestComplete);
				return SINGLETEST_NULL;
			}
		}

		return SINGLETEST_NULL;
	}
	
	CATCH(CException,e)
	{
		AfxMessageBox(_T("DLL FAIL ERROR"));
		TCHAR error[100];
		memset(error,0,sizeof(TCHAR)*100);
		e->GetErrorMessage(error,100);
		CString str;
		str.Format(_T("###########\n DLL FAIL\n,ERROE�� %s \n############"),error);
		m_pTccm->Ctrl_Addlog(CamID,str,COLOR_RED);
		return SINGLETEST_ERROR;
	}
	END_CATCH 

}
 


BOOL TestItemControl::isSingleItemTestRuning(void)
{
	int ret = WaitForSingleObject(hSingleTestComplete,0);
	if(ret == WAIT_TIMEOUT)
		return TRUE;
	else
		return FALSE;
}

void TestItemControl::TerminateSingleTest(int ID)
{
	TerminateFlag = TRUE;
	WaitForSingleObject(hSingleTestComplete,INFINITE);	 
	TerminateFlag = FALSE;
}

MyTestItem* TestItemControl::GetInspectionItem_FromID(int ID)
{
	 
	if (ID <= 0)
	{  
		return NULL;
	}
	if(ID<(VecTestItem.size()+1))
	{
		for(UINT i = 0; i < VecTestItem.size(); i++)
		{
			if(ID == VecTestItem[i].ID)
			{
			 
				return &VecTestItem[i];
			}
		}
	}
	 
	return NULL;
}

void TestItemControl::ShowTestTime(MyTestItem* item)
{
	//��ʾʱ��
	m_TimeEnd = GetTickCount();
	CString time;
	time.Format(_T("[%s] END \n TIME�� %d ms"),item->pItem->GetName(), m_TimeEnd - m_TimeStart);
	m_pTccm->Ctrl_Addlog(CamID,time, RGB(50,50,50));
}

int TestItemControl::PostMessageToItems(DWORD Msg)
{
	for(UINT i = 0; i < VecTestItem.size(); i++)
	{
		if(VecTestItem[i].pItem) 
		{
			::PostMessage(VecTestItem[i].pItem->GetOptionDialogHandle(), Msg, 0, 0);
		}
		
	}
	return 0;
}

 

void TestItemControl::SetButtonStatus(MyTestItem* item,int Flag)
{
	if (!item)		return;
	::PostMessage(item->hButton,WM_INSPECTIONBUTTON_SHOWSTATUS,Flag,0);
}

int TestItemControl::Save(MyTestItem* item)
{
	//  ReportPath 
	if (!item->pItem->isSaved()) return -2;

	BOOL bLocal = FALSE;
	bLocal = m_pTccm->Cam[CamID].pConfig->GetGeneralTab()->SaveReport_EnLocal;	 
	//if(!bLocal) return -1; 
	//�õ���¼���ļ���
	CString name        = item->pItem->GetName();
	CString Result      = item->pItem->GetResult()?_T("Pass"):_T("Fail");
	CString Headers     = item->pItem->GetReportHeads();
	CString LowerLimits = item->pItem->GetReportLowLimit();
	CString UpperLimits = item->pItem->GetReportHightLimit();
	CString Contents    = item->pItem->GetReportContents();
		
// 	if (bLocal)
// 	{
	if(m_pTccm->Cam[CamID].pConfig->GetGeneralTab()->sReportPath!=_T(""))
	{
		m_pTccm->SaveReport(name,Headers,LowerLimits,UpperLimits,Contents,Result,CamID);
	}
//	}	 
	 
	return 0;
}

void TestItemControl::ShowItemResult(MyTestItem* item)
{
	//���ˣ��õ����Խ��
	switch(item->pItem->GetResult())//���ݽ��BUTTON��ʾ״̬
	{
	case TestItemInterface::RESULT_PASS:
    case TestItemInterface::RESULT_NORMAL:    
		SetButtonStatus(item,BUTTON_PASS);		
		break;
	case TestItemInterface::RESULT_FAIL:
		SetButtonStatus(item,BUTTON_FAIL);
		break;
	case TestItemInterface::RESULT_NULL:
		SetButtonStatus(item,BUTTON_NULL);
		break;
	case TestItemInterface::RESULT_OK2:
		SetButtonStatus(item,BUTTON_PASS);
		break;
	default:
		SetButtonStatus(item,BUTTON_RESET);
		break;
	}
}

vector<MyTestItem>* TestItemControl::GetMyTestItemArray()
{
	return &VecTestItem;
}
void TestItemControl::FreeAll()
{ 
	for (UINT i=0;i<VecTestItem.size();i++)
	{
		VecTestItem[i].Free();
	}
	VecTestItem.clear();
		
}

void TestItemControl::SetTestItem(int ID)
{
	//int Flag = m_pTccm->QueryCurrentStatus(CamID);
	//switch (Flag)
	//{
	//case STATUS_OFFLINE: //����ģʽ(��̬ͼƬ�����ֶ�����)��ֱ���л�������Ŀ
	//	break;
	//case STATUS_RUNNING: 
	//	break;
	//default:  //����״̬�������Ч
	//	return;
	//}
	CString str;
	str.Format(_T("set testItemID:%d"),ID);
	m_pTccm->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	//ȷ��״̬	
	//�����ǰ�����������У�
	if(isSingleItemTestRuning())
	{	
		str.Format(_T("Stop testItemID:%d"),CurrentTestItem_ID);
		m_pTccm->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

		//ͬʱ��Ӧ��ID�͵�ǰID��ͬһ����
		if (ID == CurrentTestItem_ID) //���жϣ����жϣ�����Ū��˳���ֹ��ͬ��
		{ 		
			//ִ��ǿ�ƽ���
			TerminateSingleTest(CurrentTestItem_ID); // ֱ�ӷ���
			if (m_pTccm->isOffline(CamID))//ˢ�¾�̬����
				m_pTccm->Cam[CamID].pImgExplorer->ShowScrImage();
			//return; //����
		}
		else
		{
			//ִ��ǿ�ƽ���
			TerminateSingleTest(CurrentTestItem_ID); 
		}
	}
	//ˢ�¾�̬����
	if (m_pTccm->isOffline(CamID))
		m_pTccm->Cam[CamID].pImgExplorer->ShowScrImage();
		
	TestInit();
	
	CurrentTestItem_ID = ID;	
	
}

void TestItemControl::ResetALLButtonImage()
{
	for(UINT i = 0; i < VecTestItem.size(); i++)
	{
		SetButtonStatus(&VecTestItem[i],BUTTON_RESET);
	}
}
int TestItemControl::GetLastID(void)
{
	return LastID;
}
int TestItemControl::GetCurrentID(void)
{
	return CurrentTestItem_ID;
}

int TestItemControl::TestInit(int TestItemID)
{
	switch(m_TestMode)
	{
	case TESTMODE_MANUAL:
		 InitManualTestStatus();
		break;
	case TESTMODE_AUTO:
		InitAutoTest();
		break;
	case TESTMODE_MACHINETEST:
		MachineTestInit();
		break;
	case TESTMODE_AATEST:
		InitAATest(TestItemID);
		break;

	}

	 return 0;
}

TestItemInfo TestItemControl::GetTestInfo(UINT TestItemID)
{
	TestItemInfo tInfo;
	//memset(&tInfo, 0, sizeof(TestItemInfo));
	EnterCriticalSection(&m_csLock);
	if((TestItemID >=1)&&(TestItemID<=vecTestItemInfo.size()))
	{
		tInfo = vecTestItemInfo[TestItemID-1]; 
	}
	else
	{
		tInfo.Test_ID = -1;
		CString str;
		str.Format(_T("NO DLL_%d"),TestItemID);
		m_pTccm->Ctrl_Addlog(CamID,str,COLOR_RED);
	}
	LeaveCriticalSection(&m_csLock); 
	return tInfo;
}
TestItemInfo TestItemControl::GetTestInfo(CString itemName)
{
	TestItemInfo tInfo;
	BOOL bFind = FALSE;
	EnterCriticalSection(&m_csLock);
	for(UINT i=0;i<vecTestItemInfo.size();i++)
	{
		if(itemName == vecTestItemInfo[i].strName)
		{
			 tInfo = vecTestItemInfo[i];
			 bFind = TRUE;
			 break;
		}
	}
	 if(!bFind)
	 {
		 tInfo.Test_ID = -1;
	 }
	LeaveCriticalSection(&m_csLock);
	return tInfo;
}

 void TestItemControl::SetTestInfo(int TestItemID,TestItemInfo mTestItemInfo)
 {
	 EnterCriticalSection(&m_csLock);
	 if((UINT)TestItemID<=vecTestItemInfo.size())
	 {
		 vecTestItemInfo[TestItemID-1] = mTestItemInfo;
	 }
	 else
	 {
		 CString str;
		 str.Format(_T("NO DLL_%d"),TestItemID);
		 m_pTccm->Ctrl_Addlog(CamID,str,COLOR_RED);
	 }
	 LeaveCriticalSection(&m_csLock);
 }
 
 