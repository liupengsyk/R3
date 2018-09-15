#pragma once

#include "ccmBaseInterface.h"
#include "OptionDlg.h"
#include "TestItemInterface.h"
//#include "AFCurve.h"
 
struct AFMoveCode
{
	double MoveDiatance;
	int MoveCode;
	BOOL bCheck;
};
struct Infinity
{
	double InfDistance;
	USHORT RegHigh;
	USHORT RegLow;
	int InfinityCode;
	BOOL bCheck;
};
struct Macro
{
	double MacroDistance;
	USHORT RegHigh;
	USHORT RegLow;
	int MacroCode;
	BOOL bCheck;
};
struct MiddleCode
{
	int MidCode;
	BOOL bCheck;
};
class AF_MoveToPos_TestItem :public TestItemInterface
{
public:
	AF_MoveToPos_TestItem(ccmBaseInterface* pInterface, LPCTSTR lpszName);
	~AF_MoveToPos_TestItem(void);

	int InitItem();

	int Testing();       //�������ز��Դ���ŷ��ڴ˺���
	int Initialize();      //�������� ��ʼ������
	int Finalize();      //�������� ��ʼ��

	int LoadOption();
	int SaveOption();

	LPCTSTR GetReportHeads();
	LPCTSTR GetReportLowLimit();
	LPCTSTR GetReportHightLimit() ;
	LPCTSTR GetReportContents() ;
	AFMoveCode m_AFMoveCode;
	Infinity m_bInfinityCode;
	Macro m_bMacroCode;
	MiddleCode m_bMiddleCode;

	//AFCurve m_Curve;

private:
	OptionDlg* pDlg;
	 int CamID;

public:
	double ModuleEFL;

};