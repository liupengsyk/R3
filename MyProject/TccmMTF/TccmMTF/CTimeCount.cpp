#include "StdAfx.h"
#include "CTimeCount.h"
// ��ʱ��ʼ
void CTimeCount::Start(void) 
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&old_time);
	use_time = 0;
}

// ��ʱ������������ 
void CTimeCount::End(void)
{
	QueryPerformanceCounter(&new_time);
	use_time = (double)(new_time.QuadPart - old_time.QuadPart) / (double)frequency.QuadPart * 1000;
}

// ����㷨����ʱ�� (��λ: ms)
double CTimeCount::GetUseTime(void) 
{
	return use_time;
}