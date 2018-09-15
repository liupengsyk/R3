#pragma once

#include  <vector>
using namespace std;

class AFinfo
{
public:
	USHORT m_Code;  //AF寄存器值
	double m_Value;  //MTF值
	double m_delta; //前一个值和后一个值的差值
	int m_Flag; //当前状态
	int m_Index;  //第N个信息
	BOOL StartFlag; //是否是搜索起点
	int m_SearchStep;   //属于哪个Step
};

class SearchParam
{
public:
	SearchParam(){};
	SearchParam(int DescendTimeLimt,double DiffLimt, int StepCode)
	{
		m_DescendTime = DescendTimeLimt;
		m_Diff = DiffLimt;
		m_StepCode = StepCode;
	}

	int m_DescendTime;
	double m_Diff;
	int m_StepCode;
};

class AFCurve
{
public:
	AFCurve();
	vector<AFinfo> AFinfoArray;
	void AddNode(double value, int code, int CurSearchStep);
	int FindMax(int CurSearchStep);
	int GetNextSearchDirection(int CurSearchStep);
	int GetNextSearchStartCode(int CurSearchStep);
	CString GetCurrentNodeInfo();
	void Init(void);
	inline AFinfo GetCurrentNode(void);
	AFinfo FindMaxNode(int CurSearchStep);
	void SetNextSearchTarget(int DescendTimeLimit,double DiffLimit);
	BOOL isSearchTargetReached();

	int m_CurDescendTimeLimit;
	double m_CurDiffLimit;

	BOOL NewStartFlag;

};

