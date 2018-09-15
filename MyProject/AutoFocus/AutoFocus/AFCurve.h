#pragma once

#include  <vector>
using namespace std;

class AFinfo
{
public:
	USHORT m_Code;  //AF�Ĵ���ֵ
	double m_Value;  //MTFֵ
	double m_delta; //ǰһ��ֵ�ͺ�һ��ֵ�Ĳ�ֵ
	int m_Flag; //��ǰ״̬
	int m_Index;  //��N����Ϣ
	BOOL StartFlag; //�Ƿ����������
	int m_SearchStep;   //�����ĸ�Step
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

