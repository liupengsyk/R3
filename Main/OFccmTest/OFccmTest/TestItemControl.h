#pragma once
#include "MyTestButton.h"
#include "TestItemInterface.h"
#include "ccmBaseInterface.h"
//#include "OFccmControl.h"
#include "MyTestItem.h"
#include <vector>
using namespace std;

//class ccmBaseInterface;
class OFccmControl;
 
class TestItemControl
{
public:
	TestItemControl(OFccmControl* pTccmControl, HWND hButtonPanel,int CamID);
	~TestItemControl(void);
	
	enum TEST_STATUE
	{
		SINGLETEST_COMPLETE = 0xEEEE,
		SINGLETEST_NULL,
		SINGLETEST_ERROR,
		SINGLETEST_TERMINATE,
	};

	enum BUTTON_STATUS
	{
		BUTTON_RESET,//重置状态
		BUTTON_PASS, //测试结果OK
		BUTTON_RUNNING,//正在测试
		BUTTON_FAIL, //测试结果NG
		BUTTON_NULL, //测试结果为空状态
	}; 
	vector<MyTestItem>* GetMyTestItemArray();
public:
	int  m_TestMode;
	CWnd *p_Main;
private: 
 
	CString ReportPath;
	CString DllPath;

	OFccmControl* m_pTccm;
	
	HWND m_hButtonPanel;
	int LastID; //用于指明当前或上一次测试的项目。

	enum STEP
	{
		STEP_START,
		STEP_INITLIZE,
		STEP_TEST,
		STEP_FINALIZE,
		STEP_COMPLETE,
		STEP_TERMINATE,
		STEP_NULL,
	};

	int  TestStep;

	int m_TimeStart;
	int m_TimeEnd;
	HANDLE hSingleTestComplete;                //单项测试完毕的信号，有信号：测试完毕，无信号：测试中
	BOOL TerminateFlag;

	int ITEM_ID_FIRST;
	int ITEM_ID_LAST;

public:
	vector<MyTestItem> VecTestItem;
     void LoadInspectionItem(vector<CString>* dllnameList);  //根据CONFIG的要加载的DLL名字，加载对应的DLL
	 HMODULE LoadDLL(CString dllPath,CString dllName);
	 HWND CreateTestButton(MyTestItem* pMyTestItem,int ID); 
	 void LoadAllOption();
	 int SaveAllOption(); 
	 int CamID;
	 int TestItemCnt;

public: 
	 
	int Run(); 
	int SingleItemTest(MyTestItem* item);
	BOOL isSingleItemTestRuning(void);
	MyTestItem* GetInspectionItem_FromID(int ID);
	void TerminateSingleTest(int ID);

	void ShowTestTime(MyTestItem* item);
	int PostMessageToItems(DWORD Msg);
	 
	void EnableButton(BOOL enable);
	void ResetALLButtonImage();
	void SetButtonStatus(MyTestItem* item,int Flag);
	int Save(MyTestItem* item);
	void ShowItemResult(MyTestItem* item);

	void FreeAll();

	void SetTestItem(int ID);	
	int GetLastID(void);
	int GetCurrentID(void);
	BOOL bTestResult;

	 
	BOOL isLoadingDll;

public: 
	
	BOOL bItemTestResult;
	BOOL Result_ok2; 
	void DrawingAndShowResult(MyTestItem* item,BOOL bCloseCamera,BOOL bResult);

 
	
private:
	int CurrentTestItem_ID;

	void InitAATest(int TestItemID);	
	int AATest();

	void MachineTestInit();
	int MachineTest();

	int AutoTest();
	int DualCameraAutoTest();
	void InitAutoTest();

	int FocusAutoTest();

	int ManualTest();
	void InitManualTestStatus();
	
	vector<int> MachineItemID;
	int MachineCurrentItemID;

public:
	vector<TestItemInfo> vecTestItemInfo;
	TestItemInfo GetTestInfo(UINT TestItemID);
	TestItemInfo GetTestInfo(CString itemName);

	void SetMachineItemID(vector<int> MachineItemID);
	void SendMachineTestResult(BOOL bResult);
	  
	int TestInit(int TestItemID=0);

	void SetTestInfo(int TestItemID,TestItemInfo mTestItemInfo);

	CRITICAL_SECTION m_csLock; 
private:
	void DualCameraAutoTestTryToWaitPeerCameraTestFinished(int CamID);//双摄自动模式下，尝试等待另一个cam测试完
};

