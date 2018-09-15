#include "stdafx.h"
#include "AFCurve.h"

AFCurve::AFCurve()
{
	NewStartFlag = TRUE;
}
void AFCurve::AddNode(double value, int code, int CurSearchStep)
{
	AFinfo Node;
	Node.m_Value = value;
	Node.m_Code = code;
	Node.m_Index  = AFinfoArray.size();
	Node.m_SearchStep = CurSearchStep;

	if (NewStartFlag)
	{
		Node.StartFlag = TRUE;
		Node.m_Flag = 0;
		Node.m_delta = 0;

		NewStartFlag = FALSE;
	}
	else
	{
		Node.StartFlag = FALSE;
		Node.m_delta = Node.m_Value - AFinfoArray.back().m_Value ; //前后点的差值
		double ratio = 100- 100*Node.m_Value/AFinfoArray.back().m_Value; //前后点的比值差异

		Node.m_Flag = AFinfoArray.back().m_Flag;
		//如果是往下降，同时下降幅度大于阈值，状态--;
		if (Node.m_delta < 0)
		{
			if (ratio > m_CurDiffLimit )
			{		
				Node.m_Flag--;
			}
		}
		else
		{
			Node.m_Flag++;
			if (Node.m_Flag >= 0)
				Node.m_Flag = 0;				
		}
	}


	AFinfoArray.push_back(Node);
}

int AFCurve::FindMax(int CurSearchStep)
{
	double Max=0;
	int index=0;
	for(UINT i=0; i<AFinfoArray.size(); i++)
	{
		if (AFinfoArray[i].m_SearchStep == CurSearchStep)
		{
			if(AFinfoArray[i].m_Value >= Max)
			{
				Max = AFinfoArray[i].m_Value;
				index = i;
			}
		}
		
	}
	return index;
}

int AFCurve::GetNextSearchDirection(int CurSearchStep)
{
	int size = AFinfoArray.size();
	if (size<3) return 0;

	int index = FindMax(CurSearchStep);

	double pre, next;

	if (index == 0)
	{
		pre = AFinfoArray[0].m_Value;
		next = AFinfoArray[index+1].m_Value;
	}
	else
	{
		pre = AFinfoArray[index-1].m_Value;
		next = AFinfoArray[index+1].m_Value;
	}

	if (next <= pre)
	{
		return 1;
	}
	else
	{ 
		return -1;
	}
}

int AFCurve::GetNextSearchStartCode(int CurSearchStep)
{
	int size = AFinfoArray.size();
	 
	if(size<1) return 0;
	int index = FindMax(CurSearchStep);

	double pre, next;

	if (index == 0 || size<3)
	{
		int step = AFinfoArray[index].m_Code - AFinfoArray[0].m_Code;
		return AFinfoArray[0].m_Code +  abs(step/2);
	}	 
	else
	{
		pre = AFinfoArray[index-1].m_Value;
		next = AFinfoArray[index+1].m_Value;
	}
	USHORT targetCode = 0;
	if (next <= pre)
	{
		int step = AFinfoArray[index].m_Code - AFinfoArray[index-1].m_Code;
		targetCode = AFinfoArray[index-1].m_Code +  abs(step/2);	 
	}
	else
	{
		int step = AFinfoArray[index].m_Code - AFinfoArray[index+1].m_Code;
		targetCode = AFinfoArray[index+1].m_Code - abs(step/2); 
	}
	AFinfoArray.clear();

	 
	/*int step = AFinfoArray[index].m_Code - AFinfoArray[index+1].m_Code;
	return AFinfoArray[index].m_Code - abs(step/2);*/

	return targetCode;
}

CString AFCurve::GetCurrentNodeInfo()
{
	CString str;
	AFinfo Node = AFinfoArray.back();
	CString arrow;

	if (Node.m_delta > 0)
	{
		arrow = _T(" ↑ ");
	}
	else if(Node.m_delta < 0)
	{
		arrow = _T(" ↓ ");
	}
	else if(Node.m_delta == 0)
	{
		arrow = _T("");
	}

	str.Format(_T("%02d,C:%d,V:%.2lf,F:%d  %s"),Node.m_Index,Node.m_Code,Node.m_Value,Node.m_Flag,arrow);

	return str;
}

void AFCurve::Init(void)
{
	AFinfoArray.clear();
	NewStartFlag = TRUE;
}


AFinfo AFCurve::GetCurrentNode(void)
{
	return AFinfoArray.back();
}


AFinfo AFCurve::FindMaxNode(int CurSearchStep)
{
	int index = FindMax(CurSearchStep);
	return AFinfoArray[index];
}

void AFCurve::SetNextSearchTarget(int DescendTimeLimt,double DiffLimt)
{
	m_CurDescendTimeLimit = DescendTimeLimt;
	m_CurDiffLimit = DiffLimt;
	NewStartFlag = TRUE;
}

BOOL AFCurve::isSearchTargetReached()
{
	if (GetCurrentNode().m_Flag <= -m_CurDescendTimeLimit)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}


