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
		BUTTON_RESET,//����״̬
		BUTTON_PASS, //���Խ��OK
		BUTTON_RUNNING,//���ڲ���
		BUTTON_FAIL, //���Խ��NG
		BUTTON_NULL, //���Խ��Ϊ��״̬
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
	int LastID; //����ָ����ǰ����һ�β��Ե���Ŀ��

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
	HANDLE hSingleTestComplete;                //���������ϵ��źţ����źţ�������ϣ����źţ�������
	BOOL TerminateFlag;

	int ITEM_ID_FIRST;
	int ITEM_ID_LAST;

public:
	vector<MyTestItem> VecTestItem;
     void LoadInspectionItem(vector<CString>* dllnameList);  //����CONFIG��Ҫ���ص�DLL���֣����ض�Ӧ��DLL
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
	void DualCameraAutoTestTryToWaitPeerCameraTestFinished(int CamID);//˫���Զ�ģʽ�£����Եȴ���һ��cam������
};

