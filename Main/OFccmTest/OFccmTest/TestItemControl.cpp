#include "StdAfx.h"
#include "TestItemControl.h"
#include "OFccmTest.h"


TestItemControl::TestItemControl(OFccmControl* pTccmControl, HWND hButtonPanel,int CamID)
{
	m_pTccm = pTccmControl;
	m_hButtonPanel = hButtonPanel;
	this->CamID =CamID;
	//设置初始的测试模式
	//m_TestMode = TESTMODE_MANUAL; //初始默认为手动模式
	//SetTestMode(m_TestMode);//发送消息更改UI界面的选项
	m_TestMode = m_pTccm->Cam[CamID].pConfig->GetGeneralTab()->m_TestMode; //TESTMODE_MANUAL =0

	InitializeCriticalSection(&m_csLock);
 
	hSingleTestComplete = CreateEvent(NULL, TRUE, TRUE, NULL);
	CurrentTestItem_ID = -1;
	
	TestStep      = STEP_NULL;
	TerminateFlag = FALSE;
	LastID = 0;

	ITEM_ID_FIRST = 1;
	 

	CString ExeDir = m_pTccm->Config_GetExeDir();

	//初始化DLL路径
	DllPath = ExeDir + _T("CurrentTestItem\\");
	//创建DLL路径
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
	TestItemCnt=dllnameList->size();//用于排列窗口
	for (UINT i=0;i<dllnameList->size();i++)
	{
		CString dllname = dllnameList->at(i);
		HMODULE hdll = LoadDLL(DllPath,dllname);
		if (hdll == NULL) 
		{
			hdll = LoadDLL(DllPath,dllname);//第二次Load

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

		//定义函数指针
		typedef TestItemInterface* (*ADDPROC)(ccmBaseInterface*,LPCTSTR);
		//得到dll接口函数地址
		ADDPROC GetTestItemAddr = (ADDPROC)GetProcAddress(hdll,"CreateTestItem");//与dll中的名字一致。
		//执行dll接口函数
		if (!GetTestItemAddr)
		{
			ADDPROC GetTestItemAddr = (ADDPROC)GetProcAddress(hdll,"CreateTestItem");//与dll中的名字一致。//第二次调用

			if (!GetTestItemAddr)
			{
				AfxMessageBox(_T("GET DLL ADDR FAIL"));
				exit(TRUE);
				//continue;
			}
		}
		//得到文件Title
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
		//调用DLL接口函数
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

		MyTestItem item; //注意VECTOR保存对象扩容的析构和本身的析构
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
		//m_pTccm->Addlog(_T("已加载 ") ,COLOR_GREEN,200);

	}
	//计算ID编号的最大值
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
	btn->SendMessage(WM_INSPECTIONBUTTON_ALIGNWNDPOS); //排列位置
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

 
//测试返回为错误或者测试结束停止画面
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
	//根据ID搜索项目
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
		 if (Item_TestResult != TestItemInterface::RESULT_FAIL)//如果TestItem测试Pass，计数器+1
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
//自动测试返回为错误或者测试结束停止画面
//手动测试测不暂停
int TestItemControl::AutoTest()
{ 
	//根据ID搜索项目
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	
	int ret = SingleItemTest(item);
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//执行测试并返回结果
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
		 

		//是最后一个测试项目吗
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//不再进入测试项
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
			//如果结果NG,返回，停止测试
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{
				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //仅有MesUpdata测试项目的一边会执行MesUpdata
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
	//双摄时，需要等待对端是否测试完
	for (int iWait=0;iWait<OFccmControl::globalset.m_iDualAutoTestWaitPeerMaxTime/50;iWait++)
	{
		int iPeerCamStat = pCcmCtrl->Cam[1-CamID].iDualAutoTestStatus; 
		if (iPeerCamStat != DUAL_AUTO_TEST_TESTING)//另一个模组是否还在测试，不在测试状态，一般说明测完了
		{
			break;
		}
		Sleep(50);
	}
}

//双摄模组自动测试
int TestItemControl::DualCameraAutoTest()
{ 
	//根据ID搜索项目
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
	//执行测试并返回结果
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


		//是最后一个测试项目吗
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//不再进入测试项

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
			//如果结果NG,返回，停止测试
			//xcz add如果对端NG了，也应该进此if

			if (Item_TestResult == TestItemInterface::RESULT_FAIL || (m_pTccm->Cam[1-CamID].NGTestItemID >= 1))
			{
				if (Item_TestResult == TestItemInterface::RESULT_FAIL)
				{
					//xcz modify加if判断，防止对端失败，但是本身自己没有失败，给置成NG
					m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				}	

//				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //仅有MesUpdata测试项目的一边会执行MesUpdata
				{
					m_pTccm->Ctrl_Addlog(CamID,_T("开始等待对端测试完再mesupdate"),COLOR_BLUE,200);
					//双摄时，需要等待对端是否测试完
					//xcz modify
					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

					CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);
					str.Format(L"NG Info = %s", strErrorInfo);
					pCcmCtrl->Ctrl_Addlog(CamID, str, COLOR_RED, 200);
					if (-1 != strErrorInfo.Find(L"MesCheck Fail"))
					{
						//xcz add 当MesCheck不过时，对端不用等
						pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
	//					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);

						CurrentTestItem_ID = -1;
						DrawingAndShowResult(item,TRUE,FALSE);
						item = NULL;
						return -1;
					}
	//				m_pTccm->Ctrl_Addlog(CamID,_T("开始等待对端测试完再mesupdate"),COLOR_BLUE,200);
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
					m_pTccm->Ctrl_Addlog(CamID,_T("开始等待对端测试完再mesupdate"),COLOR_BLUE,200);
					DualCameraAutoTestTryToWaitPeerCameraTestFinished(CamID);
			}
			else if(Item_TestResult == TestItemInterface::RESULT_NORMAL)
			{
				CurrentTestItem_ID = -1;

				pCcmCtrl->Cam[CamID].iDualAutoTestStatus = DUAL_AUTO_TEST_FINISHED;
			
				//双摄时，需要等待对端是否测试完				
				m_pTccm->Ctrl_Addlog(CamID,_T("开始等待对端测试完毕"),COLOR_BLUE,200);
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
	//根据ID搜索项目
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
		SetButtonStatus( item, BUTTON_RUNNING);    //设置BUTTON为运行状态

		ret = 1;
		TestStep = STEP_TEST;
	}
	else
	{
		;
	}
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//执行测试并返回结果
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


		//是最后一个测试项目吗
		if( CurrentTestItem_ID == ITEM_ID_LAST) 
		{
			CurrentTestItem_ID = -1;//不再进入测试项
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
			//如果结果NG,返回，停止测试
			if (Item_TestResult == TestItemInterface::RESULT_FAIL)
			{
				m_pTccm->Cam[CamID].NGTestItemID=CurrentTestItem_ID-1;
				if((m_pTccm->Cam[CamID].usedMes)&&(m_pTccm->Cam[CamID].MesID == CamID)) //仅有MesUpdata测试项目的一边会执行MesUpdata
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
	//根据ID搜索项目
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	int ret = SingleItemTest(item);
	if (item->pItem->GetResult() == TestItemInterface::RESULT_OK2) Result_ok2  =TRUE;
	CString str;
	//执行测试并返回结果
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
		 

		//是最后一个测试项目吗
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
			CurrentTestItem_ID = -1;//测试结束，不然还会进来测试  2016.01.05
		}	
		item = NULL;
		return 1;
	}
	 item = NULL;
	return -1;

}
 
int TestItemControl::ManualTest()
{
	//根据ID搜索项目
	MyTestItem* item = GetInspectionItem_FromID(CurrentTestItem_ID);
	if (!item)		return SINGLETEST_NULL;

	//执行测试并返回结果
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
			CString strErrorInfo = pCcmCtrl->Ctrl_GetCamErrorInfo(CamID);  //自动模式NG时判断ErroInfo是否未设置，如果没有设置则显示对应Item TEST FAIL
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
					pCcmCtrl->Ctrl_SetCamErrorInfo(CamID, str);  //自动模式下不良分类需要
				}
			}
			bTestResult = FALSE; 
		}
		Sleep(100);	
		if((OFccmControl::globalset.CamType==L"Dual")&&(m_TestMode==TESTMODE_AUTO))
		{
			
			if(bTestResult==FALSE)//其中有一个cam测试失败
			{
				pCcmCtrl->Ctrl_Addlog(CamID,L"其中一个NG关闭DualCam", COLOR_RED,200);

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
				if((m_pTccm->Cam[0].pTestCtrl->GetCurrentID()==-1)&&(m_pTccm->Cam[1].pTestCtrl->GetCurrentID()==-1))//两个都测试完
				{
					pCcmCtrl->Ctrl_Addlog(CamID, L"都测试完关闭模组", COLOR_RED, 200);
					for (int i=0;i<2;i++)
					{
						if(m_pTccm->Cam[0].pTestCtrl->bTestResult==FALSE||m_pTccm->Cam[1].pTestCtrl->bTestResult==FALSE)//防止Cam 0和1同时进入此区域，一个NG,一个OK，造成误判
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
		case STEP_START: //项目测试 前 的初始动作
			{
				ResetEvent(hSingleTestComplete); //无信号，测试中
				m_TimeStart = GetTickCount();  //记录起始时间
				 
				TestStep = STEP_INITLIZE;	
				LastID = item->ID;  //记录上一次测试ID号
				//提示
				
				str.Format(_T("[%s]"), item->pItem->GetName());
				m_pTccm->Ctrl_Addlog(CamID,str, COLOR_BLUE);				 
 			}

		case STEP_INITLIZE: //项目测试的初始化
			{ 
				SetButtonStatus( item, BUTTON_RUNNING);    //设置BUTTON为运行状态
				item->pItem->SetResult(TestItemInterface::RESULT_NULL); //默认结果设置为NULL			 
				
				int nRet = item->pItem->Initialize();   //执行初始化
				if(nRet != 0)
				{
					return nRet;     //如果不为0，初始化未完毕，返回
				}				 
				TestStep = STEP_TEST;     //如果为0，进入下一个环节。
				 
			}

		case STEP_TEST: //项目正式测试
			{
				//执行测试
				int nRet = item->pItem->Testing();    //执行测试 
				 
				if(nRet != 0 )
				{
					return nRet;     //如果不为0，测试未完毕，返回
				}
			 
				TestStep = STEP_FINALIZE;
				 

				//如果为0，进入下一个环节。
			}

		case STEP_FINALIZE: //项目测试结束收尾
			{
				int nRet = item->pItem->Finalize();  //执行收尾工作
				if(nRet != 0)	 
				{
					return nRet;  //如果不为0，测试未完毕，返回
				}
				else if (nRet==0)
				{
					//保存数据
					Save(item);
					ShowTestTime(item);
						 
					TestStep = STEP_COMPLETE;

					ShowItemResult(item); //按钮显示结果 
				} 
			}
		case STEP_COMPLETE: //测试结束出口
			{				 
				SetEvent(hSingleTestComplete); 

				return SINGLETEST_COMPLETE;
			}
		case STEP_TERMINATE://强制中断，执行结束函数一次
			{				
				item->pItem->Finalize();
				Save(item);                 //保存数据
				ShowItemResult(item);      //按钮显示结果
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
		str.Format(_T("###########\n DLL FAIL\n,ERROE： %s \n############"),error);
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
	//显示时间
	m_TimeEnd = GetTickCount();
	CString time;
	time.Format(_T("[%s] END \n TIME： %d ms"),item->pItem->GetName(), m_TimeEnd - m_TimeStart);
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
	//得到记录的文件名
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
	//完了，得到测试结果
	switch(item->pItem->GetResult())//根据结果BUTTON显示状态
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
	//case STATUS_OFFLINE: //离线模式(静态图片必须手动测试)，直接切换测试项目
	//	break;
	//case STATUS_RUNNING: 
	//	break;
	//default:  //其他状态，点击无效
	//	return;
	//}
	CString str;
	str.Format(_T("set testItemID:%d"),ID);
	m_pTccm->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);
	//确认状态	
	//如果当前测试正在运行，
	if(isSingleItemTestRuning())
	{	
		str.Format(_T("Stop testItemID:%d"),CurrentTestItem_ID);
		m_pTccm->Ctrl_Addlog(CamID,str,COLOR_BLUE,200);

		//同时响应的ID和当前ID是同一个，
		if (ID == CurrentTestItem_ID) //先判断，再中断，不可弄错顺序防止不同步
		{ 		
			//执行强制结束
			TerminateSingleTest(CurrentTestItem_ID); // 直接返回
			if (m_pTccm->isOffline(CamID))//刷新静态画面
				m_pTccm->Cam[CamID].pImgExplorer->ShowScrImage();
			//return; //返回
		}
		else
		{
			//执行强制结束
			TerminateSingleTest(CurrentTestItem_ID); 
		}
	}
	//刷新静态画面
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
 
 